/*
 *  sg2_date.c
 *
 *  Created on: 1 feb. 2011
 *  Author: pblanc
 */

#define SG2_DATE_C_

#include "sg2.h"
#include "SG2_precomputed_delta_tt.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int SG2_date_leapyear(short year) {
	return (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));
}

S_SG2_DATE_JD *SG2_date_create_jd(unsigned long n, int *p_err) {
	S_SG2_DATE_JD *p_jd = NULL;

	p_jd = (S_SG2_DATE_JD *) malloc(sizeof(S_SG2_DATE_JD));
	if (p_jd == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_JD_MALLOC_1;
		return NULL;
	}
	p_jd->n = n;
	p_jd->jd = (double *) malloc(p_jd->n * sizeof(double));
	if (p_jd->jd == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_JD_MALLOC_2;
		free(p_jd);
		return NULL;
	}
	p_jd->Delta_tt = (double *) malloc(p_jd->n * sizeof(double));
	if (p_jd->Delta_tt == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_JD_MALLOC_3;
		free(p_jd->jd);
		free(p_jd);
		return NULL;
	}
	return p_jd;
}

void SG2_date_delete_jd(S_SG2_DATE_JD *p_jd, int *p_err) {
	free(p_jd->jd);
	free(p_jd->Delta_tt);
	free(p_jd);
}

S_SG2_DATE_YMD_H *SG2_date_create_ymd_h(unsigned long n, int *p_err) {
	S_SG2_DATE_YMD_H *p_ymdh = NULL;

	p_ymdh = (S_SG2_DATE_YMD_H *) malloc(sizeof(S_SG2_DATE_YMD_H));
	if (p_ymdh == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_1;
		return NULL;
	}
	p_ymdh->n = n;
	p_ymdh->y = (short *) malloc(p_ymdh->n * sizeof(short));
	if (p_ymdh->y == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_2;
		free(p_ymdh);
		return NULL;
	}
	p_ymdh->m = (short *) malloc(p_ymdh->n * sizeof(short));
	if (p_ymdh->m == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_3;
		free(p_ymdh->y);
		free(p_ymdh);
		return NULL;
	}
	p_ymdh->d = (short *) malloc(p_ymdh->n * sizeof(short));
	if (p_ymdh->d == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_4;
		free(p_ymdh->y);
		free(p_ymdh->m);
		free(p_ymdh);
		return NULL;
	}

	p_ymdh->h = (double *) malloc(p_ymdh->n * sizeof(double));
	if (p_ymdh->h == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_5;
		free(p_ymdh->y);
		free(p_ymdh->m);
		free(p_ymdh->d);
		free(p_ymdh);
		return NULL;
	}

	return p_ymdh;
}

void SG2_date_delete_ymd_h(S_SG2_DATE_YMD_H *p_ymdh, int *p_err) {
	free(p_ymdh->y);
	free(p_ymdh->m);
	free(p_ymdh->d);
	free(p_ymdh->h);
	free(p_ymdh);
}

S_SG2_DATE_YDOY_H *SG2_date_create_ydoy_h(unsigned long n, int *p_err) {
	S_SG2_DATE_YDOY_H *p_ydoyh = NULL;

	p_ydoyh = (S_SG2_DATE_YDOY_H *) malloc(sizeof(S_SG2_DATE_YDOY_H));
	if (p_ydoyh == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_1;
		return NULL;
	}
	p_ydoyh->n = n;
	p_ydoyh->y = (short *) malloc(p_ydoyh->n * sizeof(short));
	if (p_ydoyh->y == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_2;
		free(p_ydoyh);
		return NULL;
	}
	p_ydoyh->doy = (short *) malloc(p_ydoyh->n * sizeof(short));
	if (p_ydoyh->doy == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_3;
		free(p_ydoyh->y);
		free(p_ydoyh);
		return NULL;
	}

	p_ydoyh->h = (double *) malloc(p_ydoyh->n * sizeof(double));
	if (p_ydoyh->h == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_4;
		free(p_ydoyh->y);
		free(p_ydoyh->doy);
		free(p_ydoyh);
		return NULL;
	}

	return p_ydoyh;
}

void SG2_date_delete_ydoy_h(S_SG2_DATE_YDOY_H *p_ydoyh, int *p_err) {
	free(p_ydoyh->y);
	free(p_ydoyh->doy);
	free(p_ydoyh->h);
	free(p_ydoyh);
}

