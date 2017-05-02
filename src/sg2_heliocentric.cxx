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

#define SG2_HELIOCENTRIC_C_

#include "sg2.h"
#include "sg2_precomputed_heliocentric.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline static double sg2_heliocentric_compute_R(double jd_tt, int * p_err) {
	int idx0;
	double x, x0, dx;
	x = (jd_tt - SG2_precomputed_heliocentric_R_j0)
			/ SG2_precomputed_heliocentric_R_dj;
	x0 = floor(x);
	dx = x - x0;

	idx0 = (short) x0;
	if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_R_nj - 1)) {
		*p_err = SG2_ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
		return NAN;
	}
	return (1.0 - dx) * SG2_precomputed_heliocentric_R[idx0]
			+ dx * SG2_precomputed_heliocentric_R[idx0 + 1];
}

inline static double sg2_heliocentric_compute_L(double jd_tt, int * p_err) {
	int idx0;
	double x, x0, dx;
	x = (jd_tt - SG2_precomputed_heliocentric_L_j0)
			/ SG2_precomputed_heliocentric_L_dj;
	x0 = floor(x);
	dx = x - x0;

	idx0 = (int) x0;
	if ((idx0 < 0) || (idx0 > SG2_precomputed_heliocentric_L_nj - 1)) {
		*p_err = SG2_ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD;
		return NAN;
	}

	return (1.0 - dx) * SG2_precomputed_heliocentric_L[idx0]
			+ dx * SG2_precomputed_heliocentric_L[idx0 + 1];
}


void sg2_heliocentric_set_helioc_data(sg2_heliocentric_data_t * ths,
		time_data_t * jd, int *p_err) {
	short idx0;
	double x, x0, dx;
	int k;

	ths->R = sg2_heliocentric_compute_R(jd->jd_tt, p_err);
	if(*p_err < 0)
		return;

	ths->L = sg2_heliocentric_compute_L(jd->jd_tt, p_err);
	if(*p_err < 0)
		return;

}



