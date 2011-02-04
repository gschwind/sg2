/*
 * sg2_geocentric_coordinates.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_HELIOCENTRIC_COORDINATES_C_

#include "sg2.h"
#include "SG2_precomputed_heliocentric_coordinates.h"
#include "SG2_precomputed_geocentric_coordinates.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

S_SG2_GCOORD *SG2_geocentric_coordinates_create_gcoord(unsigned long n,
		int *p_err) {
	S_SG2_GCOORD *p_gcoord;

	p_gcoord = (S_SG2_GCOORD *) malloc(sizeof(S_SG2_GCOORD));
	if (p_gcoord == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_CREATE_GCOORD_MALLOC_1;
		return NULL;
	}

	p_gcoord->epsilon = (double *) malloc(n * sizeof(double));
	if (p_gcoord->epsilon == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_CREATE_GCOORD_MALLOC_2;
		free(p_gcoord);
		return NULL;
	}
	p_gcoord->Theta_a = (double *) malloc(n * sizeof(double));
	if (p_gcoord->Theta_a == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_CREATE_GCOORD_MALLOC_3;
		free(p_gcoord->epsilon);
		free(p_gcoord);
		return NULL;
	}
	p_gcoord->r_alpha = (double *) malloc(n * sizeof(double));
	if (p_gcoord->r_alpha == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_CREATE_GCOORD_MALLOC_4;
		free(p_gcoord->epsilon);
		free(p_gcoord->Theta_a);
		free(p_gcoord);
		return NULL;
	}
	p_gcoord->delta = (double *) malloc(n * sizeof(double));
	if (p_gcoord->delta == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_CREATE_GCOORD_MALLOC_5;
		free(p_gcoord->epsilon);
		free(p_gcoord->Theta_a);
		free(p_gcoord->r_alpha);
		free(p_gcoord);
		return NULL;
	}

	p_gcoord->R = (double *) malloc(n * sizeof(double));
	if (p_gcoord->R == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_CREATE_GCOORD_MALLOC_6;
		free(p_gcoord->epsilon);
		free(p_gcoord->Theta_a);
		free(p_gcoord->r_alpha);
		free(p_gcoord->delta);
		free(p_gcoord);
		return NULL;
	}
	return p_gcoord;
}

void SG2_geocentric_coordinates_delete_gcoord(S_SG2_GCOORD *p_gcoord,
		int *p_err) {
	free(p_gcoord->epsilon);
	free(p_gcoord->Theta_a);
	free(p_gcoord->r_alpha);
	free(p_gcoord->delta);
	free(p_gcoord->R);
	free(p_gcoord);
}

void SG2_geocentric_coordinates_set_gcoord(S_SG2_DATE_JD *p_jd,
		S_SG2_GCOORD *p_gcoord, int *p_err) {
	short idx0;
	double x, x0, dx;
	int k;
	double sin_Theta_a,cos_epsilon;

	if (p_jd->jd_tt_set != 1) {
		*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_SET_GCOORD_JDTTNOTSET;
		return;
	}

	p_gcoord->p_jd = p_jd;

	for (k = 0; k < p_jd->n; k++) {

		idx0 = (short) round((p_jd->jd_tt[k] - SG2_precomputed_heliocentric_coordinates_R_j0)
				/ SG2_precomputed_heliocentric_coordinates_R_dj);
		if ((idx0 < 0)
				|| (idx0 > SG2_precomputed_heliocentric_coordinates_R_nj)) {
			*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_SET_GCOORD_OUTOFPERIOD;
			return;
		}
		p_gcoord->R[k] = SG2_precomputed_heliocentric_coordinates_R[idx0];

		x = (p_jd->jd_tt[k] - SG2_precomputed_geocentric_coordinates_Theta_a_j0)
				/ SG2_precomputed_geocentric_coordinates_Theta_a_dj;
		x0 = floor(x);
		dx = x - x0;

		idx0 = (short) x0;
		if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_coordinates_Theta_a_nj
				- 1)) {
			*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_SET_GCOORD_OUTOFPERIOD;
			return;
		}
		p_gcoord->Theta_a[k] = (1.0 - dx)
				* SG2_precomputed_geocentric_coordinates_Theta_a[idx0] + dx
				* SG2_precomputed_geocentric_coordinates_Theta_a[idx0 + 1];

		idx0 = (short) round((p_jd->jd_tt[k] - SG2_precomputed_geocentric_coordinates_epsilon_j0)
				/ SG2_precomputed_geocentric_coordinates_epsilon_dj);
		if ((idx0 < 0)
				|| (idx0 > SG2_precomputed_heliocentric_coordinates_R_nj)) {
			*p_err = SG2_ERR_GEOCENTRIC_COORDINATES_SET_GCOORD_OUTOFPERIOD;
			return;
		}
		p_gcoord->epsilon[k] = SG2_precomputed_geocentric_coordinates_epsilon[idx0];

		sin_Theta_a = sin(p_gcoord->Theta_a[k]);
		cos_epsilon = cos(p_gcoord->epsilon[k]);

		p_gcoord->r_alpha[k] = atan2(sin_Theta_a*cos_epsilon,cos(p_gcoord->Theta_a[k]));
		p_gcoord->delta[k] = asin(sin_Theta_a*sin(p_gcoord->epsilon[k]));

	}
}
