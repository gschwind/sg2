/*
 * sg2_topocentric.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_GEOCENTRIC_C_

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

