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

#ifdef  __cplusplus
extern "C"
{
#endif

static const double SG2_precomputed_heliocentric_R_j0 = 2.4442095000000000e+006;
static const double SG2_precomputed_heliocentric_R_dj = 1.0000000000000000e+000;
static const unsigned long SG2_precomputed_heliocentric_R_nj = 18689;
static const double SG2_precomputed_heliocentric_L_j0 = 2.4442095000000000e+006;
static const double SG2_precomputed_heliocentric_L_dj = 1.0000000000000000e+000;
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

