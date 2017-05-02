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

#ifndef SG2_H_
#define SG2_H_

#include "sg2_typedef.hxx"
#include "sg2_date.hxx"
#include "sg2_geocentric.hxx"
#include "sg2_topocentric.hxx"
#include "sg2_sun_daily.hxx"

namespace sg2 {

struct geopoint_snap_data {
	time_data time;
	geopoint_data geopoint;
	geocentric_data geoc;
	topocentric_data topoc;
};

}

#endif /* SG2_H_ */
