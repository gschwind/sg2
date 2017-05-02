/*
 * Copyright 2017 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
 * Copyright 2017 ARMINES
 * Copyright 2017 MINES ParisTech
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

#ifndef SRC_SG2_MATH_HXX_
#define SRC_SG2_MATH_HXX_

#ifndef SG2_HAVE_VDT

#include <cmath>

namespace sg2 {

namespace math {

inline static double sin(double x) {
	return std::sin(x);
}

inline static double cos(double x) {
	return std::cos(x);
}

inline static double tan(double x) {
	return std::tan(x);
}

inline static void sincos(double x, double & s, double & c) {
	s = std::sin(x);
	c = std::cos(x);
}

inline static double atan2(double x, double y) {
	return std::atan2(x, y);
}

inline static double asin(double x) {
	return std::asin(x);
}

inline static double atan(double x) {
	return std::atan(x);
}

}

}

#else

#include <vdt/vdtMath.h>

namespace sg2 {

namespace math {

inline static double sin(double x) {
	return vdt::fast_sin(x);
}

inline static double cos(double x) {
	return vdt::fast_cos(x);
}

inline static double tan(double x) {
	return vdt::fast_tan(x);
}

inline static void sincos(double x, double & s, double & c) {
	vdt::fast_sincos(x, s, c);
}

inline static double atan2(double x, double y) {
	return vdt::fast_atan2(x, y);
}

inline static double asin(double x) {
	return vdt::fast_asin(x);
}

inline static double atan(double x) {
	return vdt::fast_atan(x);
}

}

}

#endif


#endif /* SRC_SG2_MATH_HXX_ */
