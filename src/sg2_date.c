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

#define SG2_DATE_C_

#include "sg2.h"
#include "sg2_precomputed_delta_tt.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline static int SG2_date_leapyear(short year) {
	return (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));
}

void sg2_date_tabjd_to_tabymdh(time_data_t const * p_jd,
		date_ymdh_t *p_ymdh, int *p_err) {
	double H, L, N, I, J, K, jd;

	jd = p_jd->jd_ut;

	H = (jd + 0.5 - floor(jd + 0.5)) * 24.0;
	L = floor(jd + 0.5) + 68569.0;
	N = floor(4 * L / 146097.0);
	L = L - floor((146097.0 * N + 3.0) / 4.0);
	I = floor(4000 * (L + 1) / 1461001.0);
	L = L - floor(1461.0 * I / 4.0) + 31.0;

	J = floor(80.0 * L / 2447.0);
	K = L - floor(2447.0 * J / 80.0);
	L = floor(J / 11.0);
	J = J + 2.0 - 12.0 * L;
	I = 100.0 * (N - 49.0) + I + L;

	p_ymdh->year = (short) I;
	p_ymdh->month = (short) J;
	p_ymdh->day_of_month = (short) K;
	p_ymdh->hour = H;

}

/**
 * NOTE: /!\ do not fill terrestrial time, rebember to call sg2_date_set_time_data_tt afterward.
 **/
void sg2_date_tabymdh_to_tabjd(date_ymdh_t const *p_ymdh, time_data_t *p_jd,
		int *p_err) {

	int k;
	double Y, M, D, H;

	Y = p_ymdh->year;
	M = p_ymdh->month;
	D = p_ymdh->day_of_month;
	H = p_ymdh->hour;

	if (M < 3) {
		M += 12;
		Y -= 1;
	}

	p_jd->jd_ut = 1721028.0 + D + floor((153.0 * M - 2.0) / 5.0) + 365.0 * Y
			+ floor(Y / 4.0) - floor(Y / 100.0) + floor(Y / 400.0) + H / 24.0
			- 0.5;

}

void sg2_date_tabymdh_to_tabydoyh(date_ymdh_t const *p_ymdh,
		date_ydoyh_t *p_ydoyh, int *p_err) {
	int k;
	double K;

	K = 2 - SG2_date_leapyear(p_ymdh->year);
	p_ydoyh->year = p_ymdh->year;
	p_ydoyh->hour = p_ymdh->hour;
	p_ydoyh->day_of_year = (short) floor(275.0 * ((double) p_ymdh->month) / 9.0)
			- K * ((double) (p_ymdh->month > 2)) + ((double) p_ymdh->day_of_month)
			- 30.0;
}

void sg2_date_tabydoyh_to_tabymdh(date_ydoyh_t const *p_ydoyh,
		date_ymdh_t *p_ymdh, int *p_err) {
	double B;
	B = (double) SG2_date_leapyear(p_ydoyh->year);
	p_ymdh->year = p_ydoyh->year;
	p_ymdh->hour = p_ydoyh->hour;

	if (p_ydoyh->day_of_year < 32) {
		p_ymdh->month = 1;
	} else {
		p_ymdh->month = 1
				+ (short) (floor((303.0 + 5.
				* (((double) p_ydoyh->day_of_year) - 59.0 - B))/ 153.0));
	}

	if (p_ymdh->month < 2) {
		p_ymdh->day_of_month = p_ydoyh->day_of_year - 31 * (p_ymdh->month - 1);
	} else {
		p_ymdh->day_of_month = p_ydoyh->day_of_year
				- ((short) (floor((153.0 * ((double) p_ymdh->month) - 2.0) / 5.0)
						- 32.0 + B));
	}

}

void sg2_date_set_time_data_tt(time_data_t * ths, double *p_delta_tt, int *err) {

	int kd;
	int idx;

	if (p_delta_tt == NULL) {
		idx = (int) floor(
				(ths->jd_ut - SG2_precomputed_delta_tt_j0)
						/ SG2_precomputed_delta_tt_dj + 0.5);

		if ((idx < 0) || (idx > SG2_precomputed_delta_tt_nj)) {
			*err = SG2_ERR_DATE_JD_SET_JD_TT_OUTOFPERIOD;
			return;
		}
		ths->jd_tt = ths->jd_ut
				+ SG2_precomputed_delta_tt[idx] / 86400.0;

	} else {
		ths->jd_tt = ths->jd_tt + *p_delta_tt / 86400.0;
	}

}