void SG2_date_jd_to_ymdh(S_SG2_DATE_JD *p_jd, S_SG2_DATE_YMD_H *p_ymdh,
		int *p_err) {
	int k;
	double H, L, N, I, J, K, jd;

	for (k = 0; k < p_jd->n; k++) {

		jd = p_jd->jd[k];

		H = (jd + 0.5 - floor(jd + 0.5)) * 24.0;
		L = floor(jd + 0.5) + 68569.0;
		N = floor(4 * L / 146097.0);
		L = L - floor((146097.0 * N + 3.0) / 4.0);
		I = floor(4000 * (L + 1) / 1461001.0);
		L = L - floor(1461.0 * I / 4.0) + 31.0;

		J = floor(80.0 * L / 2447.0);
		K = L - floor(2447.0 * J / 80.0);
		L = floor(J / 11.0);
		J = J + 2.0 - 12.0 * L;
		I = 100.0 * (N - 49.0) + I + L;

		p_ymdh->y[k] = (short) I;
		p_ymdh->m[k] = (short) J;
		p_ymdh->d[k] = (short) K;
		p_ymdh->h[k] = H;

	}

}

void SG2_date_ymdh_to_jd(S_SG2_DATE_YMD_H *p_ymdh, S_SG2_DATE_JD *p_jd,
		int *p_err) {

	int k;
	double Y, M, D, H;

	for (k = 0; k < p_ymdh->n; k++) {

		Y = p_ymdh->y[k];
		M = p_ymdh->m[k];
		D = p_ymdh->d[k];
		H = p_ymdh->h[k];

		if (M < 3) {
			M += 12;
			Y -= 1;
		}

		p_jd->jd[k] = 1721028.0 + D + floor((153.0 * M - 2.0) / 5.0) + 365.0
				* Y + floor(Y / 4.0) - floor(Y / 100.0) + floor(Y / 400.0) + H
				/ 24.0 - 0.5;

	}

}

void SG2_date_ymdh_to_ydoy(S_SG2_DATE_YMD_H *p_ymdh,
		S_SG2_DATE_YDOY_H *p_ydoyh, int *p_err) {
	int k;
	double K;

	for (k = 0; k < p_ymdh->n; k++) {
		K = 2 - SG2_date_leapyear(p_ymdh->y[k]);
		p_ydoyh->y[k] = p_ymdh->y[k];
		p_ydoyh->h[k] = p_ymdh->h[k];
		p_ydoyh->doy[k] = (short) floor(275.0 * ((double) p_ymdh->m[k]) / 9.0)
				- K * ((double) (p_ymdh->m[k] > 2)) + ((double) p_ymdh->d[k])
				- 30.0;
	}

}

void SG2_date_ydoy_to_ymdh(S_SG2_DATE_YDOY_H *p_ydoyh,
		S_SG2_DATE_YMD_H *p_ymdh, int *p_err) {
	int k;
	double B;

	for (k = 0; k < p_ydoyh->n; k++) {
		B = (double) SG2_date_leapyear(p_ydoyh->y[k]);
		p_ymdh->y[k] = p_ydoyh->y[k];
		p_ymdh->h[k] = p_ydoyh->h[k];

		if (p_ydoyh->doy[k] < 32) {
			p_ymdh->m[k] = 1;
		} else {
			p_ymdh->m[k] = 1 + (short) (floor((303.0 + 5.
					* (((double) p_ydoyh->doy[k]) - 59.0 - B)) / 153.0));
		}

		if (p_ymdh->m[k] < 2) {
			p_ymdh->d[k] = p_ydoyh->doy[k] - 31 * (p_ymdh->m[k] - 1);
		} else {
			p_ymdh->d[k] = p_ydoyh->doy[k] - ((short) (floor((153.0
					* ((double) p_ymdh->m[k]) - 2.0) / 5.0) - 32.0 + B));
		}

	}

}

void SG2_date_jd_set_Delta_tt(S_SG2_DATE_JD *p_jd, int *p_err) {

	int k;
	long idx;

	for (k = 0; k < p_jd->n; k++) {
		idx = (short) round((p_jd->jd[k]
				- SG2_precomputed_delta_tt_j0)
				/ SG2_precomputed_delta_tt_dj);
		if ((idx < 0) || (idx > SG2_precomputed_delta_tt_nj)) {
			*p_err = SG2_ERR_DATE_JD_DELTA_TT_UT_OUTOFPERIOD;
			return;
		}
		p_jd->Delta_tt[k] = SG2_precomputed_delta_tt[idx];
	}

}

