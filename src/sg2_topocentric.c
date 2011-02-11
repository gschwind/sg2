/*
 * sg2_topocentric.c
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#define SG2_TOPOCENTRIC_C_

#include "sg2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const S_SG2_ELLPS tab_ellps_ref[8] = { { .ellpstype = 0, .a = 6378137.0,
		.f = 3.352810664747481e-003 }, /* WGS84 */
{ .ellpstype = 1, .a = 6378137.0, .f = 3.352810681182319e-003 }, /* RFG83 */
{ .ellpstype = 2, .a = 6378249.2, .f = 3.407549520015651e-003 }, /* NTF / CLARKE1880 */
{ .ellpstype = 3, .a = 6378136.6, .f = 3.352819697896193e-003 }, /* AA */
{ .ellpstype = 4, .a = 6378140.0, .f = 3.352810000000000e-003 }, /* SPA */
{ .ellpstype = 5, .a = 6378169.0, .f = 3.384231430681783e-003 }, /* NGP*/
{ .ellpstype = 6, .a = 6378130.0, .f = 0.0 }, /* SPHERE */
{ .ellpstype = 7, .a = 0.0, .f = 0.0 }, /* USER-DEFINED */
};

S_SG2_TABGEOPOINT *SG2_topocentric_create_tabgeopoint(unsigned long np,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, int *p_err) {
	S_SG2_TABGEOPOINT *p_gp;

	p_gp = (S_SG2_TABGEOPOINT*) malloc(sizeof(S_SG2_TABGEOPOINT));
	if (p_gp == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_1;
		return NULL;
	}
	p_gp->np = np;
	p_gp->p_ellps = (S_SG2_ELLPS *) malloc(sizeof(S_SG2_ELLPS));
	if (p_gp->p_ellps == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_2;
		free(p_gp);
		return NULL;
	}
	p_gp->p_ellps->ellpstype = ellpstype;
	if (ellpstype != SG2_ELLPSTYPE_USER) {
		p_gp->p_ellps->a = tab_ellps_ref[ellpstype].a;
		p_gp->p_ellps->f = tab_ellps_ref[ellpstype].f;
	} else {
		p_gp->p_ellps->a = p_data_ellps[0];
		p_gp->p_ellps->f = p_data_ellps[1];
	}

	p_gp->phi = (double *) malloc(sizeof(double) * np);
	if (p_gp->phi == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_3;
		free(p_gp->p_ellps);
		free(p_gp);
		return NULL;
	}
	p_gp->lambda = (double *) malloc(sizeof(double) * np);
	if (p_gp->lambda == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_4;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp);
		return NULL;
	}
	p_gp->h = (double *) malloc(sizeof(double) * np);
	if (p_gp->h == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_5;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp);
		return NULL;
	}
	p_gp->u = (double *) malloc(sizeof(double) * np);
	if (p_gp->u == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_6;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp->h);
		free(p_gp);
		return NULL;
	}
	p_gp->x = (double *) malloc(sizeof(double) * np);
	if (p_gp->x == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_7;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp->h);
		free(p_gp->u);
		free(p_gp);
		return NULL;
	}
	p_gp->y = (double *) malloc(sizeof(double) * np);
	if (p_gp->y == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_8;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp->h);
		free(p_gp->u);
		free(p_gp->x);
		free(p_gp);
		return NULL;
	}

	p_gp->cos_phi = (double *) malloc(sizeof(double) * np);
	if (p_gp->cos_phi == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_9;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp->h);
		free(p_gp->u);
		free(p_gp->x);
		free(p_gp->y);
		free(p_gp);
		return NULL;
	}

	p_gp->sin_phi = (double *) malloc(sizeof(double) * np);
	if (p_gp->sin_phi == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_GEOPT_MALLOC_10;
		free(p_gp->p_ellps);
		free(p_gp->phi);
		free(p_gp->lambda);
		free(p_gp->h);
		free(p_gp->u);
		free(p_gp->x);
		free(p_gp->y);
		free(p_gp->cos_phi);
		free(p_gp);
		return NULL;
	}

	return p_gp;
}

