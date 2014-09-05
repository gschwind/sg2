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

#define SG2_TOPOCENTRIC_C_

#include "sg2.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const S_SG2_ELLPS tab_ellps_ref[8] =
{
{ 6378137.0, 3.352810664747481e-003 }, /* WGS84 */
{ 6378137.0, 3.352810681182319e-003 }, /* RFG83 */
{ 6378249.2, 3.407549520015651e-003 }, /* NTF / CLARKE1880 */
{ 6378136.6, 3.352819697896193e-003 }, /* AA */
{ 6378140.0, 3.352810000000000e-003 }, /* SPA */
{ 6378169.0, 3.384231430681783e-003 }, /* NGP*/
{ 6378130.0, 0.0 }, /* SPHERE */
{ 0.0, 0.0 }, /* USER-DEFINED */
};

S_SG2_ELLPS const * SG2_ELLPSTYPE_WGS84 = &tab_ellps_ref[0];
S_SG2_ELLPS const * SG2_ELLPSTYPE_RGF83 = &tab_ellps_ref[1];
S_SG2_ELLPS const * SG2_ELLPSTYPE_NTF = &tab_ellps_ref[2];
S_SG2_ELLPS const * SG2_ELLPSTYPE_AA = &tab_ellps_ref[3];
S_SG2_ELLPS const * SG2_ELLPSTYPE_SPA = &tab_ellps_ref[4];
S_SG2_ELLPS const * SG2_ELLPSTYPE_NGP = &tab_ellps_ref[5];
S_SG2_ELLPS const * SG2_ELLPSTYPE_SPHERE = &tab_ellps_ref[6];

S_SG2_ELLPS *SG2_create_user_ellipse(double a, double f) {
	S_SG2_ELLPS * p_ret = (S_SG2_ELLPS *) malloc(sizeof(S_SG2_ELLPS));
	p_ret->a = a;
	p_ret->f = f;
	return p_ret;
}

void sg2_topocecentric_set_tabgeopoint(sg2_geopoint_t * ths, double lon,
		double lat, double h, S_SG2_ELLPS const *p_data_ellps, int *p_err) {
	int kp;
	double a, app;
	double u_kp, cos_phi_kp, sin_phi_kp, tan_phi_kp, h_a_kp;

	ths->ellps = p_data_ellps;

	a = p_data_ellps->a;
	app = 1.0 - p_data_ellps->f;

	ths->lambda = lon * SG2_DEG2RAD;
	ths->phi = lat * SG2_DEG2RAD;
	ths->h = h;

	cos_phi_kp = cos(ths->phi);
	sin_phi_kp = sin(ths->phi);
	tan_phi_kp = sin_phi_kp / cos_phi_kp;

	h_a_kp = ths->h / a;
	u_kp = atan(app * tan_phi_kp);
	ths->x = cos(u_kp) + h_a_kp * cos_phi_kp;
	ths->y = app * sin(u_kp) + h_a_kp * sin_phi_kp;
	ths->u = u_kp;
	ths->cos_phi = cos_phi_kp;
	ths->sin_phi = sin_phi_kp;

}

inline static void sg2_topocentric_correction_refraction_SAE(double *p_gamma_S0, double *p_data_corr,
		double *p_gamma_S, int *p_err) {

	static const double gamma_S0_seuil = -0.010035643198967;
	static const double R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;
	double tan_gamma_S0 = 0.0;
	double gamma_S0 = 0.0, gamma_S0_2 = 0.0, gamma_S0_3 = 0.0, gamma_S0_4 = 0.0;
	unsigned long k;
	double P, T;

	P = p_data_corr[0];
	T = p_data_corr[1];

	K = (P / 1010.0) * (283. / (273. + T)) * 2.96706e-4;

	gamma_S0 = *p_gamma_S0;
	if (gamma_S0 > gamma_S0_seuil) {
		*p_gamma_S = gamma_S0
				+ K / (tan(gamma_S0 + 0.0031376 / (gamma_S0 + 0.089186)));
	} else {
		*p_gamma_S = gamma_S0 + (K / R) * tan(gamma_S0_seuil) / tan(gamma_S0);
	}

}

