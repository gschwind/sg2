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

namespace sg2 {

struct data_handler {
	void * data;

	data_handler();

	int64_t const & offset() const {
		return reinterpret_cast<int64_t*>(data)[0];
	}

	int64_t const & delta() const {
		return reinterpret_cast<int64_t*>(data)[1];
	}

	int64_t const & count() const {
		return reinterpret_cast<int64_t*>(data)[2];
	}

	double const & sinL(int l) const {
		return reinterpret_cast<double*>(data)[3+l*3+0];
	}

	double const & cosL(int l) const {
		return reinterpret_cast<double*>(data)[3+l*3+1];
	}

	double const & R(int l) const {
		return reinterpret_cast<double*>(data)[3+l*3+2];
	}
};

extern data_handler _geocentric_data;

}



#endif /* SRC_SG2_DATA_HANDLER_HXX_ */
