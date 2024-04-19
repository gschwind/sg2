/*
 * Copyright 2021 MINES ParisTech
 * Copyright 2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#ifndef SRC_SG2_DATA_HANDLER_HXX_
#define SRC_SG2_DATA_HANDLER_HXX_

#include <cstdint>

namespace sg2 {

struct geocentric_data_format {
	double sinL;
	double cosL;
	double R;
};

extern int64_t const _geocentric_data_time_offset;
extern int64_t const _geocentric_data_time_delta;
extern int64_t const _geocentric_data_xcount;

extern geocentric_data_format const _geocentric_data[];

inline int64_t const & _geocentric_data_offset() {
	return _geocentric_data_time_offset;
}

inline int64_t const & _geocentric_data_delta() {
	return _geocentric_data_time_delta;
}

inline int64_t const & _geocentric_data_count() {
	return _geocentric_data_xcount;
}

inline double const & _geocentric_data_sinL(int l) {
	return _geocentric_data[l].sinL;
}

inline double const & _geocentric_data_cosL(int l) {
	return _geocentric_data[l].cosL;
}

inline double const & _geocentric_data_R(int l) {
	return _geocentric_data[l].R;
}


}



#endif /* SRC_SG2_DATA_HANDLER_HXX_ */
