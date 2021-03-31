/*
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

extern "C" {

extern char _sg2_geocentric_data;
extern int _sg2_geocentric_data_sz;

}

namespace sg2 {


inline int64_t const & _geocentric_data_offset() {
	return reinterpret_cast<int64_t*>(&_sg2_geocentric_data)[0];
}

inline int64_t const & _geocentric_data_delta() {
	return reinterpret_cast<int64_t*>(&_sg2_geocentric_data)[1];
}

inline int64_t const & _geocentric_data_count() {
	return reinterpret_cast<int64_t*>(&_sg2_geocentric_data)[2];
}

inline double const & _geocentric_data_sinL(int l) {
	return reinterpret_cast<double*>(&_sg2_geocentric_data)[3+l*3+0];
}

inline double const & _geocentric_data_cosL(int l) {
	return reinterpret_cast<double*>(&_sg2_geocentric_data)[3+l*3+1];
}

inline double const & _geocentric_data_R(int l) {
	return reinterpret_cast<double*>(&_sg2_geocentric_data)[3+l*3+2];
}


}



#endif /* SRC_SG2_DATA_HANDLER_HXX_ */
