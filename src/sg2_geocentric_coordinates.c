/*
 * sg2_geocentric_coordinates.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_HELIOCENTRIC_COORDINATES_C_

#include "sg2.h"
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

	return p_gcoord;
}

void SG2_geocentric_coordinates_delete_gcoord(S_SG2_GCOORD *p_gcoord,
		int *p_err) {
	free(p_gcoord->epsilon);
	free(p_gcoord->Theta_a);
	free(p_gcoord->r_alpha);
	free(p_gcoord->delta);
	free(p_gcoord);
}

