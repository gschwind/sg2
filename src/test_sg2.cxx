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

#include <cstdio>
#include <cstdlib>

#include "sg2xx.hxx"


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

	/** time related data **/
	sg2::geocentric_sun_position sun_position{jd};

	/** location related data **/
	sg2::geopoint geopoint{lon, lat, alt};

	/** local-time related data **/
	sg2::topocentric_data topoc{geoc, helioc, geopoint};

	/**
	 * Computing solar system state.
	 **/

	printf("Time related data (i.e. solar system geometry)\n");
	printf("jd.jd_ut           = %f\n", sun_position.jd.jd_ut);
	printf("jd.jd_tt           = %f\n", sun_position.jd.jd_tt);

	printf("helioc.R           = %f\n", sun_position.helioc.R);
	printf("helioc.L           = %f\n", sun_position.helioc.L);

	printf("geoc.delta         = %f\n", sun_position.geoc.delta);
	printf("geoc.EOT           = %f\n", sun_position.geoc.EOT);
	printf("geoc.Theta_a       = %f\n", sun_position.geoc.Theta_a);
	printf("geoc.epsilon       = %f\n", sun_position.geoc.epsilon);
	printf("geoc.nu            = %f\n", sun_position.geoc.nu);
	printf("geoc.r_alpha       = %f\n", sun_position.geoc.r_alpha);
	printf("geoc.Delta_psi     = %f\n", sun_position.geoc.Delta_psi);

	printf("Location related data\n");

	printf("geopoint.lambda   = %f\n", geopoint.lambda);
	printf("geopoint.phi      = %f\n", geopoint.phi);
	printf("geopoint.u        = %f\n", geopoint.u);
	printf("geopoint.x        = %f\n", geopoint.x);
	printf("geopoint.y        = %f\n", geopoint.y);
	printf("geopoint.cos_phi  = %f\n", geopoint.cos_phi);
	printf("geopoint.sin_phi  = %f\n", geopoint.sin_phi);

	printf("Sun related data\n");

	printf("topoc.delta       = %f\n", topoc.delta);
	printf("topoc.alpha_S     = %f\n", topoc.alpha_S);
	printf("topoc.gamma_S0    = %f\n", topoc.gamma_S0);
	printf("topoc.omega       = %f\n", topoc.omega);
	printf("topoc.r_alpha     = %f\n", topoc.r_alpha);
	printf("topoc.toa_hi      = %f\n", topoc.toa_hi);
	printf("topoc.toa_ni      = %f\n", topoc.toa_ni);

	printf("Extra data\n");
	double x = (sun_position.jd.jd_ut + (lon/360.0) - (topoc.omega/(M_PI*2.0)));
	printf("tst-tu            = %f\n", (x-floor(x+0.5)));

	return 0;

}


