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

#define SG2_GEOCENTRIC_C_

#include "sg2.h"
#include "sg2_precomputed_heliocentric.h"
#include "sg2_precomputed_geocentric.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline static void sg2_geocentric_compute_Delta_psi(double jd_tt, double * Delta_psi, double * epsilon, int * err) {
	int idx0;
	idx0 = (int) floor(
			(jd_tt - SG2_precomputed_geocentric_Delta_psi_j0)
					/ SG2_precomputed_geocentric_Delta_psi_dj + 0.5);
	if ((idx0 < 0) || (idx0 > SG2_precomputed_geocentric_Delta_psi_nj)) {
		*err = SG2_ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD;
		*Delta_psi = NAN;
		*epsilon = NAN;
	}
	*Delta_psi = SG2_precomputed_geocentric_Delta_psi[idx0];
	*epsilon = SG2_precomputed_geocentric_epsilon[idx0];
}

void
sg2_geocentric_set_geoc_data(sg2_geocentric_data_t * ths, time_data_t * jd,
		sg2_heliocentric_data_t * helioc, int * err) {
	short idx0;
	int kd;
	double sin_Theta_a_kd, cos_epsilon_kd;
	double nu0_kd, Delta_psi_cos_epsilon_kd, M_kd;

	sg2_geocentric_compute_Delta_psi(jd->jd_tt, &(ths->Delta_psi), &(ths->epsilon), err);
	if(*err < 0)
		return;

	ths->Theta_a = helioc->L + SG2_PI + ths->Delta_psi
			+ SG2_precomputed_geocentric_Delta_tau;

	sin_Theta_a_kd = sin(ths->Theta_a);
	cos_epsilon_kd = cos(ths->epsilon);

	ths->r_alpha = atan2(sin_Theta_a_kd * cos_epsilon_kd,
			cos(ths->Theta_a));
	ths->delta = asin(sin_Theta_a_kd * sin(ths->epsilon));

	nu0_kd = 6.300388099 * jd->jd_ut - 1.539965571482657e+007;
	Delta_psi_cos_epsilon_kd = ths->Delta_psi * cos_epsilon_kd;
	M_kd = 1.720279169744191e-002 * jd->jd_tt - 4.204914238795757e+004;

	ths->nu = nu0_kd + Delta_psi_cos_epsilon_kd;
	ths->EOT = M_kd - 0.0001 - ths->r_alpha + Delta_psi_cos_epsilon_kd;
}

void
sg2_geocentric_set_geocentric_sun_position(sg2_geocentric_sun_position_t * ths, double jd, double delta_tt, int * err) {

	ths->jd.jd_ut = jd;
	if(isnan(delta_tt)) {
		sg2_date_set_time_data_tt(&(ths->jd), NULL, err);
	} else {
		sg2_date_set_time_data_tt(&(ths->jd), &delta_tt, err);
	}
	if(*err < 0) {
		return;
	}

	sg2_heliocentric_set_helioc_data(&(ths->helioc), &(ths->jd), err);
	if(*err < 0) {
		return;
	}

	sg2_geocentric_set_geoc_data(&(ths->geoc), &(ths->jd), &(ths->helioc), err);

}