inline static void sg2_topocentric_correction_refraction_ZIM(double *p_gamma_S0,
		double *p_data_corr, double *p_gamma_S, int *p_err) {

	static const double gamma_S0_seuil = -0.010035643198967;
	static const double R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;
	double tan_gamma_S0 = 0.0;
	double gamma_S0 = 0.0, gamma_S0_2 = 0.0, gamma_S0_3 = 0.0, gamma_S0_4 = 0.0;
	unsigned long k;
	double P, T;

	P = p_data_corr[0];
	T = p_data_corr[1];

	K = (P / 1013.0) * (283. / (273. + T)) * 4.848136811095360e-006;

	gamma_S0 = *p_gamma_S0;
	if (gamma_S0 <= -0.010036) {
		*p_gamma_S = gamma_S0 + (-20.774 / tan_gamma_S0) * K;
	} else if (gamma_S0 <= 0.087266) {
		gamma_S0_2 = gamma_S0 * gamma_S0;
		gamma_S0_3 = gamma_S0_2 * gamma_S0;
		gamma_S0_4 = gamma_S0_4 * gamma_S0;
		*p_gamma_S = gamma_S0
				+ (1735 - 2.969067e4 * gamma_S0 + 3.394422e5 * gamma_S0_2
						+ -2.405683e6 * gamma_S0_3 + 7.66231727e6 * gamma_S0_4)
						* K;
	} else if (gamma_S0 <= 1.483529864195180) {
		*p_gamma_S = gamma_S0
				+ K
						* (58.1 / tan_gamma_S0 - 0.07 / pow(tan_gamma_S0, 3.0)
								+ 0.000086 / pow(tan_gamma_S0, 5.0));
	}

}

void SG2_topocentric_correction_refraction(double *p_gamma_S0, unsigned long n,
		SG2_CORRECTION_REFRACTION method, double *p_data_corr,
		double *p_gamma_S, int *p_err)
{

	static const double gamma_S0_seuil = -0.010035643198967;
	static const double R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;
	double tan_gamma_S0 = 0.0;
	double gamma_S0 = 0.0, gamma_S0_2 = 0.0, gamma_S0_3 = 0.0, gamma_S0_4 = 0.0;
	unsigned long k;
	double P, T;

	switch (method)
	{
	case SG2_CORRECTION_REFRACTION_SAE:
		sg2_topocentric_correction_refraction_SAE(p_gamma_S0, p_data_corr, p_gamma_S, p_err);
		break;
	case SG2_CORRECTION_REFRACTION_ZIM:
		sg2_topocentric_correction_refraction_ZIM(p_gamma_S0, p_data_corr, p_gamma_S, p_err);
		break;
	case SG2_CORRECTION_REFRACTION_NONE:
		memcpy(p_gamma_S, p_gamma_S0, n * sizeof(double));
		return;
	}

}

void sg2_topocentric_set_topoc_data(sg2_topocentric_data_t * ths, sg2_geocentric_sun_position_t const * sun_position,
		sg2_geopoint_t const * geopoint, int * err)
{

	unsigned long np, kp, nd, kd;
	double u_kp, x_kp, y_kp, cos_phi_kp, sin_phi_kp;
	double omega_g_kp_kd;
	double geoc_r_alpha, geoc_delta, geoc_nu;
	double cos_geoc_delta_kd;
	double Delta_r_alpha_kp_kd, cos_omega_kp_kd;
	double cos_delta_kp_kd, sin_delta_kp_kd, tan_delta_kp_kd;
	double xi;

	ths->geopoint = geopoint;
	ths->sun_position = sun_position;

	xi = (geopoint->ellps->a / SG2_AU);

	{

		cos_phi_kp = geopoint->cos_phi;
		sin_phi_kp = geopoint->sin_phi;

		u_kp = geopoint->u;
		x_kp = geopoint->x;
		y_kp = geopoint->y;

		geoc_nu = sun_position->geoc.nu;
		geoc_r_alpha = sun_position->geoc.r_alpha;
		geoc_delta = sun_position->geoc.delta;

		{

			omega_g_kp_kd = geoc_nu - geoc_r_alpha + geopoint->lambda;
			cos_geoc_delta_kd = cos(geoc_delta);

			Delta_r_alpha_kp_kd = (-x_kp * sin(omega_g_kp_kd)
					/ cos_geoc_delta_kd * xi);
			ths->r_alpha = geoc_r_alpha + Delta_r_alpha_kp_kd;

			ths->delta = geoc_delta + (x_kp
					* cos(omega_g_kp_kd) * sin(geoc_delta) - y_kp
					* cos_geoc_delta_kd) * xi;

			ths->omega = omega_g_kp_kd - Delta_r_alpha_kp_kd;

			cos_omega_kp_kd = cos(ths->omega);
			cos_delta_kp_kd = cos(ths->delta);
			sin_delta_kp_kd = sin(ths->delta);
			tan_delta_kp_kd = sin_delta_kp_kd / cos_delta_kp_kd;

			ths->gamma_S0 = asin(sin_phi_kp * sin_delta_kp_kd
					+ cos_phi_kp * cos_delta_kp_kd * cos_omega_kp_kd);

			ths->alpha_S
					= atan2(sin(ths->omega), cos_omega_kp_kd
							* sin_phi_kp - tan_delta_kp_kd * cos_phi_kp)
							+ SG2_PI;

			if (ths->gamma_S0 > 0.0) {
				ths->toa_ni = (1367.0)
						/ (sun_position->helioc.R * sun_position->helioc.R);
				ths->toa_hi = (ths->toa_ni
						* sin(ths->gamma_S0));
			} else {
				ths->toa_ni = 0.0;
				ths->toa_hi = 0.0;
			}

		}
	}

}

