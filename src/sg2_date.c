/*
 *  sg2_date.c
 *
 *  Created on: 1 feb. 2011
 *  Author: pblanc
 */

#define SG2_DATE_C_

#include "sg2.h"
#include "sg2_precomputed_delta_tt.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int SG2_date_leapyear(short year) {
	return (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));
}

S_SG2_DATE_TABJD *SG2_date_create_tabjd(unsigned long nd, int *p_err) {
	S_SG2_DATE_TABJD *p_jd = NULL;

	p_jd = (S_SG2_DATE_TABJD *) malloc(sizeof(S_SG2_DATE_TABJD));
	if (p_jd == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_JD_MALLOC_1;
		return NULL;
	}
	p_jd->nd = nd;
	p_jd->jd_ut = (double *) malloc(p_jd->nd * sizeof(double));
	if (p_jd->jd_ut == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_JD_MALLOC_2;
		free(p_jd);
		return NULL;
	}
	p_jd->jd_tt_set = 0;
	p_jd->jd_tt = (double *) malloc(p_jd->nd * sizeof(double));
	if (p_jd->jd_tt == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_JD_MALLOC_3;
		free(p_jd->jd_ut);
		free(p_jd);
		return NULL;
	}

	return p_jd;
}

void SG2_date_delete_tabjd(S_SG2_DATE_TABJD *p_jd, int *p_err) {
	free(p_jd->jd_ut);
	free(p_jd->jd_tt);
	free(p_jd);
}

