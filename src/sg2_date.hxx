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

#ifndef SG2_DATE_HXX_
#define SG2_DATE_HXX_

#include "sg2_typedef.hxx"
#include <cstdint>

namespace sg2 {

/* Date YMD + H en heure décimale UT */
struct ymdh {
	short year;
	char month;
	char day_of_month;
	double hour;

	ymdh(short year, short month, char day_of_month, double hour);
	ymdh(julian_time_data const & jd);
	ymdh(ydoyh const & date);
	ymdh(double jd);

	double jd() const;

};

/* TODO: remove */
using date_ymdh_t = ymdh;

struct ydoyh {
	short year;
	short day_of_year;
	double hour;

	ydoyh(ymdh const & date);

};

using date_ydoyh_t = ydoyh;

struct julian_time_data {
	double jd_ut;            /* julian date UT (decimal day) */
	double jd_tt;            /* TT : terrestrial time */

	julian_time_data(double jd_ut, double jd_tt = NAN);
	julian_time_data(ymdh const & date, double jd_tt = NAN);

};

using time_data_t = julian_time_data;

}

#endif /* SG2_DATE_HXX_ */
