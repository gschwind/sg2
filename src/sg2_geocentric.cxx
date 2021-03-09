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
#include "sg2_precomputed_geocentric.hxx"
#include "sg2_precomputed_heliocentric.hxx"

namespace sg2 {

static void _geocentric_compute_Delta_psi(int64_t jd_tt, double * Delta_psi, double * epsilon) {
	int64_t idx0 = (jd_tt - SG2_precomputed_geocentric_Delta_psi_j0
			+ (SG2_precomputed_geocentric_Delta_psi_dj/2))
			/ SG2_precomputed_geocentric_Delta_psi_dj;
	if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_Delta_psi_nj)) {
		*Delta_psi = NAN;
		*epsilon = NAN;
		throw ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
	}
	*Delta_psi = SG2_precomputed_geocentric_Delta_psi[idx0];
	*epsilon = SG2_precomputed_geocentric_epsilon[idx0];
}

static double _heliocentric_compute_R(int64_t jd_tt)
{
	int64_t x = (jd_tt - SG2_precomputed_heliocentric_R_j0)
			/ SG2_precomputed_heliocentric_R_dj;
	int64_t dx = (jd_tt - SG2_precomputed_heliocentric_R_j0)
					% SG2_precomputed_heliocentric_R_dj;

	if ((x < 0) || (x > SG2_precomputed_heliocentric_R_nj - 1)) {
		throw ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
	}

	double alpha = static_cast<double>(dx)
			/static_cast<double>(SG2_precomputed_heliocentric_R_dj);
	return std::fma(alpha,
			(SG2_precomputed_heliocentric_R[x+1]-SG2_precomputed_heliocentric_R[x]),
			SG2_precomputed_heliocentric_R[x]);
}

static double _heliocentric_compute_L(int64_t jd_tt)
{
	int64_t x = (jd_tt - SG2_precomputed_heliocentric_L_j0)
			/ SG2_precomputed_heliocentric_L_dj;
	int64_t dx = (jd_tt - SG2_precomputed_heliocentric_L_j0)
					% SG2_precomputed_heliocentric_L_dj;

	if ((x < 0) || (x > SG2_precomputed_heliocentric_L_nj - 1)) {
		throw ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
	}

	double alpha = static_cast<double>(dx)
			/static_cast<double>(SG2_precomputed_heliocentric_L_dj);
	return std::fma(alpha,
			(SG2_precomputed_heliocentric_L[x+1]-SG2_precomputed_heliocentric_L[x]),
			SG2_precomputed_heliocentric_L[x]);
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

	sin_Theta_a_kd = math::sin(Theta_a);
	cos_epsilon_kd = math::cos(epsilon);

	r_alpha = math::atan2(sin_Theta_a_kd * cos_epsilon_kd,
			math::cos(Theta_a));
	delta = math::asin(sin_Theta_a_kd * math::sin(epsilon));

	// TODO: convert to nano second
	nu0_kd = 6.300388099 * julian{date{jd.jd_ut}}.jd - 1.539965571482657e+007;
	Delta_psi_cos_epsilon_kd = Delta_psi * cos_epsilon_kd;
	M_kd = 1.720279169744191e-002 * julian{date{jd.jd_tt}}.jd - 4.204914238795757e+004;

	nu = nu0_kd + Delta_psi_cos_epsilon_kd;
	EOT = M_kd - 0.0001 - r_alpha + Delta_psi_cos_epsilon_kd;
}

} // namespace sg2
