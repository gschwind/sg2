/*
 * Copyright 2011-2021 MINES ParisTech
 * Copyright 2011 Philippe Blanc <philippe.blanc@mines-paristech.fr>
 * Copyright 2014-2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#ifndef SG2_TOPOCENTRIC_HXX_
#define SG2_TOPOCENTRIC_HXX_

#include "sg2_typedef.hxx"
#include "sg2_geocentric.hxx"
#include "sg2_geopoint.hxx"
#include "sg2_math.hxx"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace sg2 {

enum correction_refraction_e {
	REFRACTION_NONE = 0,
	REFRACTION_SAE = 1,
	REFRACTION_ZIM = 2
};

struct topocentric_data {
	double r_alpha;  /* Topocentric right sun ascension (rad) */
	double delta;    /* Topocentric sun declination (rad) */
	double omega;    /* Topocentric local hour angle (rad) */
	double gamma_S0; /* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double alpha_S;  /* Topocentric sun azimuth (rad) */
	double toa_ni;   /* irradiation at top of atmosphere normal incidence (W/m2) */
	double toa_hi;   /* irradiation at top of atmosphere horizontal incidence (W/m2) */

	topocentric_data() = default;
	topocentric_data(topocentric_data const &) = default;
	topocentric_data(geocentric_data const & geoc, geopoint const & gp);

	auto operator=(topocentric_data const &) -> topocentric_data & = default;

	double topocentric_correction_refraction(double const P, double const T, correction_refraction_e method = REFRACTION_NONE) const;

	/* Topocentric right sun ascension (rad) */
	double get_right_sun_ascension() const {
		return r_alpha;
	}

	/* Topocentric sun declination (rad) */
	double get_sun_declination() const {
		return delta;
	}

	/* Topocentric local hour angle (rad) */
	double get_omega() const {
		return omega;
	}

	/* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double get_geometric_sun_elevation() const {
		return gamma_S0;
	}

	/* Topocentric sun azimuth (rad) */
	double get_geometric_azimuth() const {
		return alpha_S;
	}

	/* irradiation at top of atmosphere normal incidence (W/m2) */
	double get_top_of_atmosphere_normal_incidence() const {
		return toa_ni;
	}

	/* irradiation at top of atmosphere horizontal incidence (W/m2) */
	double get_top_of_atmosphere_horizontal_incidence() const {
		return toa_hi;
	}

};


inline double topocentric_correction_refraction_SAE(double const gamma_S0, double const P, double const T)
{
	double const gamma_S0_seuil = -0.010035643198967;
	double const R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;

	K = (P / 1010.0) * (283. / (273. + T)) * 2.96706e-4;

	if (gamma_S0 > gamma_S0_seuil) {
		return gamma_S0 + K / (math::tan(gamma_S0 + 0.0031376 / (gamma_S0 + 0.089186)));
	} else {
		return gamma_S0 + (K / R) * math::tan(gamma_S0_seuil) / math::tan(gamma_S0);
	}

}


inline double topocentric_correction_refraction_ZIM(double const gamma_S0, double const P, double const T)
{
	double K;
	double tan_gamma_S0 = math::tan(gamma_S0);

	K = (P / 1013.0) * (283. / (273. + T)) * 4.848136811095360e-006;

	if (gamma_S0 <= -0.010036) {
		return gamma_S0 + K * (-20.774 / tan_gamma_S0);
	} else if (gamma_S0 <= 0.087266) {
		return gamma_S0 + K * polyval(gamma_S0, 7.66231727e6, -2.405683e6, 3.394422e5, -2.969067e4, 1735.0);
	} else if (gamma_S0 <= 1.483529864195180) {
		return gamma_S0 + K * (58.1 / tan_gamma_S0 - 0.07 / ipow<3>(tan_gamma_S0) + 0.000086 / ipow<5>(tan_gamma_S0));
	} else {
		return NAN;
	}

}

inline double topocentric_data::topocentric_correction_refraction(double const P,
		double const T, correction_refraction_e method) const
{
	switch (method) {
	case REFRACTION_SAE:
		return topocentric_correction_refraction_SAE(gamma_S0, P, T);
		break;
	case REFRACTION_ZIM:
		return topocentric_correction_refraction_ZIM(gamma_S0, P, T);
		break;
	case REFRACTION_NONE:
		return gamma_S0;
		break;
	default:
		return NAN;
		break;
	}
}

inline topocentric_data::topocentric_data(geocentric_data const & geoc, geopoint const & gp)
{

	if (!std::isfinite(geoc.R)) {
		r_alpha = std::numeric_limits<double>::quiet_NaN();
		delta = std::numeric_limits<double>::quiet_NaN();
		omega = std::numeric_limits<double>::quiet_NaN();
		gamma_S0 = std::numeric_limits<double>::quiet_NaN();
		alpha_S = std::numeric_limits<double>::quiet_NaN();
		toa_ni = std::numeric_limits<double>::quiet_NaN();
		toa_hi = std::numeric_limits<double>::quiet_NaN();
		return;
	}

	double xi = (gp.ellipse.a / AU);

    double omega_g_kp_kd = geoc.nu - geoc.r_alpha + gp.lambda;
    double cos_geoc_delta_kd = math::cos(geoc.delta);

    double Delta_r_alpha_kp_kd = (-gp.x * math::sin(omega_g_kp_kd)
            / cos_geoc_delta_kd * xi);
    r_alpha = geoc.r_alpha + Delta_r_alpha_kp_kd;

    delta = geoc.delta + (gp.x * math::cos(omega_g_kp_kd) * math::sin(geoc.delta)
                             - gp.y * cos_geoc_delta_kd) * xi;

    omega = omega_g_kp_kd - Delta_r_alpha_kp_kd;

    double cos_omega_kp_kd = math::cos(omega);
    double cos_delta_kp_kd = math::cos(delta);
    double sin_delta_kp_kd = math::sin(delta);
    double tan_delta_kp_kd = sin_delta_kp_kd / cos_delta_kp_kd;

    gamma_S0 = math::asin(gp.sin_phi_kp * sin_delta_kp_kd
                       + gp.cos_phi_kp * cos_delta_kp_kd * cos_omega_kp_kd);

    alpha_S = math::atan2(math::sin(omega), cos_omega_kp_kd * gp.sin_phi_kp - tan_delta_kp_kd * gp.cos_phi_kp)
                    + PI;

    if (gamma_S0 > 0.0) {
        toa_ni = SOLAR_CONSTANT / (geoc.R * geoc.R);
        toa_hi = toa_ni * math::sin(gamma_S0);
    } else {
        toa_ni = 0.0;
        toa_hi = 0.0;
    }
}

} // namespace sg2

#endif /* SG2_TOPOCENTRIC_HXX_ */
