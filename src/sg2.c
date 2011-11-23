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

#define SG2_C_

#include "sg2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

S_SG2_SUNPOS *sg2_create_sunpos(unsigned long np, unsigned long nd, int *p_err)
{

	S_SG2_SUNPOS *p_sunpos;
	double *p_tmp1, *p_tmp2, *p_tmp3, *p_tmp4, *p_tmp5, *p_tmp6;
	int kp;

	p_sunpos = (S_SG2_SUNPOS *) malloc(sizeof(S_SG2_SUNPOS));
	if (p_sunpos == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_1;
		return NULL;
	}
	p_sunpos->np = np;
	p_sunpos->nd = nd;

	p_tmp1 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp1 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_2;
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->alpha_S = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->alpha_S == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_3;
		free(p_tmp1);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++)
	{
		p_sunpos->alpha_S[kp] = &p_tmp1[kp * nd];
	}

	p_tmp2 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp2 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_4;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->gamma_S0 = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->gamma_S0 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_5;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++)
	{
		p_sunpos->gamma_S0[kp] = &p_tmp2[kp * nd];
	}

	p_tmp3 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp3 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_6;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->gamma_S = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->gamma_S == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_7;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp3);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++)
	{
		p_sunpos->gamma_S[kp] = &p_tmp3[kp * nd];
	}

	p_tmp4 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp4 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_8;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->omega = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->omega == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_9;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp3);
		free(p_sunpos->gamma_S);
		free(p_tmp4);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++)
	{
		p_sunpos->omega[kp] = &p_tmp4[kp * nd];
	}

	p_tmp5 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp5 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_10;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp4);
		free(p_sunpos->omega);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->delta = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->delta == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_11;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp3);
		free(p_sunpos->gamma_S);
		free(p_tmp4);
		free(p_sunpos->omega);
		free(p_tmp5);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++)
	{
		p_sunpos->delta[kp] = &p_tmp5[kp * nd];
	}

	p_tmp6 = (double *) malloc(np * nd * sizeof(double));
	if (p_tmp6 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_12;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp4);
		free(p_sunpos->omega);
		free(p_tmp5);
		free(p_sunpos->delta);
		free(p_sunpos);
		return NULL;
	}
	p_sunpos->E0 = (double **) malloc(np * sizeof(double *));
	if (p_sunpos->E0 == NULL)
	{
		*p_err = SG2_ERR_CREATE_SUNPOS_MALLOC_13;
		free(p_tmp1);
		free(p_sunpos->alpha_S);
		free(p_tmp2);
		free(p_sunpos->gamma_S0);
		free(p_tmp3);
		free(p_sunpos->gamma_S);
		free(p_tmp4);
		free(p_sunpos->omega);
		free(p_tmp5);
		free(p_sunpos->delta);
		free(p_tmp6);
		free(p_sunpos);
		return NULL;
	}
	for (kp = 0; kp < np; kp++)
	{
		p_sunpos->E0[kp] = &p_tmp6[kp * nd];
	}
	return p_sunpos;
}

void sg2_delete_sunpos(S_SG2_SUNPOS *p_sunpos, int *p_err)
{
	free(p_sunpos->alpha_S[0]);
	free(p_sunpos->alpha_S);
	free(p_sunpos->gamma_S0[0]);
	free(p_sunpos->gamma_S0);
	free(p_sunpos->gamma_S[0]);
	free(p_sunpos->gamma_S);
	free(p_sunpos->omega[0]);
	free(p_sunpos->omega);
	free(p_sunpos->delta[0]);
	free(p_sunpos->delta);
	free(p_sunpos->E0[0]);
	free(p_sunpos->E0);
	free(p_sunpos);
}

S_SG2_TABTILT *sg2_create_tabtilt(unsigned long na, int *p_err)
{
	S_SG2_TABTILT *p_tilt;

	p_tilt = (S_SG2_TABTILT *) malloc(sizeof(S_SG2_TABTILT));
	if (p_tilt == NULL)
	{
		*p_err = SG2_ERR_CREATE_TABTILT_MALLOC_1;
		return NULL;
	}
	p_tilt->na = na;

	p_tilt->alpha = NULL;
	p_tilt->beta = NULL;

	p_tilt->alpha = (double *) malloc(na * sizeof(double));
	if (p_tilt->alpha == NULL)
	{
		*p_err = SG2_ERR_CREATE_TABTILT_MALLOC_2;
		sg2_delete_tabtilt(p_tilt, p_err);
	}
	p_tilt->beta = (double *) malloc(na * sizeof(double));
	if (p_tilt->beta == NULL)
	{
		*p_err = SG2_ERR_CREATE_TABTILT_MALLOC_2;
		sg2_delete_tabtilt(p_tilt, p_err);
	}

	return p_tilt;
}

