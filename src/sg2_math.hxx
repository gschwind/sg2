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

#ifndef SRC_SG2_MATH_HXX_
#define SRC_SG2_MATH_HXX_

#include <tuple>

namespace sg2 {

template<int N, bool ODD>
struct _ipow;

template<int N>
struct _ipow<N, true> {
	static inline constexpr double get(double const x) {
		return _ipow<N-1, false>::get(x)*x;
	}
};

template<int N>
struct _ipow<N, false> {
	static inline constexpr double get(double const x) {
		return _ipow<N/2,(N/2)%2!=0>::get(x)*_ipow<N/2,(N/2)%2!=0>::get(x);
	}
};

template<>
struct _ipow<2, false> {
	static inline constexpr double get(double const x) {
		return x*x;
	}
};


template<>
struct _ipow<1, true> {
	static inline constexpr double get(double const x) {
		return x;
	}
};


template<int N>
inline constexpr double ipow(double const x)
{
	return _ipow<N,N%2!=0>::get(x);
}

template<int N, typename ... ARGS>
struct _polyval {
	static inline constexpr double get(double x, std::tuple<ARGS...> const & t)
	{
		return x*_polyval<N-1, ARGS...>::get(x, t)+std::get<N>(t);
	}
};

template<typename ... ARGS>
struct _polyval<0, ARGS...> {
	static inline constexpr double get(double x, std::tuple<ARGS...> const & t)
	{
		return std::get<0>(t);
	}
};


template<typename ... ARGS>
inline constexpr double polyval(double x, ARGS ... args)
{
	return _polyval<sizeof...(ARGS)-1, ARGS...>::get(x, std::tuple<ARGS...>{args...});
}

} // namespace sg2

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
