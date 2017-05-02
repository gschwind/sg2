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

#ifndef SG2_TOPOCENTRIC_HXX_
#define SG2_TOPOCENTRIC_HXX_

#include "sg2_typedef.hxx"
#include "sg2_geocentric.hxx"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace sg2 {

extern ellps const ELLPS_WGS84;
extern ellps const ELLPS_RGF83;
extern ellps const ELLPS_NTF;
extern ellps const ELLPS_AA;
extern ellps const ELLPS_SPA;
extern ellps const ELLPS_NGP;
extern ellps const ELLPS_SPHERE;

struct ellps {
	double a; /* Axis a (m) */
	double f; /* Flatness (-)*/

	ellps() { }
	ellps(ellps const &) = default;
	ellps(double a, double f);

};

struct geopoint_data {
	ellps  ellipse;
	double phi;        /* Latitude (rad) */
	double lambda;     /* Longitude (rad) */
	double h;          /* Altitude Above the Reference Ellipsoid */
	double cos_phi;
	double sin_phi;
	double u;
	double x;
	double y;

	geopoint_data() { }
	geopoint_data(geopoint_data const &) = default;

	/**
	 * @input lat: latitude in degrees
	 * @input lon: longitude in degrees
	 * @input h: altitude in meters
	 **/
	geopoint_data(double lon, double lat, double h, ellps const & p_data_ellps = ELLPS_WGS84);

	auto operator=(geopoint_data const &) -> geopoint_data & = default;

	double get_phi() const {
		return phi;
	}

	double get_lambda() const {
		return lambda;
	}

	double get_cos_phi() const {
		return cos_phi;
	}

	double get_sin_phi() const {
		return sin_phi;
	}

	double get_u() const {
		return u;
	}

	double get_x() const {
		return x;
	}

	double get_y() const {
		return y;
	}

};

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

	topocentric_data() { }
	topocentric_data(topocentric_data const &) = default;
	topocentric_data(geocentric_data const & geoc, geopoint_data const & geopoint);

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

inline ellps::ellps(double a, double f) :
		a{a},
		f{f}
{

}

inline geopoint_data::geopoint_data(double lon, double lat, double h, ellps const & ellipse) :
	ellipse{ellipse},
	lambda{lon * DEG2RAD},
	phi{lat * DEG2RAD},
	h{h}
{
	int kp;
	double a, app;
	double u_kp, cos_phi_kp, sin_phi_kp, tan_phi_kp, h_a_kp;

	a = ellipse.a;
	app = 1.0 - ellipse.f;

	cos_phi_kp = vdt::fast_cos(phi);
	sin_phi_kp = vdt::fast_sin(phi);
	tan_phi_kp = sin_phi_kp / cos_phi_kp;

	h_a_kp = h / a;
	u_kp = vdt::fast_atan(app * tan_phi_kp);
	x = vdt::fast_cos(u_kp) + h_a_kp * cos_phi_kp;
	y = app * vdt::fast_sin(u_kp) + h_a_kp * sin_phi_kp;
	u = u_kp;
	cos_phi = cos_phi_kp;
	sin_phi = sin_phi_kp;

}

inline static double _topocentric_correction_refraction_SAE(double const gamma_S0, double const P, double const T)
{
	double const gamma_S0_seuil = -0.010035643198967;
	double const R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;
	double tan_gamma_S0 = 0.0;
	double gamma_S0_2 = 0.0, gamma_S0_3 = 0.0, gamma_S0_4 = 0.0;
	unsigned long k;

	K = (P / 1010.0) * (283. / (273. + T)) * 2.96706e-4;

	if (gamma_S0 > gamma_S0_seuil) {
		return gamma_S0 + K / (vdt::fast_tan(gamma_S0 + 0.0031376 / (gamma_S0 + 0.089186)));
	} else {
		return gamma_S0 + (K / R) * vdt::fast_tan(gamma_S0_seuil) / vdt::fast_tan(gamma_S0);
	}

}