void SG2_topocecentric_set_tabgeopoint(double *lon, double *lat, double *h,
		S_SG2_TABGEOPOINT *p_gp, int *p_err) {
	int kp;
	double a, app;
	double u_kp, cos_phi_kp, sin_phi_kp, tan_phi_kp, h_a_kp;

	a = p_gp->p_ellps->a;
	app = 1.0 - p_gp->p_ellps->f;

	for (kp = 0; kp < p_gp->np; kp++) {

		p_gp->lambda[kp] = lon[kp] * SG2_DEG2RAD;
		p_gp->phi[kp] = lat[kp] * SG2_DEG2RAD;
		if (h == NULL) {
			p_gp->h[kp] = 0.0;
		} else {
			p_gp->h[kp] = h[kp];
		}

		cos_phi_kp = cos(p_gp->phi[kp]);
		sin_phi_kp = sin(p_gp->phi[kp]);
		tan_phi_kp = sin_phi_kp / cos_phi_kp;

		h_a_kp = p_gp->h[kp] / a;
		u_kp = atan(app * tan_phi_kp);
		p_gp->x[kp] = cos(u_kp) + h_a_kp * cos_phi_kp;
		p_gp->y[kp] = app * sin(u_kp) + h_a_kp * sin_phi_kp;
		p_gp->u[kp] = u_kp;
		p_gp->cos_phi[kp] = cos_phi_kp;
		p_gp->sin_phi[kp] = sin_phi_kp;

	}

}

void SG2_topocentric_delete_tabgeopoint(S_SG2_TABGEOPOINT *p_gp, int *p_err) {
	free(p_gp->p_ellps);
	free(p_gp->phi);
	free(p_gp->lambda);
	free(p_gp->h);
	free(p_gp->u);
	free(p_gp->x);
	free(p_gp->y);
	free(p_gp->cos_phi);
	free(p_gp->sin_phi);
	free(p_gp);
}

S_SG2_TOPOC_DATA *SG2_topocentric_create_topoc_data(unsigned long np,
		unsigned long nd, int *p_err) {
	S_SG2_TOPOC_DATA *p_topoc;
	double *p_tmp1, *p_tmp2, *p_tmp3, *p_tmp4, *p_tmp5;
	unsigned long kp;

	p_topoc = (S_SG2_TOPOC_DATA *) malloc(sizeof(S_SG2_TOPOC_DATA));
	if (p_topoc == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_1;
		return NULL;
	}
	p_topoc->nd = nd;
	p_topoc->np = np;

	/*
	 double **r_alpha;
	 double **delta;
	 double **omega;
	 double **gamma_S;
	 double **alpha_S;
	 */

	p_tmp1 = (double *) malloc(nd * np * sizeof(double));
	if (p_tmp1 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_1;
		free(p_topoc);
		return NULL;
	}
	p_topoc->r_alpha = (double **) malloc(np * sizeof(double *));
	if (p_topoc->r_alpha == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_2;
		free(p_tmp1);
		free(p_topoc);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_topoc->r_alpha[kp] = &p_tmp1[nd * kp];
	}

	p_tmp2 = (double *) malloc(nd * np * sizeof(double));
	if (p_tmp2 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_3;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc);
		return NULL;
	}
	p_topoc->delta = (double **) malloc(np * sizeof(double *));
	if (p_topoc->delta == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_4;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_tmp2);
		free(p_topoc);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_topoc->delta[kp] = &p_tmp2[nd * kp];
	}

	p_tmp3 = (double *) malloc(nd * np * sizeof(double));
	if (p_tmp3 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_5;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc->delta);
		free(p_tmp2);
		free(p_topoc);
		return NULL;
	}
	p_topoc->omega = (double **) malloc(np * sizeof(double *));
	if (p_topoc->omega == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_6;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc->delta);
		free(p_tmp2);
		free(p_tmp3);
		free(p_topoc);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_topoc->omega[kp] = &p_tmp3[nd * kp];
	}

	p_tmp4 = (double *) malloc(nd * np * sizeof(double));
	if (p_tmp4 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_7;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc->delta);
		free(p_tmp2);
		free(p_topoc->omega);
		free(p_tmp3);
		free(p_topoc);
		return NULL;
	}
	p_topoc->gamma_S0 = (double **) malloc(np * sizeof(double *));
	if (p_topoc->gamma_S0 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_8;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc->delta);
		free(p_tmp2);
		free(p_topoc->omega);
		free(p_tmp3);
		free(p_tmp4);
		free(p_topoc);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_topoc->gamma_S0[kp] = &p_tmp4[nd * kp];
	}

	p_tmp5 = (double *) malloc(nd * np * sizeof(double));
	if (p_tmp5 == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_9;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc->delta);
		free(p_tmp2);
		free(p_topoc->omega);
		free(p_tmp3);
		free(p_topoc->gamma_S0);
		free(p_tmp4);
		free(p_topoc);
		return NULL;
	}
	p_topoc->alpha_S = (double **) malloc(np * sizeof(double *));
	if (p_topoc->alpha_S == NULL) {
		*p_err = SG2_ERR_TOPOCENTRIC_CREATE_TOPOC_MALLOC_10;
		free(p_topoc->r_alpha);
		free(p_tmp1);
		free(p_topoc->delta);
		free(p_tmp2);
		free(p_topoc->omega);
		free(p_tmp3);
		free(p_topoc->gamma_S0);
		free(p_tmp4);
		free(p_tmp5);
		free(p_topoc);
		return NULL;
	}
	for (kp = 0; kp < np; kp++) {
		p_topoc->alpha_S[kp] = &p_tmp5[nd * kp];
	}

	return p_topoc;

}

