/*
 * sg2_topocentric.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_TOPOCENTRIC_C_

#include "sg2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static S_SG2_ELLPS tab_ellps_ref[] = {
		{ .a = 6378137.0, .f = 3.352810664747481e-003 }, /* WGS84 */
		{ .a = 6378137.0, .f = 3.352810681182319e-003 }, /* RFG83 */
		{ .a = 6378249.2, .f = 3.407549520015651e-003 }, /* NTF / CLARKE1880 */
		{ .a = 6378136.6, .f = 3.352819697896193e-003 }, /* AA */
		{ .a = 6378140.0, .f = 3.352810000000000e-003 }, /* SPA */
		{ .a = 6378169.0, .f = 3.384231430681783e-003 }, /* NGP*/
		{ .a = 6378130.0, .f = 0.0 }, /* SPHERE */
		{ .a = 0.0, .f = 0.0 }, /* USER-DEFINED */
};

S_SG2_GEOPOINT *SG2_topocentric_create_geopoint(unsigned long p,
		SG2_ELLPSTYPE ellpstype, int *p_err) {
	S_SG2_GEOPOINT *p_gp;

	p_gp = (S_SG2_GEOPOINT*) malloc(sizeof(S_SG2_GEOPOINT));
	if (p_gp == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPOINT_MALLOC_1;
		return NULL;
	}
	p_gp->p = p;
	p_gp->p_ellps = (S_SG2_ELLPS *) malloc(sizeof(S_SG2_ELLPS));
	if (p_gp->p_ellps == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPOINT_MALLOC_2;
		free(p_gp);
		return NULL;
	}
	memcpy(p_gp->p_ellps,&tab_ellps_ref[ellpstype],1);

	p_gp->phi = (double *) malloc(sizeof(double)*p);
	if (p_gp->phi == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPOINT_MALLOC_3;
		free(p_gp->p_ellps);
		free(p_gp);
		return NULL;
	}
	p_gp->lambda = (double *) malloc(sizeof(double)*p);
	if (p_gp->lambda == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPOINT_MALLOC_4;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp);
		return NULL;
	}
	p_gp->h = (double *) malloc(sizeof(double)*p);
	if (p_gp->h == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPOINT_MALLOC_5;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp);
		return NULL;
	}

	return p_gp;
}

void SG2_topocentric_delete_geopoint(S_SG2_GEOPOINT *p_gp, int *p_err)
{
	free(p_gp->p_ellps);
	free(p_gp->phi);
	free(p_gp->lambda);
	free(p_gp->h);
	free(p_gp);
}

S_SG2_TCOORD *SG2_topocentric_create_tcoord(unsigned long n, unsigned long p, int *p_err)
{
	S_SG2_TCOORD *p_tcoord;
	double *p_tmp1, *p_tmp2, *p_tmp3, *p_tmp4, *p_tmp5;
	unsigned long kp;

	p_tcoord = (S_SG2_TCOORD *) malloc(sizeof(S_SG2_TCOORD));
	if (p_tcoord == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_1;
		return NULL;
	}
	p_tcoord->n = n;
	p_tcoord->p = p;

	/*
	double **r_alpha;
	double **delta;
	double **omega;
	double **gamma_S;
	double **alpha_S;
	*/

	p_tmp1 = (double *) malloc(n*p*sizeof(double));
	if (p_tmp1 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_1;
		free(p_tcoord);
		return NULL;
	}
	p_tcoord->r_alpha = (double **) malloc(p*sizeof(double *));
	if (p_tcoord->r_alpha == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_2;
		free(p_tmp1);
		free(p_tcoord);
		return NULL;
	}
	for (kp = 0; kp < p ; kp++) {
		p_tcoord->r_alpha[kp] = &p_tmp1[n*kp];
	}

	p_tmp2 = (double *) malloc(n*p*sizeof(double));
	if (p_tmp2 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_3;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord);
		return NULL;
	}
	p_tcoord->delta = (double **) malloc(p*sizeof(double *));
	if (p_tcoord->delta == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_4;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tmp2);
		free(p_tcoord);
		return NULL;
	}
	for (kp = 0; kp < p ; kp++) {
		p_tcoord->delta[kp] = &p_tmp2[n*kp];
	}

	p_tmp3 = (double *) malloc(n*p*sizeof(double));
	if (p_tmp3 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_5;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord->delta);
		free(p_tmp2);
		free(p_tcoord);
		return NULL;
	}
	p_tcoord->omega = (double **) malloc(p*sizeof(double *));
	if (p_tcoord->omega == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_6;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord->delta);
		free(p_tmp2);
		free(p_tmp3);
		free(p_tcoord);
		return NULL;
	}
	for (kp = 0; kp < p ; kp++) {
		p_tcoord->omega[kp] = &p_tmp3[n*kp];
	}

	p_tmp4 = (double *) malloc(n*p*sizeof(double));
	if (p_tmp4 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_7;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord->delta);
		free(p_tmp2);
		free(p_tcoord->omega);
		free(p_tmp3);
		free(p_tcoord);
		return NULL;
	}
	p_tcoord->gamma_S = (double **) malloc(p*sizeof(double *));
	if (p_tcoord->gamma_S == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_8;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord->delta);
		free(p_tmp2);
		free(p_tcoord->omega);
		free(p_tmp3);
		free(p_tmp4);
		free(p_tcoord);
		return NULL;
	}
	for (kp = 0; kp < p ; kp++) {
		p_tcoord->gamma_S[kp] = &p_tmp4[n*kp];
	}

	p_tmp5 = (double *) malloc(n*p*sizeof(double));
	if (p_tmp5 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_9;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord->delta);
		free(p_tmp2);
		free(p_tcoord->omega);
		free(p_tmp3);
		free(p_tcoord->gamma_S);
		free(p_tmp4);
		free(p_tcoord);
		return NULL;
	}
	p_tcoord->alpha_S = (double **) malloc(p*sizeof(double *));
	if (p_tcoord->alpha_S == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TCOORD_MALLOC_10;
		free(p_tcoord->r_alpha);
		free(p_tmp1);
		free(p_tcoord->delta);
		free(p_tmp2);
		free(p_tcoord->omega);
		free(p_tmp3);
		free(p_tcoord->gamma_S);
		free(p_tmp4);
		free(p_tmp5);
		free(p_tcoord);
		return NULL;
	}
	for (kp = 0; kp < p ; kp++) {
		p_tcoord->alpha_S[kp] = &p_tmp5[n*kp];
	}

	return p_tcoord;

}

void SG2_topocentric_delete_tcoord(S_SG2_TCOORD *p_tcoord, int *p_err)
{
	free(p_tcoord->r_alpha[0]);
	free(p_tcoord->r_alpha);
	free(p_tcoord->delta[0]);
	free(p_tcoord->delta);
	free(p_tcoord->gamma_S[0]);
	free(p_tcoord->gamma_S);
	free(p_tcoord->alpha_S[0]);
	free(p_tcoord->alpha_S);
	free(p_tcoord);
}
