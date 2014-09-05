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

#ifndef SG2_GEOCENTRIC_H_
#define SG2_GEOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

void sg2_geocentric_set_geoc_data(sg2_geocentric_data_t * ths, time_data_t * jd,
		sg2_heliocentric_data_t * p_helioc, int * err);

/**
 * Setup geocentric sun position
 *
 * @output ths: a geocentric_sun_position to be set
 * @input jd: julian day
 * @input delta_tt: delta for terrestrial time or NAN (i.e. computed)
 * @output err: first error encountered
 **/
void sg2_geocentric_set_geocentric_sun_position(sg2_geocentric_sun_position_t * ths, double jd, double delta_tt, int * err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_GEOCENTRIC_H_ */