void sg2_delete_tabtilt(S_SG2_TABTILT *p_tilt, int *p_err)
{
	free(p_tilt->alpha);
	free(p_tilt->beta);
	free(p_tilt);
}

void sg2_set_tabtilt(double *p_azimuth, double *p_slope, S_SG2_TABTILT *p_tilt,
		int *p_err)
{
	int na, ka;
	na = p_tilt->na;

	for (ka = 0; ka < na; ka++)
	{
		p_tilt->alpha[ka] = p_azimuth[ka] * SG2_DEG2RAD;
		p_tilt->beta[ka] = p_slope[ka] * SG2_DEG2RAD;
	}
}

S_SG2_TOA_IRRAD *sg2_create_toa_irrad(unsigned long na, unsigned long np,
		unsigned long nd, int *p_err)
{
	S_SG2_TOA_IRRAD *p_toa_irrad;
	double *p_tmp;
	double **pp_tmp;
	int ka, kp;

	p_toa_irrad = (S_SG2_TOA_IRRAD *) malloc(sizeof(S_SG2_TOA_IRRAD));
	if (p_toa_irrad == NULL)
	{
		*p_err = SG2_ERR_CREATE_TOA_IRRAD_MALLOC_1;
		return NULL;
	}
	p_toa_irrad->na = na;
	p_toa_irrad->np = np;
	p_toa_irrad->nd = nd;

	p_tmp = (double *) malloc(na * np * nd * sizeof(double));
	if (p_tmp == NULL)
	{
		*p_err = SG2_ERR_CREATE_TOA_IRRAD_MALLOC_2;
		free(p_toa_irrad);
		return NULL;
	}

	pp_tmp = (double **) malloc(np * nd * sizeof(double *));
	if (pp_tmp == NULL)
	{
		*p_err = SG2_ERR_CREATE_TOA_IRRAD_MALLOC_3;
		free(p_tmp);
		free(p_toa_irrad);
		return NULL;
	}

	p_toa_irrad->toa_dt = (double ***) malloc(na * sizeof(double **));
	if (p_toa_irrad->toa_dt == NULL)
	{
		*p_err = SG2_ERR_CREATE_TOA_IRRAD_MALLOC_4;
		free(p_tmp);
		free(pp_tmp);
		free(p_toa_irrad);
		return NULL;
	}

	for (ka = 0; ka < na; ka++)
	{
		p_toa_irrad->toa_dt[ka] = &pp_tmp[ka * np * nd];
		for (kp = 0; kp < np; kp++)
		{
			p_toa_irrad->toa_dt[ka][kp] = &p_tmp[ka * np * nd + kp * nd];
		}
	}

	p_toa_irrad->p_sunpos = sg2_create_sunpos(np, nd, p_err);
	if (p_toa_irrad->p_sunpos == NULL)
	{
		*p_err = SG2_ERR_CREATE_TOA_IRRAD_MALLOC_5;
		free(p_toa_irrad->toa_dt[0][0]);
		free(p_toa_irrad->toa_dt[0]);
		free(p_toa_irrad->toa_dt);
		free(p_toa_irrad);
		return NULL;
	}

	return p_toa_irrad;
}

void sg2_delete_toa_irrad(S_SG2_TOA_IRRAD *p_toa_irrad, int *p_err)
{
	free(p_toa_irrad->toa_dt[0][0]);
	free(p_toa_irrad->toa_dt[0]);
	free(p_toa_irrad->toa_dt);
	sg2_delete_sunpos(p_toa_irrad->p_sunpos, p_err);
	free(p_toa_irrad);
}

