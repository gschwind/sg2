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

#ifndef SG2_PRECOMPUTED_HELIOCENTRIC_H_
#define SG2_PRECOMPUTED_HELIOCENTRIC_H_

#include <cstdint>

#ifdef  __cplusplus
extern "C"
{
#endif

// offset = 1979-12-02T00:00
static const int64_t
SG2_precomputed_heliocentric_R_j0 = 312940800000000000;  //< nano second since 1970-01-01T00:00
static const int64_t
SG2_precomputed_heliocentric_R_dj =     86400000000000; //< 1 day in nano second
static const unsigned long SG2_precomputed_heliocentric_R_nj = 18689;

// offset = 1979-12-02T00:00
static const int64_t
SG2_precomputed_heliocentric_L_j0 = 312940800000000000;  //< nano second since 1970-01-01T00:00
static const int64_t
SG2_precomputed_heliocentric_L_dj =     86400000000000; //< 1 day in nano second
static const unsigned long SG2_precomputed_heliocentric_L_nj = 18689;

/*
SG2_precomputed_heliocentric_R
   Heliocentric radius (ua, 1 ua = 149597870691 +/- 6 m)
SG2_precomputed_heliocentric_L
   Heliocentric longitude (radian) */

extern double const SG2_precomputed_heliocentric_R[18689];
extern double const SG2_precomputed_heliocentric_L[18689];

#ifdef	__cplusplus
}
#endif

#endif /* SG2_PRECOMPUTED_HELIOCENTRIC_H_ */

