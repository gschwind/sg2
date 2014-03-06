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

#ifndef SG2_H_
#define SG2_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include "sg2_typedef.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <stdio.h>

S_SG2_SUNPOS *sg2_create_sunpos(unsigned long np, unsigned long nd,
		int *p_err);

void sg2_delete_sunpos(S_SG2_SUNPOS *p_sunpos, int *p_err);

S_SG2_TABTILT *sg2_create_tabtilt(unsigned long na, int *p_err);

void sg2_set_tabtilt(double *p_azimuth, double *p_slope,
		S_SG2_TABTILT *p_tilt, int *p_err);

void sg2_delete_tabtilt(S_SG2_TABTILT *p_tilt, int *p_err);

S_SG2_TOA_IRRAD *sg2_create_toa_irrad(unsigned long na,
		unsigned long np, unsigned long nd, int *p_err);

void sg2_delete_toa_irrad(S_SG2_TOA_IRRAD *p_toa_irrad, int *p_err);

void sg2_set_sunpos(double *p_lon, double *p_lat, double *p_elevation,
		S_SG2_ELLPS *p_data_ellps, double *p_jd_ut,
		double *p_delta_tt, SG2_CORRECTION_REFRACTION method,
		double *data_corr, S_SG2_SUNPOS *p_sunpos, int *p_err);

#ifdef	__cplusplus
}
#endif


#ifdef  __cplusplus
#include "sg2xx.hxx"
#endif

#endif /* SG2_H_ */
