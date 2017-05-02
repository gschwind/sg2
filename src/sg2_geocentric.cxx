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

#include "sg2_precomputed_heliocentric.h"
#include "sg2_precomputed_geocentric.h"

namespace sg2 {

inline void _geocentric_compute_Delta_psi(double jd_tt, double * Delta_psi, double * epsilon) {
	int idx0;
	idx0 = (int) floor(
			(jd_tt - SG2_precomputed_geocentric_Delta_psi_j0)
					/ SG2_precomputed_geocentric_Delta_psi_dj + 0.5);
	if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_Delta_psi_nj)) {
		*Delta_psi = NAN;
		*epsilon = NAN;
		throw ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
	}
	*Delta_psi = SG2_precomputed_geocentric_Delta_psi[idx0];
	*epsilon = SG2_precomputed_geocentric_epsilon[idx0];
}

inline double _heliocentric_compute_R(double jd_tt)
{
	int idx0;
	double x, x0, dx;
	x = (jd_tt - SG2_precomputed_heliocentric_R_j0)
			/ SG2_precomputed_heliocentric_R_dj;
	x0 = floor(x);
	dx = x - x0;

	idx0 = (short) x0;
	if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_R_nj - 1)) {
		throw ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
	}
	return (1.0 - dx) * SG2_precomputed_heliocentric_R[idx0]
			+ dx * SG2_precomputed_heliocentric_R[idx0 + 1];
}

inline double _heliocentric_compute_L(double jd_tt)
{
	int idx0;
	double x, x0, dx;
	x = (jd_tt - SG2_precomputed_heliocentric_L_j0)
			/ SG2_precomputed_heliocentric_L_dj;
	x0 = floor(x);
	dx = x - x0;

	idx0 = (int) x0;
	if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_L_nj - 1)) {
		throw ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
	}

	return (1.0 - dx) * SG2_precomputed_heliocentric_L[idx0]
			+ dx * SG2_precomputed_heliocentric_L[idx0 + 1];
}

geocentric_data::geocentric_data(time_data const & jd)
{
	short idx0;
	int kd;
	double sin_Theta_a_kd, cos_epsilon_kd;
	double nu0_kd, Delta_psi_cos_epsilon_kd, M_kd;

	R = _heliocentric_compute_R(jd.jd_tt);
	L = _heliocentric_compute_L(jd.jd_tt);

	_geocentric_compute_Delta_psi(jd.jd_tt, &(Delta_psi), &(epsilon));

	Theta_a = L + PI + Delta_psi
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

} // namespace sg2