S_SG2_DATE_TABYMDH *SG2_date_create_tabymdh(unsigned long nd, int *p_err) {
	S_SG2_DATE_TABYMDH *p_ymdh = NULL;

	p_ymdh = (S_SG2_DATE_TABYMDH *) malloc(sizeof(S_SG2_DATE_TABYMDH));
	if (p_ymdh == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_1;
		return NULL;
	}
	p_ymdh->nd = nd;
	p_ymdh->y = (short *) malloc(p_ymdh->nd * sizeof(short));
	if (p_ymdh->y == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_2;
		free(p_ymdh);
		return NULL;
	}
	p_ymdh->m = (short *) malloc(p_ymdh->nd * sizeof(short));
	if (p_ymdh->m == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_3;
		free(p_ymdh->y);
		free(p_ymdh);
		return NULL;
	}
	p_ymdh->d = (short *) malloc(p_ymdh->nd * sizeof(short));
	if (p_ymdh->d == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YMD_H_MALLOC_4;
		free(p_ymdh->y);
		free(p_ymdh->m);
		free(p_ymdh);
		return NULL;
	}

	p_ymdh->h = (double *) malloc(p_ymdh->nd * sizeof(double));
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

void SG2_date_delete_tabymdh(S_SG2_DATE_TABYMDH *p_ymdh, int *p_err) {
	free(p_ymdh->y);
	free(p_ymdh->m);
	free(p_ymdh->d);
	free(p_ymdh->h);
	free(p_ymdh);
}

S_SG2_DATE_TABYDOYH *SG2_date_create_tabydoyh(unsigned long nd, int *p_err) {
	S_SG2_DATE_TABYDOYH *p_ydoyh = NULL;

	p_ydoyh = (S_SG2_DATE_TABYDOYH *) malloc(sizeof(S_SG2_DATE_TABYDOYH));
	if (p_ydoyh == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_1;
		return NULL;
	}
	p_ydoyh->nd = nd;
	p_ydoyh->y = (short *) malloc(p_ydoyh->nd * sizeof(short));
	if (p_ydoyh->y == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_2;
		free(p_ydoyh);
		return NULL;
	}
	p_ydoyh->doy = (short *) malloc(p_ydoyh->nd * sizeof(short));
	if (p_ydoyh->doy == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_3;
		free(p_ydoyh->y);
		free(p_ydoyh);
		return NULL;
	}

	p_ydoyh->h = (double *) malloc(p_ydoyh->nd * sizeof(double));
	if (p_ydoyh->h == NULL) {
		*p_err = SG2_ERR_DATE_CREATE_YDOY_H_MALLOC_4;
		free(p_ydoyh->y);
		free(p_ydoyh->doy);
		free(p_ydoyh);
		return NULL;
	}

	return p_ydoyh;
}

void SG2_date_delete_tabydoyh(S_SG2_DATE_TABYDOYH *p_ydoyh, int *p_err) {
	free(p_ydoyh->y);
	free(p_ydoyh->doy);
	free(p_ydoyh->h);
	free(p_ydoyh);
}

void SG2_date_tabjd_to_tabymdh(S_SG2_DATE_TABJD *p_jd, S_SG2_DATE_TABYMDH *p_ymdh,
		int *p_err) {
	int k;
	double H, L, N, I, J, K, jd;

	for (k = 0; k < p_jd->nd; k++) {

		jd = p_jd->jd_ut[k];

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

void SG2_date_tabymdh_to_tabjd(S_SG2_DATE_TABYMDH *p_ymdh, S_SG2_DATE_TABJD *p_jd,
		int *p_err) {

	int k;
	double Y, M, D, H;

	for (k = 0; k < p_ymdh->nd; k++) {

		Y = p_ymdh->y[k];
		M = p_ymdh->m[k];
		D = p_ymdh->d[k];
		H = p_ymdh->h[k];

		if (M < 3) {
			M += 12;
			Y -= 1;
		}

		p_jd->jd_ut[k] = 1721028.0 + D + floor((153.0 * M - 2.0) / 5.0) + 365.0
				* Y + floor(Y / 4.0) - floor(Y / 100.0) + floor(Y / 400.0) + H
				/ 24.0 - 0.5;

	}
	p_jd->jd_tt_set = 0;

}

void SG2_date_tabymdh_to_tabydoyh(S_SG2_DATE_TABYMDH *p_ymdh,
		S_SG2_DATE_TABYDOYH *p_ydoyh, int *p_err) {
	int k;
	double K;

	for (k = 0; k < p_ymdh->nd; k++) {
		K = 2 - SG2_date_leapyear(p_ymdh->y[k]);
		p_ydoyh->y[k] = p_ymdh->y[k];
		p_ydoyh->h[k] = p_ymdh->h[k];
		p_ydoyh->doy[k] = (short) floor(275.0 * ((double) p_ymdh->m[k]) / 9.0)
				- K * ((double) (p_ymdh->m[k] > 2)) + ((double) p_ymdh->d[k])
				- 30.0;
	}

}

void SG2_date_tabydoyh_to_tabymdh(S_SG2_DATE_TABYDOYH *p_ydoyh,
		S_SG2_DATE_TABYMDH *p_ymdh, int *p_err) {
	int k;
	double B;

	for (k = 0; k < p_ydoyh->nd; k++) {
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

void SG2_date_set_tabjd_tt(S_SG2_DATE_TABJD *p_jd, double *p_delta_tt, int *p_err) {

	int k;
	long idx;

	if (p_delta_tt == NULL) {
		for (k = 0; k < p_jd->nd; k++) {
			idx = (short) round((p_jd->jd_ut[k] - SG2_precomputed_delta_tt_j0)
					/ SG2_precomputed_delta_tt_dj);
			if ((idx < 0) || (idx > SG2_precomputed_delta_tt_nj)) {
				*p_err = SG2_ERR_DATE_JD_SET_JD_TT_OUTOFPERIOD;
				return;
			}
			p_jd->jd_tt[k] = p_jd->jd_ut[k] + SG2_precomputed_delta_tt[idx]
					/ 86400.0;
		}
	} else {
		for (k = 0; k < p_jd->nd; k++) {
			p_jd->jd_tt[k] = p_jd->jd_tt[k] + p_delta_tt[k] / 86400.0;
		}
	}
	p_jd->jd_tt_set = 1;
}

