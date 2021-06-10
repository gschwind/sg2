/* -*- coding: utf-8 -*-
 *
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

#ifndef SG2_TYPEDEF_HXX_
#define SG2_TYPEDEF_HXX_

#include <limits>

#include <cmath>

namespace sg2 {

using namespace std;

static constexpr double const PI = 3.1415926535897931;
static constexpr double const D_PI = 6.2831853071795862;

constexpr inline double DEG(double RAD) {
	return RAD*180.0/PI;
}

constexpr inline double RAD(double DEG) {
	return DEG*PI/180.0;
}

/* Top of atmosphere broadband irradiance on normal incidence (W/m2) */
static constexpr double const SOLAR_CONSTANT = 1362.0;
/* Astronomical unit : annual average of the Sun-Earth distance (m) */
static constexpr double const AU = 149597870691.0; /* +/- 6 m (McCarthy et Petit, 2003) */

inline double CLAMP_0_2PI(double angle) { return ((angle)-std::floor((angle)/(D_PI))*(D_PI)); }
inline double CLAMP_PI_PI(double angle) { return ((angle)-std::round((angle)/(D_PI))*(D_PI)); }

struct date;
struct julian;
struct ymdh;
struct ydoyh;
struct ellps;

struct geopoint;
struct geocentric_data;
struct topocentric_data;

} // namespace sg2

#endif /* SG2_TYPEDEF_HXX_ */
