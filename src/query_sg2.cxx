/*
 * Copyright 2014-2021 MINES ParisTech
 * Copyright 2014-2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#include <sg2.hxx>
#include <cstdio>
#include <cstdlib>

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

	sg2::julian xjd{jd};

	/** location related data **/
	sg2::geopoint geopoint{lon, lat, alt, sg2::ELLPS_WGS84};

	/** time related data **/
	sg2::geocentric_data geoc{sg2::date{xjd.value}};

	/** local-time related data **/
	sg2::topocentric_data topoc{geoc, geopoint};

	/**
	 * Computing solar system state.
	 **/

	auto sunrise = sg2::sunrise(geoc.ut, geopoint);

	printf("Time related data (i.e. solar system geometry)\n");
	printf("geoc.ut            = %s\n", static_cast<std::string>(geoc.ut).c_str());
	printf("geoc.tt            = %s\n", static_cast<std::string>(geoc.tt).c_str());

	printf("geoc.R             = %f\n", geoc.R);
	printf("geoc.L             = %f\n", geoc.L);

	printf("geoc.delta         = %f\n", geoc.delta);
	printf("geoc.EOT           = %f\n", geoc.EOT);
	printf("geoc.Theta_a       = %f\n", geoc.Theta_a);
	printf("geoc.epsilon       = %f\n", geoc.epsilon);
	printf("geoc.nu            = %f\n", geoc.nu);
	printf("geoc.r_alpha       = %f\n", geoc.r_alpha);
	printf("geoc.Delta_psi     = %f\n", geoc.Delta_psi);

	printf("Location related data\n");

	printf("geopoint.lambda   = %f\n", geopoint.lambda);
	printf("geopoint.phi      = %f\n", geopoint.phi);
	printf("geopoint.u        = %f\n", geopoint.u);
	printf("geopoint.x        = %f\n", geopoint.x);
	printf("geopoint.y        = %f\n", geopoint.y);

	printf("Sun related data\n");

	printf("topoc.delta       = %f\n", topoc.delta);
	printf("topoc.alpha_S     = %f\n", topoc.alpha_S);
	printf("topoc.gamma_S0    = %f\n", topoc.gamma_S0);
	printf("topoc.omega       = %f\n", topoc.omega);
	printf("topoc.r_alpha     = %f\n", topoc.r_alpha);
	printf("topoc.toa_hi      = %f\n", topoc.toa_hi);
	printf("topoc.toa_ni      = %f\n", topoc.toa_ni);

	printf("Extra data\n");
	double x = (xjd.value + (lon/360.0) - (topoc.omega/(M_PI*2.0)));
	printf("tst-tu            = %f\n", (x-floor(x+0.5)));

	printf("sun_rise          = %s\n", static_cast<std::string>(std::get<0>(sunrise)).c_str());
	printf("sun_transite      = %s\n", static_cast<std::string>(std::get<1>(sunrise)).c_str());
	printf("sun_set           = %s\n", static_cast<std::string>(std::get<2>(sunrise)).c_str());

	return 0;

}


