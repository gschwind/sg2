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

#ifndef SG2_HELIOCENTRIC_HXX_
#define SG2_HELIOCENTRIC_HXX_

#include "sg2_typedef.hxx"

namespace sg2 {

/* Heliocentric coordinates */
struct heliocentric_data {
	double R; /* Radius Sun-Earth (ua) */
	double L; /* Heliocentric Earth true longitude (rad) */

	heliocentric_data(julian_time_data const & jd);

};

} // namespace sg2

#endif /* SG2_HELIOCENTRIC_HXX_ */
