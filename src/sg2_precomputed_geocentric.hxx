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

#ifndef SG2_PRECOMPUTED_GEOCENTRIC_H_
#define SG2_PRECOMPUTED_GEOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

static const double SG2_precomputed_geocentric_epsilon_j0 = 2.4442095000000000e+006;
static const double SG2_precomputed_geocentric_epsilon_dj = 3.0000000000000000e+001;
static const unsigned long SG2_precomputed_geocentric_epsilon_nj = 623;
static const double SG2_precomputed_geocentric_Delta_psi_j0 = 2.4442095000000000e+006;
static const double SG2_precomputed_geocentric_Delta_psi_dj = 3.0000000000000000e+001;
static const unsigned long SG2_precomputed_geocentric_Delta_psi_nj = 623;

/*
SG2_precomputed_geocentric_epsilon
   True Earth Obliquity (radian)
SG2_precomputed_geocentric_Delta_psi
   Nutation in Geocentric Sun longitude (radian)
*/

static const double SG2_precomputed_geocentric_Delta_tau = -9.9337353631981704e-005;

extern double const SG2_precomputed_geocentric_epsilon[623];
extern double const SG2_precomputed_geocentric_Delta_psi[623];

#ifdef	__cplusplus
}
#endif

#endif /* SG2_PRECOMPUTED_GEOCENTRIC_H_ */

