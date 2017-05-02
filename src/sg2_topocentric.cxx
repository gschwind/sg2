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


#include "sg2_topocentric.hxx"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace sg2 {

ellps const ELLPS_WGS84  = { 6378137.0, 3.352810664747481e-003 }; /* WGS84 */
ellps const ELLPS_RGF83  = { 6378137.0, 3.352810681182319e-003 }; /* RFG83 */
ellps const ELLPS_NTF    = { 6378249.2, 3.407549520015651e-003 }; /* NTF / CLARKE1880 */
ellps const ELLPS_AA     = { 6378136.6, 3.352819697896193e-003 }; /* AA */
ellps const ELLPS_SPA    = { 6378140.0, 3.352810000000000e-003 }; /* SPA */
ellps const ELLPS_NGP    = { 6378169.0, 3.384231430681783e-003 }; /* NGP*/
ellps const ELLPS_SPHERE = { 6378130.0, 0.0                    }; /* SPHERE */

ellps::ellps(double a, double f) :
		a{a},
		f{f}
{

}

geopoint_data::geopoint_data(double lon, double lat, double h, ellps const & ellipse) :
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

	cos_phi_kp = cos(phi);
	sin_phi_kp = sin(phi);
	tan_phi_kp = sin_phi_kp / cos_phi_kp;

	h_a_kp = h / a;
	u_kp = atan(app * tan_phi_kp);
	x = cos(u_kp) + h_a_kp * cos_phi_kp;
	y = app * sin(u_kp) + h_a_kp * sin_phi_kp;
	u = u_kp;
	cos_phi = cos_phi_kp;
	sin_phi = sin_phi_kp;

}

inline double _topocentric_correction_refraction_SAE(double const gamma_S0, double const P, double const T)
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
		return gamma_S0 + K / (tan(gamma_S0 + 0.0031376 / (gamma_S0 + 0.089186)));
	} else {
		return gamma_S0 + (K / R) * tan(gamma_S0_seuil) / tan(gamma_S0);
	}

}

inline double _topocentric_correction_refraction_ZIM(double const gamma_S0, double const P, double const T)
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

double topocentric_data::topocentric_correction_refraction(double const P,
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
	}

}

topocentric_data::topocentric_data(geocentric_data const & geoc, geopoint_data const & geopoint)
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

    Delta_r_alpha_kp_kd = (-x_kp * sin(omega_g_kp_kd)
            / cos_geoc_delta_kd * xi);
    r_alpha = geoc_r_alpha + Delta_r_alpha_kp_kd;

    delta = geoc_delta + (x_kp * cos(omega_g_kp_kd) * sin(geoc_delta)
                             - y_kp * cos_geoc_delta_kd) * xi;

    omega = omega_g_kp_kd - Delta_r_alpha_kp_kd;

    cos_omega_kp_kd = cos(omega);
    cos_delta_kp_kd = cos(delta);
    sin_delta_kp_kd = sin(delta);
    tan_delta_kp_kd = sin_delta_kp_kd / cos_delta_kp_kd;

    gamma_S0 = asin(sin_phi_kp * sin_delta_kp_kd
                       + cos_phi_kp * cos_delta_kp_kd * cos_omega_kp_kd);

    alpha_S = atan2(sin(omega), cos_omega_kp_kd * sin_phi_kp - tan_delta_kp_kd * cos_phi_kp)
                    + PI;

    if (gamma_S0 > 0.0) {
        toa_ni = SOLAR_CONSTANT / (geoc.R * geoc.R);
        toa_hi = toa_ni * sin(gamma_S0);
    } else {
        toa_ni = 0.0;
        toa_hi = 0.0;
    }
}

} // namespace sg2
