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

/** 1970-01-01T12:00 **/
static double const EPOCH_JD = 2440587.5;

inline int _date_leapyear(int year) {
	return (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));
}

/* return the julian day at 12h */
inline int _ymd_to_julian_day(int year, int month, int day)
{
	int k;
	double Y, M, D, H;

	Y = year;
	M = month;
	D = day;
	if (M < 3) {
		M += 12;
		Y -= 1;
	}

	return 1721028.0 + D + floor((153.0 * M - 2.0) / 5.0) + 365.0 * Y
			+ floor(Y / 4.0) - floor(Y / 100.0) + floor(Y / 400.0);

}

inline void _julian_day_to_ymd(int jd, int & year, int & month, int & day)
{
	double H, L, N, I, J, K;

	L = jd + 68569.0;
	N = floor(4 * L / 146097.0);
	L = L - floor((146097.0 * N + 3.0) / 4.0);
	I = floor(4000 * (L + 1) / 1461001.0);
	L = L - floor(1461.0 * I / 4.0) + 31.0;

	J = floor(80.0 * L / 2447.0);
	K = L - floor(2447.0 * J / 80.0);
	L = floor(J / 11.0);
	J = J + 2.0 - 12.0 * L;
	I = 100.0 * (N - 49.0) + I + L;

	year = I;
	month = J;
	day = K;

}

ymdh::ymdh(short year, short month, char day_of_month, double hour) :
	year{year},
	month{month},
	day_of_month{day_of_month},
	hour{hour}
{

}

ymdh::ymdh(double jd)
{
	_julian_day_to_ymd(jd, year, month, day_of_month);
	hour = (jd + 0.5 - floor(jd + 0.5)) * 24.0;
}

ymdh::ymdh(julian_time_data const & jd) :
	ymdh{jd.jd_ut}
{

}

ymdh::ymdh(date const & d)
{
	int jd = d.nsec / (1e9 * 60.0 * 60.0 * 24.0) + EPOCH_JD + 0.5;
	_julian_day_to_ymd(jd, year, month, day_of_month);
	hour = static_cast<double>(d.nsec % 1000000000LL)/(60.0*60.0*1e9);
}

ymdh::ymdh(ydoyh const & p_ydoyh)
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

ymdhmsn::ymdhmsn(date const date)
{
	int64_t xnsec = date.nsec;

	// Find the nearest integer jd.
	int jd = xnsec / (1e9 * 60.0 * 60.0 * 24.0) + EPOCH_JD + 0.5;
	_julian_day_to_ymd(jd, year, month, day_of_month);

	/* in this order we avoid a lot of modulo */
	xnsec %= (1000000000LL * 60LL * 60LL * 24LL);
	hour = xnsec / (1000000000LL * 60LL * 60LL);
	xnsec -= hour * (1000000000LL * 60LL * 60LL);
	min = xnsec / (1000000000LL * 60LL);
	xnsec -= min * (1000000000LL * 60LL);
	sec = xnsec / (1000000000LL);
	xnsec -= sec * (1000000000LL);
	nsec = (int)xnsec;

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
			throw ERR_DATE_JD_SET_JD_TT_OUTOFPERIOD;
		}
		jd_tt = jd_ut + SG2_precomputed_delta_tt[idx] / 86400.0;
	}
}

/**
 * NOTE: /!\ do not fill terrestrial time, rebember to call sg2_date_set_time_data_tt afterward.
 **/
julian_time_data::julian_time_data(ymdh const & p_ymdh, double jd_tt) :
	julian_time_data{julian(p_ymdh), jd_tt}
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

date::date(int64_t nsec) :
	nsec{nsec}
{

}

date::date(double jd) :
	nsec((jd-EPOCH_JD)*24.0*60.0*60.0*1e9)
{

}

date::date(ymdh const & d)
{
	int64_t xjd = _ymd_to_julian_day(d.year, d.month, d.day_of_month);
	nsec = (xjd-EPOCH_JD)*(24LL*60LL*60LL*1000000000LL) + d.hour*60.0*60.0*1e9;
}

date::date(ydoyh const & d) :
	date{ymdh{d}}
{

}

julian::julian(double jd) : jd{jd} { }

julian::julian(ymdh const & d)
{
	int64_t xjd = _ymd_to_julian_day(d.year, d.month, d.day_of_month);
	jd = xjd - 0.5 + d.hour / 24.0;
}

julian::julian(ydoyh const & d) :
	julian{ymdh{d}}
{

}

julian::julian(date const nsec) :
	jd{nsec/(24.0*60.0*60.0*1e9)+EPOCH_JD}
{
}

}
