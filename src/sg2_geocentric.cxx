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

#include "sg2_geocentric.hxx"
#include "sg2_err.hxx"

#include "sg2_precomputed_geocentric.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace sg2 {

inline void _geocentric_compute_Delta_psi(double jd_tt, double * Delta_psi, double * epsilon) {
	int idx0;
	idx0 = (int) floor(
			(jd_tt - SG2_precomputed_geocentric_Delta_psi_j0)
					/ SG2_precomputed_geocentric_Delta_psi_dj + 0.5);
	if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_Delta_psi_nj)) {
		*Delta_psi = NAN;
		*epsilon = NAN;
		throw SG2_ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
	}
	*Delta_psi = SG2_precomputed_geocentric_Delta_psi[idx0];
	*epsilon = SG2_precomputed_geocentric_epsilon[idx0];
}

geocentric_data::geocentric_data(julian_time_data const & jd, heliocentric_data const & helioc)
{
	short idx0;
	int kd;
	double sin_Theta_a_kd, cos_epsilon_kd;
	double nu0_kd, Delta_psi_cos_epsilon_kd, M_kd;

	_geocentric_compute_Delta_psi(jd.jd_tt, &(Delta_psi), &(epsilon));

	Theta_a = helioc.L + PI + Delta_psi
			+ SG2_precomputed_geocentric_Delta_tau;

	sin_Theta_a_kd = sin(Theta_a);
	cos_epsilon_kd = cos(epsilon);

	r_alpha = atan2(sin_Theta_a_kd * cos_epsilon_kd,
			cos(Theta_a));
	delta = asin(sin_Theta_a_kd * sin(epsilon));

	nu0_kd = 6.300388099 * jd.jd_ut - 1.539965571482657e+007;
	Delta_psi_cos_epsilon_kd = Delta_psi * cos_epsilon_kd;
	M_kd = 1.720279169744191e-002 * jd.jd_tt - 4.204914238795757e+004;

	nu = nu0_kd + Delta_psi_cos_epsilon_kd;
	EOT = M_kd - 0.0001 - r_alpha + Delta_psi_cos_epsilon_kd;
}

geocentric_sun_position::geocentric_sun_position(double xjd, double delta_tt) :
	jd{xjd, delta_tt},
	helioc{jd},
	geoc{jd, helioc}
{

}

} // namespace sg2
