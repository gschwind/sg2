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

#include "sg2_date.hxx"
#include "sg2_err.hxx"

#include "sg2_precomputed_delta_tt.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace sg2 {

inline int _date_leapyear(int year) {
	return (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));
}


ymdh::ymdh(double jd)
{
	double H, L, N, I, J, K;

	H = (jd + 0.5 - floor(jd + 0.5)) * 24.0;
	L = floor(jd + 0.5) + 68569.0;
	N = floor(4 * L / 146097.0);
	L = L - floor((146097.0 * N + 3.0) / 4.0);
	I = floor(4000 * (L + 1) / 1461001.0);
	L = L - floor(1461.0 * I / 4.0) + 31.0;

	J = floor(80.0 * L / 2447.0);
	K = L - floor(2447.0 * J / 80.0);
	L = floor(J / 11.0);
	J = J + 2.0 - 12.0 * L;
	I = 100.0 * (N - 49.0) + I + L;

	this->year = (short) I;
	this->month = (short) J;
	this->day_of_month = (short) K;
	this->hour = H;
}

ymdh::ymdh(julian_time_data const & jd) :
	ymdh{jd.jd_ut}
{

}

double ymdh::jd() const {
	int k;
	double Y, M, D, H;

	Y = this->year;
	M = this->month;
	D = this->day_of_month;
	H = this->hour;

	if (M < 3) {
		M += 12;
		Y -= 1;
	}

	return 1721028.0 + D + floor((153.0 * M - 2.0) / 5.0) + 365.0 * Y
			+ floor(Y / 4.0) - floor(Y / 100.0) + floor(Y / 400.0) + H / 24.0
			- 0.5;
}

julian_time_data::julian_time_data(double _jd_ut, double _jd_tt) :
	jd_ut{_jd_ut},
	jd_tt{_jd_tt}
{
	if (isnan(jd_tt)) {
		int idx = static_cast<int>(floor(
				(jd_ut - SG2_precomputed_delta_tt_j0)
						/ SG2_precomputed_delta_tt_dj + 0.5));

		if ((idx < 0) || (idx > SG2_precomputed_delta_tt_nj)) {
			throw SG2_ERR_DATE_JD_SET_JD_TT_OUTOFPERIOD;
		}
		jd_tt = jd_ut + SG2_precomputed_delta_tt[idx] / 86400.0;
	}
}

/**
 * NOTE: /!\ do not fill terrestrial time, rebember to call sg2_date_set_time_data_tt afterward.
 **/
julian_time_data::julian_time_data(ymdh const & p_ymdh, double jd_tt) :
	julian_time_data{p_ymdh.jd(), jd_tt}
{

}

ydoyh::ydoyh(ymdh const & p_ymdh)
{
	int k;
	double K;

	K = 2 - _date_leapyear(p_ymdh.year);
	this->year = p_ymdh.year;
	this->hour = p_ymdh.hour;
	this->day_of_year = (short) floor(275.0 * ((double) p_ymdh.month) / 9.0)
			- K * ((double) (p_ymdh.month > 2)) + ((double) p_ymdh.day_of_month)
			- 30.0;
}

ymdh::ymdh(date_ydoyh_t const & p_ydoyh)
{
	double B;
	B = (double) _date_leapyear(p_ydoyh.year);
	this->year = p_ydoyh.year;
	this->hour = p_ydoyh.hour;

	if (p_ydoyh.day_of_year < 32) {
		this->month = 1;
	} else {
		this->month = 1
				+ (short) (floor((303.0 + 5.
				* (((double) p_ydoyh.day_of_year) - 59.0 - B))/ 153.0));
	}

	if (this->month < 2) {
		this->day_of_month = p_ydoyh.day_of_year - 31 * (this->month - 1);
	} else {
		this->day_of_month = p_ydoyh.day_of_year
				- ((short) (floor((153.0 * ((double) this->month) - 2.0) / 5.0)
						- 32.0 + B));
	}
}

}
