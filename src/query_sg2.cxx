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

#include <sg2_sun_daily.hxx>
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

	sg2::time_data xjd{jd};

	/** location related data **/
	sg2::geopoint_data geopoint{lon, lat, alt, sg2::ELLPS_WGS84};

	/** time related data **/
	sg2::geocentric_data geoc{xjd};

	/** local-time related data **/
	sg2::topocentric_data topoc{geoc, geopoint};

	sg2::sun_daily_data day{geopoint};
	try {
		day.update(floor(jd+0.5));
	} catch (...) {
		printf("fail to compute daily value\n");
	}

	/**
	 * Computing solar system state.
	 **/

	sg2::ymdhmsn date_ut(xjd.jd_ut);
	sg2::ymdhmsn date_tt(xjd.jd_tt);

	printf("Time related data (i.e. solar system geometry)\n");
	printf("jd.jd_ut           = %04d-%02d-%02dT%02d:%02d:%06.3f\n", date_ut.year, date_ut.month, date_ut.day_of_month, date_ut.hour, date_ut.min, date_ut.sec+date_ut.nsec*1e-9);
	printf("jd.jd_tt           = %04d-%02d-%02dT%02d:%02d:%06.3f\n", date_tt.year, date_tt.month, date_tt.day_of_month, date_tt.hour, date_tt.min, date_tt.sec+date_tt.nsec*1e-9);

	printf("helioc.R           = %f\n", geoc.R);
	printf("helioc.L           = %f\n", geoc.L);

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
	printf("geopoint.cos_phi  = %f\n", std::cos(geopoint.phi));
	printf("geopoint.sin_phi  = %f\n", std::sin(geopoint.phi));

	printf("Sun related data\n");

	printf("topoc.delta       = %f\n", topoc.delta);
	printf("topoc.alpha_S     = %f\n", topoc.alpha_S);
	printf("topoc.gamma_S0    = %f\n", topoc.gamma_S0);
	printf("topoc.omega       = %f\n", topoc.omega);
	printf("topoc.r_alpha     = %f\n", topoc.r_alpha);
	printf("topoc.toa_hi      = %f\n", topoc.toa_hi);
	printf("topoc.toa_ni      = %f\n", topoc.toa_ni);

	printf("Extra data\n");
	double x = (xjd.jd_ut + (lon/360.0) - (topoc.omega/(M_PI*2.0)));
	printf("tst-tu            = %f\n", (x-floor(x+0.5)));

	printf("sun_rise          = %f\n", day.get_sun_rise_time());
	printf("sun_set           = %f\n", day.get_sun_set_time());
	printf("sun_zenit         = %f\n", day.get_sun_zenit_time());
	printf("sun_begin_of_day  = %f\n", day.get_sun_begin_of_day_time());
	printf("sun_end_of_day    = %f\n", day.get_sun_end_of_day_time());

	return 0;

}


