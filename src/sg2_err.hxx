/*
 * Copyright 2011-2021 MINES ParisTech
 * Copyright 2011 Philippe Blanc <philippe.blanc@mines-paristech.fr>
 * Copyright 2014-2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#ifndef SG2_ERR_HXX_
#define SG2_ERR_HXX_

namespace sg2 {

static int const ERR_DATE_JD_SET_JD_TT_OUTOFPERIOD = -401;

static int const ERR_HELIOCENTRIC_SET_HELIOC_JDTTNOTSET = -510;
static int const ERR_HELIOCENTRIC_SET_HELIOC_OUTOFPERIOD = -511;

static int const ERR_GEOCENTRIC_SET_GEOC_JDTTNOTSET = -610;
static int const ERR_GEOCENTRIC_SET_GEOC_OUTOFPERIOD = -611;

static int const ERR_TOPOCENTRIC_CORRECTION_REFRACTION_METHOD = -720;

static int const ERR_SET_TOA_IRRAD_NTSNOTVALID = -920;

}
 // namespace sg2

#endif /* SG2_ERR_HXX_ */
