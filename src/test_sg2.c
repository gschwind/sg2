/*
 * Copyright 2014 MINES ParisTech
 * Copyright 2014 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
 *
 * This file is part of libsg2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include "sg2.h"


void usage(void) {
	printf("usage: test_sg2 <lat> <lon> <alt> <jd>\n");
}

int main(int argc, char ** argv) {

	if(argc != 5) {
		usage();
		return -1;
	}

	double lat = atof(argv[1]);
	double lon = atof(argv[2]);
	double alt = atof(argv[3]);
	double jd = atof(argv[4]);

	int err = 0;

	/** time related data **/
	S_SG2_DATE_TABJD * xjd = SG2_date_create_tabjd(1, &err);
	S_SG2_HELIOC_DATA * xhelioc = SG2_heliocentric_create_helioc_data(1, &err);
	S_SG2_GEOC_DATA * xgeoc = SG2_geocentric_create_geoc_data(1, &err);

	/** location related data **/
	S_SG2_TABGEOPOINT * xgeopoint = SG2_topocentric_create_tabgeopoint(1, SG2_ELLPSTYPE_WGS84, &err);

	/** local-time related data **/
	S_SG2_TOPOC_DATA * xtopoc = SG2_topocentric_create_topoc_data(1, 1, &err);


	/**
	 * Computing solar system state.
	 **/

	/* TODO: beter interfaces */
	xjd->jd_ut[0] = jd;
	SG2_date_set_tabjd_tt(NULL, xjd, &err);
	if(err != 0) {
		printf("error while setting julian date\n");
		return err;
	}

	SG2_heliocentric_set_helioc_data(xjd, xhelioc, &err);
	if(err != 0) {
		printf("error while computing heliocentric data\n");
		return err;
	}

	SG2_geocentric_set_geoc_data(xjd, xhelioc, xgeoc, &err);
	if (err != 0) {
		printf("error while computing geocentric data\n");
		return err;
	}

	/**
	 * Computing local geometry data.
	 **/
	SG2_topocecentric_set_tabgeopoint(&lon, &lat, &alt, xgeopoint, &err);
	if(err != 0) {
		printf("error while setting geopoint\n");
		return err;
	}

	/**
	 * computing sun position at selected location.
	 **/
	SG2_topocentric_set_topoc_data(xgeoc, xgeopoint, xtopoc, &err);
	if(err != 0) {
		printf("error while cumputing topocentric data\n");
		return err;
	}

	printf("Time related data (i.e. solar system geometry)\n");
	printf("jd.jd_ut           = %f\n", xjd->jd_ut[0]);
	printf("jd.jd_tt           = %f\n", xjd->jd_tt[0]);

	printf("helioc.R           = %f\n", xhelioc->R[0]);
	printf("helioc.L           = %f\n", xhelioc->L[0]);

	printf("geoc.delta         = %f\n", xgeoc->delta[0]);
	printf("geoc.EOT           = %f\n", xgeoc->EOT[0]);
	printf("geoc.Theta_a       = %f\n", xgeoc->Theta_a[0]);
	printf("geoc.epsilon       = %f\n", xgeoc->epsilon[0]);
	printf("geoc.nu            = %f\n", xgeoc->nu[0]);
	printf("geoc.r_alpha       = %f\n", xgeoc->r_alpha[0]);
	printf("geoc.Delta_psi     = %f\n", xgeoc->Delta_psi[0]);

	printf("Location related data\n");

	printf("geopoint.lambda   = %f\n", xgeopoint->lambda[0]);
	printf("geopoint.phi      = %f\n", xgeopoint->phi[0]);
	printf("geopoint.u        = %f\n", xgeopoint->u[0]);
	printf("geopoint.x        = %f\n", xgeopoint->x[0]);
	printf("geopoint.y        = %f\n", xgeopoint->y[0]);
	printf("geopoint.cos_phi  = %f\n", xgeopoint->cos_phi[0]);
	printf("geopoint.sin_phi  = %f\n", xgeopoint->sin_phi[0]);

	printf("Sun related data\n");

	printf("topoc.delta       = %f\n", xtopoc->delta[0][0]);
	printf("topoc.alpha_S     = %f\n", xtopoc->alpha_S[0][0]);
	printf("topoc.gamma_S0    = %f\n", xtopoc->gamma_S0[0][0]);
	printf("topoc.omega       = %f\n", xtopoc->omega[0][0]);
	printf("topoc.r_alpha     = %f\n", xtopoc->r_alpha[0][0]);
	printf("topoc.toa_hi      = %f\n", xtopoc->toa_hi[0][0]);
	printf("topoc.toa_ni      = %f\n", xtopoc->toa_ni[0][0]);

	/** TODO: clean exit on error **/

	return 0;

}


