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

struct date {
	int64_t nsec; // nanosecond since 1970

	date(int64_t nsec); // TODO
	date(double jd); // TODO
	date(ymdh const & d); // TODO
	date(ydoyh const & d); // TODO

	operator int64_t() const
	{
		return nsec;
	}

};

struct julian {
	double jd; // julian date in factionnal days

	julian(double jd);
	julian(ymdh const & d); // TODO
	julian(ydoyh const & d); // TODO
	julian(date nsec); // TODO

	operator double() const
	{
		return jd;
	}

};

/* Date YMD + H en heure décimale UT */
struct ymdh {
	int year;
	int month;
	int day_of_month;
	double hour;

	ymdh(short year, short month, char day_of_month, double hour);
	ymdh(julian_time_data const & jd);
	ymdh(double jd);
	ymdh(date const & d);
	ymdh(ydoyh const & ydoyh);

	double jd() const;

};

struct ymdhmsn {
	int year;
	int month;
	int day_of_month;
	int hour;
	int min;
	int sec;
	int nsec;

	ymdhmsn(date const date);

};

struct ydoyh {
	short year;
	short day_of_year;
	double hour;

	ydoyh(ymdh const & date);

};

struct julian_time_data {
	double jd_ut;            /* julian date UT (decimal day) */
	double jd_tt;            /* TT : terrestrial time */

	julian_time_data(double jd_ut, double jd_tt = NAN);
	julian_time_data(ymdh const & date, double jd_tt = NAN);

};

}

#endif /* SG2_DATE_HXX_ */
