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
#include "sg2_data_handler.hxx"
#include "sg2_utils.hxx"

#include <tuple>

namespace sg2 {

static std::tuple<double, double> _geocentric_compute_Delta_psi_and_epsilon(int64_t jd_tt)
{
	int64_t idx0 = (jd_tt - SG2_precomputed_geocentric_Delta_psi_j0
			+ (SG2_precomputed_geocentric_Delta_psi_dj/2))
			/ SG2_precomputed_geocentric_Delta_psi_dj;
	if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_Delta_psi_nj)) {
		throw ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
	}
	double Delta_psi = SG2_precomputed_geocentric_Delta_psi[idx0];
	double epsilon = SG2_precomputed_geocentric_epsilon[idx0];
	return std::make_tuple(Delta_psi, epsilon);
}

static std::tuple<double, double> _heliocentric_compute_R_and_L(int64_t jd_tt)
{
	int64_t x = (jd_tt/1000000000 - _geocentric_data_offset())
			/ _geocentric_data_delta();
	int64_t dx = (jd_tt/1000000000 - _geocentric_data_offset())
					% _geocentric_data_delta();

	if ((x < 0) || (x > _geocentric_data_count() - 1)) {
		throw ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
	}

	double alpha = static_cast<double>(dx)
			/static_cast<double>(_geocentric_data_delta());

	double sinL = std::fma(alpha,(_geocentric_data_sinL(x+1)-_geocentric_data_sinL(x)),_geocentric_data_sinL(x));
	double cosL = std::fma(alpha,(_geocentric_data_cosL(x+1)-_geocentric_data_cosL(x)),_geocentric_data_cosL(x));
	double R    = std::fma(alpha,(_geocentric_data_R   (x+1)-_geocentric_data_R   (x)),_geocentric_data_R   (x));

	return std::make_tuple(R, math::atan2(sinL, cosL));
}


geocentric_data::geocentric_data(time_data const & jd)
{
	short idx0;
	int kd;
	double sin_Theta_a_kd, cos_epsilon_kd;
	double nu0_kd, Delta_psi_cos_epsilon_kd, M_kd;

	std::tie(R, L) = _heliocentric_compute_R_and_L(jd.jd_tt);

	Delta_psi = approx_Dpsi.compute(julian{date{jd.jd_tt}}.jd);
	epsilon = approx_epsilon.compute(julian{date{jd.jd_tt}}.jd);

	Theta_a = L + PI + Delta_psi
			+ SG2_precomputed_geocentric_Delta_tau;

	sin_Theta_a_kd = math::sin(Theta_a);
	cos_epsilon_kd = math::cos(epsilon);

	r_alpha = math::atan2(sin_Theta_a_kd * cos_epsilon_kd,
			math::cos(Theta_a));
	delta = math::asin(sin_Theta_a_kd * math::sin(epsilon));

	// TODO: convert to nano second
	nu0_kd = approx_nu0.compute(julian{date{jd.jd_ut}}.jd);

	Delta_psi_cos_epsilon_kd = Delta_psi * cos_epsilon_kd;
	M_kd = 1.720279169744191e-002 * julian{date{jd.jd_tt}}.jd - 4.204914238795757e+004;

	nu = nu0_kd + Delta_psi_cos_epsilon_kd;
	EOT = M_kd - 0.0001 - r_alpha + Delta_psi_cos_epsilon_kd;
}

} // namespace sg2
