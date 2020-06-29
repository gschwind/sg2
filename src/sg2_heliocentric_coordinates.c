/*
 * sg2_heliocentric_coordinates.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_HELIOCENTRIC_COORDINATES_C_

#include "sg2.h"
#include "SG2_precomputed_heliocentric_coordinates.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

S_SG2_HCOORD *SG2_heliocentric_coordinates_create_hcoord(unsigned long n,
		int *p_err) {
	S_SG2_HCOORD *p_hcoord;

	p_hcoord = (S_SG2_HCOORD *) malloc(sizeof(S_SG2_HCOORD));
	if (p_hcoord == NULL) {
		*p_err = SG2_ERR_HELIOCENTRIC_COORDINATES_CREATE_HCOORD_MALLOC_1;
		return NULL;
	}

	p_hcoord->R = (double *) malloc(n * sizeof(double));
	if (p_hcoord->R == NULL) {
		*p_err = SG2_ERR_HELIOCENTRIC_COORDINATES_CREATE_HCOORD_MALLOC_2;
		free(p_hcoord);
		return NULL;
	}
	p_hcoord->L = (double *) malloc(n * sizeof(double));
	if (p_hcoord->L == NULL) {
		*p_err = SG2_ERR_HELIOCENTRIC_COORDINATES_CREATE_HCOORD_MALLOC_3;
		free(p_hcoord->R);
		free(p_hcoord);
		return NULL;
	}

	return p_hcoord;
}

void SG2_heliocentric_coordinates_delete_hcoord(S_SG2_HCOORD *p_hcoord,
		int *p_err) {
	free(p_hcoord->R);
	free(p_hcoord->L);
	free(p_hcoord);
}

void SG2_heliocentric_coordinates_set_hcoord(S_SG2_DATE_JD *p_jd,
		S_SG2_HCOORD *p_hcoord, int *p_err) {
	short idx0;
	double x, x0, dx;
	int k;

	if (p_jd->jd_tt_set != 1) {
		*p_err = SG2_ERR_HELIOCENTRIC_COORDINATES_SET_HCOORD_JDTTNOTSET;
		return;
	}

	p_hcoord->p_jd = p_jd;

	for (k = 0; k < p_jd->n; k++) {

		x = (p_jd->jd_tt[k] - SG2_precomputed_heliocentric_coordinates_R_j0)
				/ SG2_precomputed_heliocentric_coordinates_R_dj;
		idx0 = (short) round(x);
		if ((idx0 < 0)
				|| (idx0 > SG2_precomputed_heliocentric_coordinates_R_nj)) {
			*p_err = SG2_ERR_HELIOCENTRIC_COORDINATES_SET_HCOORD_OUTOFPERIOD;
			return;
		}
		p_hcoord->R[k] = SG2_precomputed_heliocentric_coordinates_R[idx0];

		x = (p_jd->jd_tt[k] - SG2_precomputed_heliocentric_coordinates_L_j0)
				/ SG2_precomputed_heliocentric_coordinates_L_dj;
		x0 = floor(x);
		dx = x - x0;

		idx0 = (short) x0;
		if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_coordinates_L_nj
				- 1)) {
			*p_err = SG2_ERR_HELIOCENTRIC_COORDINATES_SET_HCOORD_OUTOFPERIOD;
			return;
		}

		p_hcoord->L[k] = (1.0 - dx)
				* SG2_precomputed_heliocentric_coordinates_L[idx0] + dx
				* SG2_precomputed_heliocentric_coordinates_L[idx0 + 1];

	}
}
