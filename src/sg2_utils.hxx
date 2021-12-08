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

#ifndef SRC_SG2_UTILS_HXX_
#define SRC_SG2_UTILS_HXX_

#include "sg2_typedef.hxx"
#include "sg2_math.hxx"

#include <array>

namespace sg2 {

template<int const N>
struct sinusoidal_approx {
	double j0;
	double a;
	double b;
	array<double, N> a0;
	array<double, N> ro;
	array<double, N> phi;

	double compute(double jd) const {
		double jc = jd - j0;
		double s = a*jc+b;
		for (int kf = 0; kf < N; ++kf) {
			s += ro[kf]*math::cos(a0[kf]*jc-phi[kf]);
		}
		return s;
	}

};

template<int const nP, int const S>
struct polynomial_approx {
	struct {
		int year_interval[2];
		int year_offset;
		double poly_coeff[S];
	} data[nP];

	double compute(int y) const {
		if (y < data[0].year_interval[0])
			return NAN;

		int i;
		for (i = 0;; ++i) {
			if (i >= nP)
				return NAN;
			if (y>=data[i].year_interval[1])
				break;
		}

		// Polyval
		double const x = y-data[i].year_offset;
		double s = 0.0;
		for (int k = 0; k < S; ++k) {
			s = s * x + data[i].poly_coeff[k];
		}
		return s;
	}

};

extern sinusoidal_approx<1> approx_Dpsi;
extern sinusoidal_approx<1> approx_epsilon;
extern sinusoidal_approx<0> approx_nu0;
extern sinusoidal_approx<0> approx_M_0;
extern polynomial_approx<5, 6> approx_deltat_msc;

static double const Delta_tau = -9.9337353631981704e-005;

} // namespace sg2


#endif /* SRC_SG2_UTILS_HXX_ */
