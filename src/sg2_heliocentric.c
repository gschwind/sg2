/*
 * sg2_heliocentric.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_HELIOCENTRIC_C_

#include "sg2.h"
#include "SG2_precomputed_heliocentric.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

S_SG2_HELIOC_DATA *SG2_heliocentric_create_helioc_data(unsigned long n, int *p_err) {
	S_SG2_HELIOC_DATA *p_helioc;

	p_helioc = (S_SG2_HELIOC_DATA *) malloc(sizeof(S_SG2_HELIOC_DATA));
	if (p_helioc == NULL) {
		*p_err = SG2_ERR_HELIOCENTRIC_CREATE_HELIOC_MALLOC_1;
		return NULL;
	}

	p_helioc->R = (double *) malloc(n * sizeof(double));
	if (p_helioc->R == NULL) {
		*p_err = SG2_ERR_HELIOCENTRIC_CREATE_HELIOC_MALLOC_2;
		free(p_helioc);
		return NULL;
	}
	p_helioc->L = (double *) malloc(n * sizeof(double));
	if (p_helioc->L == NULL) {
		*p_err = SG2_ERR_HELIOCENTRIC_CREATE_HELIOC_MALLOC_3;
		free(p_helioc->R);
		free(p_helioc);
		return NULL;
	}

	return p_helioc;
}

void SG2_heliocentric_delete_helioc_data(S_SG2_HELIOC_DATA *p_helioc, int *p_err) {
	free(p_helioc->R);
	free(p_helioc->L);
	free(p_helioc);
}

void SG2_heliocentric_set_helioc_data(S_SG2_DATE_TABJD *p_jd, S_SG2_HELIOC_DATA *p_helioc,
		int *p_err) {
	short idx0;
	double x, x0, dx;
	int k;

	if (p_jd->jd_tt_set != 1) {
		*p_err = SG2_ERR_HELIOCENTRIC_SET_HELIOC_JDTTNOTSET;
		return;
	}

	p_helioc->p_jd = p_jd;

	for (k = 0; k < p_jd->nd; k++) {

		x = (p_jd->jd_tt[k] - SG2_precomputed_heliocentric_R_j0)
				/ SG2_precomputed_heliocentric_R_dj;
		idx0 = (short) round(x);
		if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_R_nj)) {
			*p_err = SG2_ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
			return;
		}
		p_helioc->R[k] = SG2_precomputed_heliocentric_R[idx0];

		x = (p_jd->jd_tt[k] - SG2_precomputed_heliocentric_L_j0)
				/ SG2_precomputed_heliocentric_L_dj;
		x0 = floor(x);
		dx = x - x0;

		idx0 = (short) x0;
		if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_L_nj - 1)) {
			*p_err = SG2_ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
			return;
		}

		p_helioc->L[k] = (1.0 - dx) * SG2_precomputed_heliocentric_L[idx0] + dx
				* SG2_precomputed_heliocentric_L[idx0 + 1];

	}
}
