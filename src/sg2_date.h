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

#ifndef SG2_DATE_H_
#define SG2_DATE_H_

#ifdef  __cplusplus
extern "C"
{
#endif

void sg2_date_tabjd_to_tabymdh(time_data_t const * p_jd, date_ymdh_t * p_ymdh,
		int *p_err);

void sg2_date_tabymdh_to_tabjd(date_ymdh_t const * p_ymdh, time_data_t *p_jd,
		int *p_err);

void sg2_date_tabymdh_to_tabydoyh(date_ymdh_t const * p_ymdh,
		date_ydoyh_t * p_ydoyh, int *p_err);

void sg2_date_tabydoyh_to_tabymdh(date_ydoyh_t const * p_ydoyh,
		date_ymdh_t * p_ymdh, int *p_err);

void sg2_date_set_time_data_tt(time_data_t * ths, double *p_delta_tt, int *err);

void sg2_julian_date_to_tabymdh(double jd, date_ymdh_t *p_ymdh);

double sg2_date_tabymdh_to_julian_date(date_ymdh_t const *p_ymdh);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_DATE_H_ */
