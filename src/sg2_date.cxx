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

#include "sg2_date.hxx"
#include "sg2_err.hxx"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "sg2_precomputed_delta_tt.hxx"

namespace sg2 {

void time_data::_init_jd_tt() {
	// round((jd_ut-SG2_precomputed_delta_tt_j0)/SG2_precomputed_delta_tt_dj)
	int idx = (jd_ut.nsec - SG2_precomputed_delta_tt_j0
			+ (SG2_precomputed_delta_tt_dj/2))
			/ SG2_precomputed_delta_tt_dj;

	if ((idx < 0) || (idx > SG2_precomputed_delta_tt_nj)) {
		throw ERR_DATE_JD_SET_JD_TT_OUTOFPERIOD;
	}

	jd_tt = jd_ut + static_cast<int64_t>(SG2_precomputed_delta_tt[idx] * 1000000000.0);
}


}
