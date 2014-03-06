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

#ifndef SG2_TOPOCENTRIC_H_
#define SG2_TOPOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

S_SG2_ELLPS *SG2_create_user_ellipse(double a, double f);

S_SG2_TABGEOPOINT *SG2_topocentric_create_tabgeopoint(unsigned long np,
		S_SG2_ELLPS const *p_data_ellps, int *p_err);

void SG2_topocentric_delete_tabgeopoint(S_SG2_TABGEOPOINT *p_gp,
		int *p_err);

void SG2_topocecentric_set_tabgeopoint(double *lon, double *lat,
		double *h, S_SG2_TABGEOPOINT *p_gp, int *p_err);

S_SG2_TOPOC_DATA *SG2_topocentric_create_topoc_data(unsigned long np,
		unsigned long nd, int *p_err);

void SG2_topocentric_delete_topoc_data(S_SG2_TOPOC_DATA *p_topoc,
		int *p_err);

void SG2_topocentric_set_topoc_data(S_SG2_GEOC_DATA *p_geoc,
		S_SG2_TABGEOPOINT *p_gp, S_SG2_TOPOC_DATA *p_topoc, int *p_err);

void SG2_topocentric_correction_refraction(double *p_gamma_S0, unsigned long n,
		SG2_CORRECTION_REFRACTION method, double *p_data_corr, double *p_gamma_S, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_TOPOCENTRIC_H_ */
