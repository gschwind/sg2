/*
 * Copyright 2011 Philippe Blanc <philippe.blanc@mines-paristech.fr>
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

#define SG2_C_

#include "sg2.hxx"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

namespace sg2 {

inline static double _clam(double x) {
	return x - std::floor(x);
}

std::tuple<date, date, date> sunrise(date const & date, geopoint_data const & gp)
{
	// round nearest day a 0 UT
	time_data d0{(date.nsec/86400000000000l)*86400000000000l};

	time_data dp{d0.jd_ut.nsec-86400000000000};
	time_data dn{d0.jd_ut.nsec+86400000000000};

	{
		sg2::ymdhmsn d(d0.jd_ut);
		printf("sun_rise2           = %04d-%02d-%02dT%02d:%02d:%06.3f\n", d.year, d.month, d.day_of_month, d.hour, d.min, d.sec+d.nsec*1e-9);
	}
	{
		sg2::ymdhmsn d(dp.jd_ut);
		printf("sun_rise2           = %04d-%02d-%02dT%02d:%02d:%06.3f\n", d.year, d.month, d.day_of_month, d.hour, d.min, d.sec+d.nsec*1e-9);
	}

	{
		sg2::ymdhmsn d(dn.jd_ut);
		printf("sun_rise2           = %04d-%02d-%02dT%02d:%02d:%06.3f\n", d.year, d.month, d.day_of_month, d.hour, d.min, d.sec+d.nsec*1e-9);
	}


	geocentric_data geoc_dp{dp};
	geocentric_data geoc_d0{d0};
	geocentric_data geoc_dn{dn};

	geoc_d0.nu = (geoc_d0.nu/2.0/PI-std::floor(geoc_d0.nu/2.0/PI))*PI*2.0;

	// Eq A3
	double m0 = (geoc_d0.r_alpha - gp.lambda - geoc_dn.nu)/2.0/PI;

	// Eq A4
	double x0 = (sin(RAD(-0.8333))-sin(gp.phi)*sin(geoc_d0.delta))/(cos(gp.phi)*cos(geoc_d0.delta));
	if (x0 > 1.0 || x0 < -1.0) {
		return {d0.jd_ut, d0.jd_ut, d0.jd_ut};
	}
	double H0 = acos(x0);

	// Eq. A5
	double m1 = m0-H0/2.0/PI;
	double m2 = m0+H0/2.0/PI;

	std::cout << "m0=" << m0 << std::endl;
	std::cout << "m1=" << m1 << std::endl;
	std::cout << "m2=" << m2 << std::endl;

	m0 = _clam(m0);
	m1 = _clam(m1);
	m2 = _clam(m2);

	std::cout << "m0=" << m0 << std::endl;
	std::cout << "m1=" << m1 << std::endl;
	std::cout << "m2=" << m2 << std::endl;

	double v0 = geoc_d0.nu + RAD(360.985647)*m0;
	double v1 = geoc_d0.nu + RAD(360.985647)*m1;
	double v2 = geoc_d0.nu + RAD(360.985647)*m2;

	double n0 = m0 + static_cast<double>(d0.jd_tt.nsec-d0.jd_ut.nsec)/(86400*1e9);
	double n1 = m1 + static_cast<double>(d0.jd_tt.nsec-d0.jd_ut.nsec)/(86400*1e9);
	double n2 = m2 + static_cast<double>(d0.jd_tt.nsec-d0.jd_ut.nsec)/(86400*1e9);

	double a0 = geoc_d0.r_alpha-geoc_dp.r_alpha;
	double a1 = geoc_d0.delta-geoc_dp.delta;
	double b0 = geoc_dn.r_alpha-geoc_d0.r_alpha;
	double b1 = geoc_dn.delta-geoc_d0.delta;
	double c0 = b0 - a0;
	double c1 = b1 - a1;

	double r_alpha_p0 = geoc_d0.r_alpha + n0*(a0+b0+c0*n0)/2.0;
	double delta_p0 = geoc_d0.delta + n0*(a1+b1+c1*n0)/2.0;
	double r_alpha_p1 = geoc_d0.r_alpha + n1*(a0+b0+c0*n1)/2.0;
	double delta_p1 = geoc_d0.delta + n1*(a1+b1+c1*n1)/2.0;
	double r_alpha_p2 = geoc_d0.r_alpha + n2*(a0+b0+c0*n2)/2.0;
	double delta_p2 = geoc_d0.delta + n2*(a1+b1+c1*n2)/2.0;

	double Hp0 = v0 + gp.lambda - r_alpha_p0;
	double Hp1 = v1 + gp.lambda - r_alpha_p1;
	double Hp2 = v2 + gp.lambda - r_alpha_p2;

	Hp0 = ((Hp0+PI)/2.0/PI-std::floor((Hp0+PI)/2.0/PI))*PI*2.0-PI;
	Hp1 = ((Hp1+PI)/2.0/PI-std::floor((Hp1+PI)/2.0/PI))*PI*2.0-PI;
	Hp2 = ((Hp2+PI)/2.0/PI-std::floor((Hp2+PI)/2.0/PI))*PI*2.0-PI;

	double h0 = asin(sin(gp.phi)*sin(delta_p0)+cos(gp.phi)*cos(delta_p0)*cos(Hp0));
	double h1 = asin(sin(gp.phi)*sin(delta_p1)+cos(gp.phi)*cos(delta_p1)*cos(Hp1));
	double h2 = asin(sin(gp.phi)*sin(delta_p2)+cos(gp.phi)*cos(delta_p2)*cos(Hp2));

	double T = m0 - Hp0/2.0/PI;

	double R = m1 + (h1+RAD(0.8333))/(2.0*PI*cos(delta_p1)*cos(gp.phi)*sin(Hp1));
	double S = m2 + (h2+RAD(0.8333))/(2.0*PI*cos(delta_p2)*cos(gp.phi)*sin(Hp2));

	return {d0.jd_ut.nsec+static_cast<int64_t>(R*86400000000000),
			d0.jd_ut.nsec+static_cast<int64_t>(T*86400000000000),
			d0.jd_ut.nsec+static_cast<int64_t>(S*86400000000000)};

}

}
