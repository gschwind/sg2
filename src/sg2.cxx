/*
 * Copyright 2011-2021 MINES ParisTech
 * Copyright 2011 Philippe Blanc <philippe.blanc@mines-paristech.fr>
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

#define SG2_C_

#include "sg2.hxx"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

namespace sg2 {

// date 'd' Must be close to mean solar time.
std::tuple<date, date, date> sunrise(date const & d, geopoint const & gp)
{
	// the sun rise and sun set when sun go above this limit.
	// SPA is using RAD(-0.8333) with some refracton correction
	// TODO: make it as parameter
	static double const sun_rise_gamma = RAD(0.0);
	static double const sin_sun_rise_gamma = sin(sun_rise_gamma);

	// Round at 12:00 local mst in UT.

	int64_t const delta_mst = (gp.lambda/2.0/PI)*86400000L;
	int64_t mst = d.msec + delta_mst;

	// to round at 12h floor(mst-86400000L/2L+86400000L/2L)+86400000L/2L
	// with floor(T) == T - T%86400000L
	if (mst < 0)
		mst = mst - (86400000L + mst%86400000L) + 86400000L/2L;
	else
		mst = mst - mst%86400000L + 86400000L/2L;

	int64_t const ut_ref = mst - delta_mst;

	date d0{ut_ref}; // D0 is close to MST
	date dp{d0.msec-86400000L};
	date dn{d0.msec+86400000L};

	geocentric_data geoc_d0x{d0};

	if (geoc_d0x.tt.isnat()) {
		return {sg2::nat, sg2::nat, sg2::nat};
	}

	int64_t const delta_tt = geoc_d0x.tt.msec-geoc_d0x.ut.msec;

	geocentric_data geoc_dp{dp, dp};

	if (geoc_dp.tt.isnat()) {
		return {sg2::nat, sg2::nat, sg2::nat};
	}

	geocentric_data geoc_d0{d0, d0};

	if (geoc_d0.tt.isnat()) {
		return {sg2::nat, sg2::nat, sg2::nat};
	}

	geocentric_data geoc_dn{dn, dn};

	if (geoc_dn.tt.isnat()) {
		return {sg2::nat, sg2::nat, sg2::nat};
	}

	// Eq A3
	// CLAMP here because geoc_d0.r_alpha and geoc_d0x.nu may have 2*PI
	// extrat differences
	// m0 relatif to 12h MST, m0 offset match d0
	// Should be close to 0

	// r_alpha - nu - lambda is the relative angle between sun and gp location in
	// the celestial equator
	// m0 should be close to 0 given geoc_d0x
	double m0 = sg2::CLAMP_PI_PI(geoc_d0.r_alpha - geoc_d0x.nu - gp.lambda)/D_PI;

	// Eq A4
	// Safe way to deal with divide by zero.
	// note that in this case if cos(x) is close to 0.0 or equal to zero
	// sin(x) will be close to 1.0 meaning we do not have to check for
	// 0 divide by 0, we are sure the nominator will be far from zero
	// when denominator will be close to 0.
	double xnominator = sin_sun_rise_gamma-sin(gp.phi)*sin(geoc_d0.delta);
	double xdenominator = cos(gp.phi)*cos(geoc_d0.delta);
	if (fabs(xnominator)>fabs(xdenominator)) {
		// The sun will never go above sun_rise_gamma.
		return {sg2::nat, sg2::nat, sg2::nat};
	}

	double H0 = acos(xnominator/xdenominator);

	// Eq. A5
	double m1 = m0-H0/D_PI;
	double m2 = m0+H0/D_PI;

	double v0 = geoc_d0x.nu + RAD(360.985647)*m0;
	double v1 = geoc_d0x.nu + RAD(360.985647)*m1;
	double v2 = geoc_d0x.nu + RAD(360.985647)*m2;

	double n0 = m0 + delta_tt/(86400e3);
	double n1 = m1 + delta_tt/(86400e3);
	double n2 = m2 + delta_tt/(86400e3);

	double alpha_a = geoc_d0.r_alpha-geoc_dp.r_alpha;
	alpha_a = sg2::CLAMP_0_2PI(alpha_a);

	double delta_a = geoc_d0.delta-geoc_dp.delta;

	double alpha_b = geoc_dn.r_alpha-geoc_d0.r_alpha;
	alpha_b = sg2::CLAMP_0_2PI(alpha_b);

	double delta_b = geoc_dn.delta-geoc_d0.delta;

	double alpha_c = alpha_b - alpha_a;
	double delta_c = delta_b - delta_a;

	double r_alpha_p0 = geoc_d0.r_alpha + n0*(alpha_a+alpha_b+alpha_c*n0)/2.0;
	double r_alpha_p1 = geoc_d0.r_alpha + n1*(alpha_a+alpha_b+alpha_c*n1)/2.0;
	double r_alpha_p2 = geoc_d0.r_alpha + n2*(alpha_a+alpha_b+alpha_c*n2)/2.0;

	double delta_p1 =   geoc_d0.delta   + n1*(delta_a+delta_b+delta_c*n1)/2.0;
	double delta_p2 =   geoc_d0.delta   + n2*(delta_a+delta_b+delta_c*n2)/2.0;

	double Hp0 = -sg2::CLAMP_PI_PI(r_alpha_p0 - v0 - gp.lambda);
	double Hp1 = -sg2::CLAMP_PI_PI(r_alpha_p1 - v1 - gp.lambda);
	double Hp2 = -sg2::CLAMP_PI_PI(r_alpha_p2 - v2 - gp.lambda);

	// Hp1 must be negative, because the sun rise on morning
	if (Hp1 > 0.0)
		Hp1 -= 2.0*PI;

	// Must be positive, because sun set later
	if (Hp2 < 0.0)
		Hp2 += 2.0*PI;

	double T = m0 - Hp0/D_PI;

	double R;
	double S;

	// Check if sun will rise
	double x1sin = sin(gp.phi)*sin(delta_p1);
	double x1cos = cos(gp.phi)*cos(delta_p1);
	if (fabs(sin_sun_rise_gamma-x1sin)>fabs(x1cos)) {
		// The sun will never rise today.
		R = std::numeric_limits<double>::quiet_NaN();
	} else {
		double h1 = asin(x1sin+x1cos*cos(Hp1));
		// Must be negative because it's the morning.
		double xH1 = -acos((sin_sun_rise_gamma-x1sin)/x1cos);
		R = m1 + (xH1 - Hp1) / (D_PI);
	}

	// Check if sun will set
	double x2sin = sin(gp.phi)*sin(delta_p2);
	double x2cos = cos(gp.phi)*cos(delta_p2);
	if (fabs(sin_sun_rise_gamma-x2sin)>fabs(x2cos)) {
		// The sun will never set today.
		S = std::numeric_limits<double>::quiet_NaN();
	} else {
		double h2 = asin(x2sin+x2cos*cos(Hp2));
		double xH2 = acos((sin_sun_rise_gamma-x2sin)/x2cos);
		S = m2 + (xH2 - Hp2) / (D_PI);
	}

	return {std::isfinite(R)?date{d0.msec+static_cast<int64_t>(R*86400e3)}:sg2::nat,
			date{d0.msec+static_cast<int64_t>(T*86400e3)},
			std::isfinite(S)?date{d0.msec+static_cast<int64_t>(S*86400e3)}:sg2::nat};

}

}