inline static double _topocentric_correction_refraction_ZIM(double const gamma_S0, double const P, double const T)
{
	static const double gamma_S0_seuil = -0.010035643198967;
	static const double R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;
	double tan_gamma_S0 = 0.0;
	double gamma_S0_2 = 0.0, gamma_S0_3 = 0.0, gamma_S0_4 = 0.0;
	unsigned long k;

	K = (P / 1013.0) * (283. / (273. + T)) * 4.848136811095360e-006;

	if (gamma_S0 <= -0.010036) {
		return gamma_S0 + (-20.774 / tan_gamma_S0) * K;
	} else if (gamma_S0 <= 0.087266) {
		gamma_S0_2 = gamma_S0 * gamma_S0;
		gamma_S0_3 = gamma_S0_2 * gamma_S0;
		gamma_S0_4 = gamma_S0_4 * gamma_S0;
		return gamma_S0
				+ (1735 - 2.969067e4 * gamma_S0 + 3.394422e5 * gamma_S0_2
				+ -2.405683e6 * gamma_S0_3 + 7.66231727e6 * gamma_S0_4) * K;
	} else if (gamma_S0 <= 1.483529864195180) {
		return gamma_S0 + K * (58.1 / tan_gamma_S0 - 0.07 / pow(tan_gamma_S0, 3.0) + 0.000086 / pow(tan_gamma_S0, 5.0));
	} else {
		return NAN;
	}

}

inline double topocentric_data::topocentric_correction_refraction(double const P,
		double const T, correction_refraction_e method) const
{
	switch (method) {
	case REFRACTION_SAE:
		return _topocentric_correction_refraction_SAE(gamma_S0, P, T);
		break;
	case REFRACTION_ZIM:
		return _topocentric_correction_refraction_ZIM(gamma_S0, P, T);
		break;
	case REFRACTION_NONE:
		return gamma_S0;
		break;
	default:
		return NAN;
		break;
	}
}

inline topocentric_data::topocentric_data(geocentric_data const & geoc, geopoint_data const & geopoint)
{
	unsigned long np, kp, nd, kd;
	double u_kp, x_kp, y_kp, cos_phi_kp, sin_phi_kp;
	double omega_g_kp_kd;
	double geoc_r_alpha, geoc_delta, geoc_nu;
	double cos_geoc_delta_kd;
	double Delta_r_alpha_kp_kd, cos_omega_kp_kd;
	double cos_delta_kp_kd, sin_delta_kp_kd, tan_delta_kp_kd;
	double xi;

	xi = (geopoint.ellipse.a / AU);

    cos_phi_kp = geopoint.cos_phi;
    sin_phi_kp = geopoint.sin_phi;

    u_kp = geopoint.u;
    x_kp = geopoint.x;
    y_kp = geopoint.y;

    geoc_nu = geoc.nu;
    geoc_r_alpha = geoc.r_alpha;
    geoc_delta = geoc.delta;

    omega_g_kp_kd = geoc_nu - geoc_r_alpha + geopoint.lambda;
    cos_geoc_delta_kd = cos(geoc_delta);

    Delta_r_alpha_kp_kd = (-x_kp * vdt::fast_sin(omega_g_kp_kd)
            / cos_geoc_delta_kd * xi);
    r_alpha = geoc_r_alpha + Delta_r_alpha_kp_kd;

    delta = geoc_delta + (x_kp * vdt::fast_cos(omega_g_kp_kd) * vdt::fast_sin(geoc_delta)
                             - y_kp * cos_geoc_delta_kd) * xi;

    omega = omega_g_kp_kd - Delta_r_alpha_kp_kd;

    cos_omega_kp_kd = vdt::fast_cos(omega);
    cos_delta_kp_kd = vdt::fast_cos(delta);
    sin_delta_kp_kd = vdt::fast_sin(delta);
    tan_delta_kp_kd = sin_delta_kp_kd / cos_delta_kp_kd;

    gamma_S0 = vdt::fast_asin(sin_phi_kp * sin_delta_kp_kd
                       + cos_phi_kp * cos_delta_kp_kd * cos_omega_kp_kd);

    alpha_S = vdt::fast_atan2(vdt::fast_sin(omega), cos_omega_kp_kd * sin_phi_kp - tan_delta_kp_kd * cos_phi_kp)
                    + PI;

    if (gamma_S0 > 0.0) {
        toa_ni = SOLAR_CONSTANT / (geoc.R * geoc.R);
        toa_hi = toa_ni * vdt::fast_sin(gamma_S0);
    } else {
        toa_ni = 0.0;
        toa_hi = 0.0;
    }
}

} // namespace sg2

#endif /* SG2_TOPOCENTRIC_HXX_ */