void SG2_topocentric_delete_topoc_data(S_SG2_TOPOC_DATA *p_topoc, int *p_err) {
	free(p_topoc->r_alpha[0]);
	free(p_topoc->r_alpha);
	free(p_topoc->delta[0]);
	free(p_topoc->delta);
	free(p_topoc->gamma_S0[0]);
	free(p_topoc->gamma_S0);
	free(p_topoc->alpha_S[0]);
	free(p_topoc->alpha_S);
	free(p_topoc);
}

void SG2_topocentric_set_topoc_data(S_SG2_GEOC_DATA *p_geoc,
		S_SG2_TABGEOPOINT *p_gp, S_SG2_TOPOC_DATA *p_topoc, int *p_err) {

	unsigned long np, kp, nd, kd;
	double u_kp, x_kp, y_kp, cos_phi_kp, sin_phi_kp;
	double omega_g_kp_kd;
	double *geoc_r_alpha, *geoc_delta, *geoc_nu;
	double cos_geoc_delta_kd;
	double Delta_r_alpha_kp_kd, cos_omega_kp_kd;
	double cos_delta_kp_kd, sin_delta_kp_kd, tan_delta_kp_kd;
	double xi;

	p_topoc->p_gp = p_gp;
	p_topoc->p_geoc = p_geoc;
	p_topoc->p_jd = p_geoc->p_jd;

	np = p_topoc->np;
	nd = p_topoc->nd;

	xi = (p_gp->p_ellps->a / SG2_AU);

	for (kp = 0; kp < np; kp++) {

		cos_phi_kp = p_gp->cos_phi[kp];
		sin_phi_kp = p_gp->sin_phi[kp];

		u_kp = p_gp->u[kp];
		x_kp = p_gp->x[kp];
		y_kp = p_gp->y[kp];

		geoc_nu = p_geoc->nu;
		geoc_r_alpha = p_geoc->r_alpha;
		geoc_delta = p_geoc->delta;

		for (kd = 0; kd < nd; kd++) {

			omega_g_kp_kd = geoc_nu[kd] - geoc_r_alpha[kd] + p_gp->lambda[kp];
			cos_geoc_delta_kd = cos(geoc_delta[kd]);

			Delta_r_alpha_kp_kd = (-x_kp * sin(omega_g_kp_kd)
					/ cos_geoc_delta_kd * xi);
			p_topoc->r_alpha[kp][kd] = geoc_r_alpha[kd] + Delta_r_alpha_kp_kd;

			p_topoc->delta[kp][kd] = geoc_delta[kd] + (x_kp
					* cos(omega_g_kp_kd) * sin(geoc_delta[kd]) - y_kp
					* cos_geoc_delta_kd) * xi;

			p_topoc->omega[kp][kd] = omega_g_kp_kd - Delta_r_alpha_kp_kd;

			cos_omega_kp_kd = cos(p_topoc->omega[kp][kd]);
			cos_delta_kp_kd = cos(p_topoc->delta[kp][kd]);
			sin_delta_kp_kd = sin(p_topoc->delta[kp][kd]);
			tan_delta_kp_kd = sin_delta_kp_kd / cos_delta_kp_kd;

			p_topoc->gamma_S0[kp][kd] = asin(sin_phi_kp * sin_delta_kp_kd
					+ cos_phi_kp * cos_delta_kp_kd * cos_omega_kp_kd);

			p_topoc->alpha_S[kp][kd]
					= atan2(sin(p_topoc->omega[kp][kd]), cos_omega_kp_kd
							* sin_phi_kp - tan_delta_kp_kd * cos_phi_kp)
							+ SG2_PI;

		}
	}

}

