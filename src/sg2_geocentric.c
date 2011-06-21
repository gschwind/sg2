/*
 * sg2_geocentric.c
 *
 *  Created on: 4 f�vr. 2011
 *      Author: pblanc
 */

#define SG2_GEOCENTRIC_C_

#include "sg2.h"
#include "sg2_precomputed_heliocentric.h"
#include "sg2_precomputed_geocentric.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

S_SG2_GEOC_DATA *SG2_geocentric_create_geoc_data(unsigned long nd, int *p_err) {
	S_SG2_GEOC_DATA *p_geoc;

	p_geoc = (S_SG2_GEOC_DATA *) malloc(sizeof(S_SG2_GEOC_DATA));
	if (p_geoc == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_1;
		return NULL;
	}

	p_geoc->nd = nd;
	p_geoc->Delta_psi = NULL;
	p_geoc->EOT = NULL;
	p_geoc->Theta_a = NULL;
	p_geoc->delta = NULL;
	p_geoc->epsilon = NULL;
	p_geoc->nu = NULL;
	p_geoc->p_helioc = NULL;
	p_geoc->p_jd = NULL;

	p_geoc->epsilon = (double *) malloc(nd * sizeof(double));
	if (p_geoc->epsilon == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_2;
		free(p_geoc);
		return NULL;
	}
	p_geoc->Theta_a = (double *) malloc(nd * sizeof(double));
	if (p_geoc->Theta_a == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_3;
		free(p_geoc->epsilon);
		free(p_geoc);
		return NULL;
	}
	p_geoc->r_alpha = (double *) malloc(nd * sizeof(double));
	if (p_geoc->r_alpha == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_4;
		free(p_geoc->epsilon);
		free(p_geoc->Theta_a);
		free(p_geoc);
		return NULL;
	}
	p_geoc->delta = (double *) malloc(nd * sizeof(double));
	if (p_geoc->delta == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_5;
		free(p_geoc->epsilon);
		free(p_geoc->Theta_a);
		free(p_geoc->r_alpha);
		free(p_geoc);
		return NULL;
	}

	p_geoc->Delta_psi = (double *) malloc(nd * sizeof(double));
	if (p_geoc->Delta_psi == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_6;
		free(p_geoc->epsilon);
		free(p_geoc->Theta_a);
		free(p_geoc->r_alpha);
		free(p_geoc->delta);
		free(p_geoc);
		return NULL;
	}

	p_geoc->nu = (double *) malloc(nd * sizeof(double));
	if (p_geoc->nu == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_7;
		free(p_geoc->epsilon);
		free(p_geoc->Theta_a);
		free(p_geoc->r_alpha);
		free(p_geoc->delta);
		free(p_geoc->Delta_psi);
		free(p_geoc);
		return NULL;
	}

	p_geoc->EOT = (double *) malloc(nd * sizeof(double));
	if (p_geoc->EOT == NULL) {
		*p_err = SG2_ERR_GEOCENTRIC_CREATE_GEOC_MALLOC_8;
		free(p_geoc->epsilon);
		free(p_geoc->Theta_a);
		free(p_geoc->r_alpha);
		free(p_geoc->delta);
		free(p_geoc->Delta_psi);
		free(p_geoc->nu);
		free(p_geoc);
		return NULL;
	}

	return p_geoc;
}

void SG2_geocentric_delete_geoc_data(S_SG2_GEOC_DATA *p_geoc, int *p_err) {
	free(p_geoc->epsilon);
	free(p_geoc->Theta_a);
	free(p_geoc->r_alpha);
	free(p_geoc->delta);
	free(p_geoc->Delta_psi);
	free(p_geoc->nu);
	free(p_geoc->EOT);
	free(p_geoc);
}

void SG2_geocentric_set_geoc_data(S_SG2_DATE_TABJD *p_jd, S_SG2_HELIOC_DATA *p_helioc,
		S_SG2_GEOC_DATA *p_geoc, int *p_err) {
	short idx0;
	int kd;
	double sin_Theta_a_kd, cos_epsilon_kd;
	double nu0_kd, Delta_psi_cos_epsilon_kd, M_kd;

	if (p_jd->jd_tt_set != 1) {
		*p_err = SG2_ERR_GEOCENTRIC_SET_GEOC_JDTTNOTSET;
		return;
	}

	p_geoc->p_jd = p_jd;
	p_geoc->p_helioc = p_helioc;

	for (kd = 0; kd < p_jd->nd; kd++) {

		idx0 = (short) round((p_jd->jd_tt[kd]
				- SG2_precomputed_geocentric_Delta_psi_j0)
				/ SG2_precomputed_geocentric_Delta_psi_dj);
		if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_Delta_psi_nj)) {
			*p_err = SG2_ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
			return;
		}
		p_geoc->Delta_psi[kd] = SG2_precomputed_geocentric_Delta_psi[idx0];

		p_geoc->Theta_a[kd] = p_helioc->L[kd] + SG2_PI + p_geoc->Delta_psi[kd]
				+ SG2_precomputed_geocentric_Delta_tau;

		/*

		 idx0 = (short) round((p_jd->jd_tt[k]
		 - SG2_precomputed_geocentric_epsilon_j0)
		 / SG2_precomputed_geocentric_epsilon_dj);
		 if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_R_nj)) {
		 *p_err = SG2_ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
		 return;
		 }
		 */

		p_geoc->epsilon[kd] = SG2_precomputed_geocentric_epsilon[idx0];

		sin_Theta_a_kd = sin(p_geoc->Theta_a[kd]);
		cos_epsilon_kd = cos(p_geoc->epsilon[kd]);

		p_geoc->r_alpha[kd] = atan2(sin_Theta_a_kd * cos_epsilon_kd, cos(
				p_geoc->Theta_a[kd]));
		p_geoc->delta[kd] = asin(sin_Theta_a_kd * sin(p_geoc->epsilon[kd]));

		nu0_kd = 6.300388099 * p_jd->jd_ut[kd] - 1.539965571482657e+007;
		Delta_psi_cos_epsilon_kd = p_geoc->Delta_psi[kd] * cos_epsilon_kd;
		M_kd = 1.720279169744191e-002*p_jd->jd_tt[kd]-4.204914238795757e+004;

		p_geoc->nu[kd] = nu0_kd + Delta_psi_cos_epsilon_kd;
		p_geoc->EOT[kd] = M_kd - 0.0001 - p_geoc->r_alpha[kd] + Delta_psi_cos_epsilon_kd;
	}
}