void sg2_set_sunpos(double *p_lon, double *p_lat, double *p_elevation,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, double *p_jd_ut,
		double *p_delta_tt, SG2_CORRECTION_REFRACTION method,
		double *p_data_corr, S_SG2_SUNPOS *p_sunpos, int *p_err)
{

	S_SG2_DATE_TABJD *p_jd;
	S_SG2_HELIOC_DATA *p_helioc;
	S_SG2_GEOC_DATA *p_geoc;
	S_SG2_TABGEOPOINT *p_gp;
	S_SG2_TOPOC_DATA *p_topoc;
	int nd,kd;
	int np,kp;
	int sz_data;

	/*
	 FILE *fp_log;

	 fp_log = fopen("c:\\temp\\sg2_log_sunpos.txt", "w");*/

	np = p_sunpos->np;
	nd = p_sunpos->nd;

	p_jd = SG2_date_create_tabjd(nd, p_err);
	memcpy(p_jd->jd_ut, p_jd_ut, nd * sizeof(double));

	SG2_date_set_tabjd_tt(NULL, p_jd, p_err);

	p_gp = SG2_topocentric_create_tabgeopoint(np, ellpstype, p_data_ellps,
			p_err);

	SG2_topocecentric_set_tabgeopoint(p_lon, p_lat, p_elevation, p_gp, p_err);

	p_helioc = SG2_heliocentric_create_helioc_data(p_jd->nd, p_err);

	SG2_heliocentric_set_helioc_data(p_jd, p_helioc, p_err);

	p_geoc = SG2_geocentric_create_geoc_data(p_jd->nd, p_err);

	SG2_geocentric_set_geoc_data(p_jd, p_helioc, p_geoc, p_err);

	p_topoc = SG2_topocentric_create_topoc_data(np, nd, p_err);

	SG2_topocentric_set_topoc_data(p_geoc, p_gp, p_topoc, p_err);

	/*
	 fprintf(fp_log, "sg2_set_sunpos :: %d, %d : set ok\n", np, nd);
	 fflush(fp_log);
	 */

	sz_data = np * nd * sizeof(double);
	memcpy(p_sunpos->alpha_S[0], p_topoc->alpha_S[0], sz_data);
	memcpy(p_sunpos->gamma_S0[0], p_topoc->gamma_S0[0], sz_data);
	memcpy(p_sunpos->omega[0], p_topoc->omega[0], sz_data);
	memcpy(p_sunpos->delta[0], p_topoc->delta[0], sz_data);
	
	for (kd = 0; kd < nd; kd++) {
		p_sunpos->E0[0][kd] = SG2_SOLAR_CONSTANT * pow(p_helioc->R[kd], -2.0);			
		for (kp = 1; kp < np; kp++) {
			p_sunpos->E0[kp][kd] = p_sunpos->E0[0][kd];			
		}
	}

	/*
	 fprintf(fp_log, "sg2_set_sunpos :: %d, %d : memcpy ok\n", np, nd);
	 fflush(fp_log);
	 */

	SG2_topocentric_correction_refraction(p_sunpos->gamma_S0[0], np * nd,
			method, p_data_corr, p_sunpos->gamma_S[0], p_err);

	p_sunpos->p_jd = p_jd;

	SG2_geocentric_delete_geoc_data(p_geoc, p_err);
	SG2_heliocentric_delete_helioc_data(p_helioc, p_err);
	SG2_topocentric_delete_tabgeopoint(p_gp, p_err);
	SG2_topocentric_delete_topoc_data(p_topoc, p_err);

	/*
	 fclose(fp_log);
	 */

}

