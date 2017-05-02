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

#ifndef SG2_HELIOCENTRIC_H_
#define SG2_HELIOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

void sg2_heliocentric_set_helioc_data(sg2_heliocentric_data_t * ths, time_data_t * jd, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_HELIOCENTRIC_H_ */