void SG2_topocentric_correction_refraction(double *p_gamma_S0, unsigned long n,
		SG2_CORRECTION_REFRACTION method, double *p_data_corr, double *p_gamma_S, int *p_err) {

	static const double gamma_S0_seuil = -0.010035643198967;
	static const double R = 0.029614018235657;
	/*(tan(gamma_S0_seuil + 0.0031376 / (gamma_S0_seuil+ 0.089186))) */
	double K;
	double Dgamma_S = 0.0;
	double tan_gamma_S0;
	double gamma_S0, gamma_S0_2, gamma_S0_3, gamma_S0_4;
	unsigned long k;
	double P, T;

	switch (method) {

	case SG2_CORRECTION_REFRACTION_SAE:

		P = p_data_corr[0];
		T = p_data_corr[1];

		K = (P / 1010.0) * (283. / (273. + T)) * 2.96706e-4;
		for (k = 0; k < n; k++) {
			gamma_S0 = p_gamma_S0[k];
			if (gamma_S0 > gamma_S0_seuil) {
				p_gamma_S[k] = gamma_S0 + K / (tan(gamma_S0 + 0.0031376
						/ (gamma_S0 + 0.089186)));
			} else {
				p_gamma_S[k] = gamma_S0 + (K / R) * tan(gamma_S0_seuil) / tan(
						gamma_S0);
			}
		}
		break;
	case SG2_CORRECTION_REFRACTION_ZIM:

		P = p_data_corr[0];
		T = p_data_corr[1];

		K = (P / 1013.0) * (283. / (273. + T)) * 4.848136811095360e-006;
		for (k = 0; k < n; k++) {
			gamma_S0 = p_gamma_S0[k];
			if (gamma_S0 <= -0.010036) {
				p_gamma_S[k] = gamma_S0 + (-20.774 / tan_gamma_S0) * K;
			} else if (gamma_S0 <= 0.087266) {
				gamma_S0_2 = gamma_S0 * gamma_S0;
				gamma_S0_3 = gamma_S0_2 * gamma_S0;
				gamma_S0_4 = gamma_S0_4 * gamma_S0;
				p_gamma_S[k] = gamma_S0 + (1735 - 2.969067e4 * gamma_S0
						+ 3.394422e5 * gamma_S0_2 + -2.405683e6 * gamma_S0_3
						+ 7.66231727e6 * gamma_S0_4) * K;
			} else if (gamma_S0 <= 1.483529864195180) {
				p_gamma_S[k] = gamma_S0 + K * (58.1 / tan_gamma_S0 - 0.07
						/ pow(tan_gamma_S0, 3.0) + 0.000086 / pow(tan_gamma_S0,
						5.0));
			}
		}
		break;
	default:
		memcpy(p_gamma_S, p_gamma_S0, n*sizeof(double));
		return;
	}


}