void sg2_set_toa_irrad(double *p_lon, double *p_lat, double *p_elevation,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, double *p_jd_ut,
		double *p_delta_tt, double dt, double eta, unsigned long nts,
		double *p_azimuth, double *p_slope, S_SG2_TOA_IRRAD *p_toa_irrad,
		int *p_err)
{

	S_SG2_DATE_TABJD *p_jd;
	S_SG2_TABTILT *p_tilt;
	S_SG2_TABGEOPOINT *p_gp;
	S_SG2_DATE_TABJD *p_jd_nts;
	S_SG2_HELIOC_DATA *p_helioc_nts;
	S_SG2_GEOC_DATA *p_geoc_nts;
	S_SG2_TOPOC_DATA *p_topoc_nts;
	int ka;
	int na;
	int kd;
	int nd;
	int kp;
	int np;
	int kts;
	double dts;
	double sum_sin_gamma_S0_kp;
	double sum_cos_gamma_S0_kp;
	double *p_jd_ut_dec;

	/*FILE *fp_log;

	 fp_log = fopen("c:\\temp\\sg2_log_toa_irrad.txt", "w");
	 */

	na = p_toa_irrad->na;
	np = p_toa_irrad->np;
	nd = p_toa_irrad->nd;

	p_toa_irrad->dt = dt;
	p_toa_irrad->eta = eta;

	p_jd = SG2_date_create_tabjd(nd, p_err);
	memcpy(p_jd->jd_ut, p_jd_ut, nd * sizeof(double));
	p_toa_irrad->p_jd = p_jd;

	p_tilt = sg2_create_tabtilt(na, p_err);
	sg2_set_tabtilt(p_azimuth, p_slope, p_tilt, p_err);
	p_toa_irrad->p_tilt = p_tilt;

	p_gp = SG2_topocentric_create_tabgeopoint(np, ellpstype, p_data_ellps,
			p_err);
	SG2_topocecentric_set_tabgeopoint(p_lon, p_lat, p_elevation, p_gp, p_err);

	p_jd_ut_dec = (double *) malloc(nd * sizeof(double));
	for (kd = 0; kd < nd; kd++)
	{
		p_jd_ut_dec[kd] = p_jd_ut[kd] - eta * dt / 2.0;
	}
	sg2_set_sunpos(p_lon, p_lat, NULL, ellpstype, p_data_ellps, p_jd_ut_dec,
			p_delta_tt, SG2_CORRECTION_REFRACTION_NONE, NULL, p_toa_irrad->p_sunpos, p_err);
	free(p_jd_ut_dec);

	switch (nts)
	{
	case 0:
		*p_err = SG2_ERR_SET_TOA_IRRAD_NTSNOTVALID;
		return;
		break;
	case 1:
		break;
	default:

		dts = dt / ((double) (nts - 1));

		p_jd_nts = SG2_date_create_tabjd(nts, p_err);

		p_helioc_nts = SG2_heliocentric_create_helioc_data(nts, p_err);
		p_geoc_nts = SG2_geocentric_create_geoc_data(nts, p_err);
		p_topoc_nts = SG2_topocentric_create_topoc_data(np, nts, p_err);

		for (kd = 0; kd < nd; kd++)
		{
			p_jd_nts->jd_ut[0] = p_jd->jd_ut[kd] - eta * dt;
			for (kts = 1; kts < nts; kts++)
			{
				p_jd_nts->jd_ut[kts] = p_jd_nts->jd_ut[kts - 1] + dts;
			}
			SG2_date_set_tabjd_tt(NULL, p_jd_nts, p_err);

			SG2_heliocentric_set_helioc_data(p_jd_nts, p_helioc_nts, p_err);
			SG2_geocentric_set_geoc_data(p_jd_nts, p_helioc_nts, p_geoc_nts,
					p_err);
			SG2_topocentric_set_topoc_data(p_geoc_nts, p_gp, p_topoc_nts, p_err);

			for (kp = 0; kp < np; kp++)
			{
				sum_sin_gamma_S0_kp = 0.0;
				sum_cos_gamma_S0_kp = 0.0;
				for (kts = 0; kts < nts; kts++)
				{
					/*
					 fprintf(fp_log,
					 "sg2_set_toa_irrad (%d) :: [%d/%d, %d/%d] :: %f %f\n",
					 nts, kp, p_topoc_nts->np, kts, p_topoc_nts->nd,
					 p_topoc_nts->p_jd->jd_ut[kts],
					 sin(p_topoc_nts->gamma_S0[kp][kts]));
					 fprintf(fp_log, "%f %f\n", p_topoc_nts->p_jd->jd_ut[kts],
					 p_topoc_nts->gamma_S0[kp][kts]);
					 fflush(fp_log);
					 */

					if (p_topoc_nts->gamma_S0[kp][kts] > 0)
					{
						sum_sin_gamma_S0_kp += sin(
								p_topoc_nts->gamma_S0[kp][kts]);
						sum_cos_gamma_S0_kp += cos(
								p_topoc_nts->gamma_S0[kp][kts]);
					}
				}
				for (ka = 0; ka < na; ka++)
				{
					p_toa_irrad->toa_dt[ka][kp][kd] = SG2_SOLAR_CONSTANT * pow(
							p_helioc_nts->R[0], -2.0) * sum_sin_gamma_S0_kp
							* dts;
				}
			}

		}

		SG2_date_delete_tabjd(p_jd_nts, p_err);
		SG2_geocentric_delete_geoc_data(p_geoc_nts, p_err);
		SG2_heliocentric_delete_helioc_data(p_helioc_nts, p_err);
		SG2_topocentric_delete_topoc_data(p_topoc_nts, p_err);

		break;
	}

	SG2_topocentric_delete_tabgeopoint(p_gp, p_err);

	/*
	 fprintf(fp_log, "sg2_set_toa_irrad :: %d, %d, %d : END\n", na, np, nd);
	 fclose(fp_log);
	 */

}
