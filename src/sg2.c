/*
 * sg2.c
 *
 *  Created on: 10 févr. 2011
 *      Author: pblanc
 */

#define SG2_C_

#include "sg2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SG2_MACRO_CHECKERR(ret) if (*p_err != 0) { return (ret);}

S_SG2_SUNPOS *sg2_create_sunpos(unsigned long np, unsigned long nd, int *p_err) {

	S_SG2_SUNPOS *p_sunpos;
	double *p_tmp1, *p_tmp2, *p_tmp3;
	int kp;

	p_sunpos = (S_SG2_SUNPOS *) malloc(sizeof(S_SG2_SUNPOS));
	if (p_sunpos == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_1;
		return NULL;
	}

	p_tmp1 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp1 == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_2;
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->alpha_S = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->alpha_S == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_3;
		free(p_tmp1);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_sunpos->alpha_S[kp] = &p_tmp1[kp * nd];
	}

	p_tmp2 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp2 == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_4;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->gamma_S0 = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->gamma_S0 == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_5;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_sunpos->gamma_S0[kp] = &p_tmp2[kp * nd];
	}

	p_tmp3 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp3 == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_6;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->gamma_S = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->gamma_S == NULL) {
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_7;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp3);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_sunpos->gamma_S[kp] = &p_tmp3[kp * nd];
	}

	return p_sunpos;
}

void sg2_sunpos_delete(S_SG2_SUNPOS *p_sunpos, int *p_err) {
	free(p_sunpos->alpha_S[0]);
	free(p_sunpos->alpha_S);
	free(p_sunpos->gamma_S0[0]);
	free(p_sunpos->gamma_S0);
	free(p_sunpos->gamma_S[0]);
	free(p_sunpos->gamma_S);
	free(p_sunpos);
}

void sg2_sunpos_set(double *p_lon, double *p_lat, double *p_elevation,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, double *p_jd_ut,
		double *p_delta_tt,
		SG2_CORRECTION_REFRACTION method, double *p_data_corr,
		S_SG2_SUNPOS *p_sunpos, int *p_err) {

	S_SG2_DATE_TABJD *p_jd;
	S_SG2_HELIOC_DATA *p_helioc;
	S_SG2_GEOC_DATA *p_geoc;
	S_SG2_TABGEOPOINT *p_gp;
	S_SG2_TOPOC_DATA *p_topoc;
	int kd;
	int nd;
	int np;
	int sz_data;

	np = p_sunpos->np;
	nd = p_sunpos->nd;

	p_jd = SG2_date_create_tabjd(nd, p_err);

	for (kd = 1; kd < nd; kd++) {
		p_jd->jd_ut[kd] = p_jd_ut[kd];
	}
	SG2_date_set_tabjd_tt(p_jd, p_delta_tt, p_err);

	p_gp = SG2_topocentric_create_tabgeopoint(np, ellpstype, p_data_ellps,
			p_err);

	SG2_topocecentric_set_tabgeopoint(p_lon, p_lat, p_elevation, p_gp, p_err);

	p_helioc = SG2_heliocentric_create_helioc_data(p_jd->nd, p_err);

	SG2_heliocentric_set_helioc_data(p_jd, p_helioc, p_err);

	p_geoc = SG2_geocentric_create_geoc_data(p_jd->nd, p_err);

	SG2_geocentric_set_geoc_data(p_jd, p_helioc, p_geoc, p_err);

	p_topoc = SG2_topocentric_create_topoc_data(nd, np, p_err);

	SG2_topocentric_set_topoc_data(p_geoc, p_gp, p_topoc, p_err);

	sz_data = np * nd * sizeof(double);
	memcpy(p_sunpos->alpha_S, p_topoc->alpha_S, sz_data);
	memcpy(p_sunpos->gamma_S0, p_topoc->gamma_S0, sz_data);
	SG2_topocentric_correction_refraction(p_sunpos->gamma_S0[0], np * nd,
			method, p_data_corr, p_sunpos->gamma_S[0], p_err);

	SG2_date_delete_tabjd(p_jd, p_err);
	SG2_geocentric_delete_geoc_data(p_geoc, p_err);
	SG2_heliocentric_delete_helioc_data(p_helioc, p_err);
	SG2_topocentric_delete_tabgeopoint(p_gp, p_err);
	SG2_topocentric_delete_topoc_data(p_topoc, p_err);

}

