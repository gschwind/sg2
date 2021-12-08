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

#include "sg2_geopoint.hxx"

namespace sg2 {

ellps const ELLPS_WGS84  = { 6378137.0, 3.352810664747481e-003 }; /* WGS84 */
ellps const ELLPS_RGF83  = { 6378137.0, 3.352810681182319e-003 }; /* RFG83 */
ellps const ELLPS_NTF    = { 6378249.2, 3.407549520015651e-003 }; /* NTF / CLARKE1880 */
ellps const ELLPS_AA     = { 6378136.6, 3.352819697896193e-003 }; /* AA */
ellps const ELLPS_SPA    = { 6378140.0, 3.352810000000000e-003 }; /* SPA */
ellps const ELLPS_NGP    = { 6378169.0, 3.384231430681783e-003 }; /* NGP*/
ellps const ELLPS_SPHERE = { 6378130.0, 0.0                    }; /* SPHERE */


} // namespace sg2


