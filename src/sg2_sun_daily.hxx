/*
 * Copyright 2014-2021 MINES ParisTech
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

#ifndef SG2_XX_HXX_
#define SG2_XX_HXX_

#include "sg2.hxx"

#include <limits>
#include <vector>
#include <functional>
#include <stdexcept>

namespace sg2 {

using namespace std;

/** DO not export this template if C++ is not above C++11 **/

#ifdef SG2_WIN32
static double golden_ratio = 1.6180339887498949025257388711906969547272;
#else
static double constexpr golden_ratio = (1.0 + std::sqrt(5))/2.0;
#endif

/**
 * Implement the golden-section search
 * ref: https://en.wikipedia.org/wiki/Golden-section_search
 **/
template<typename F, typename comp = std::less<double> >
double hc_find_extrema(F func, double const EPS, double left_bound, double right_bound) {
	comp _comp;
	double a, b, c, d;

	a = left_bound;
	b = right_bound;

    c = b - (b - a) / golden_ratio;
    d = a + (b - a) / golden_ratio;

	while (std::fabs(c - d) > EPS) {
		if (_comp(func(c), func(d))) {
			b = d;
		} else {
			a = c;
		}
	    c = b - (b - a) / golden_ratio;
	    d = a + (b - a) / golden_ratio;
	}

    return (a + b)/2.0;

}

template<typename F>
double hc_find_max(F func, double const EPS, double left_bound, double right_bound) {
	return hc_find_extrema<decltype(func), std::greater<double> >(func, EPS, left_bound, right_bound);
}

template<typename F>
double hc_find_min(F func, double const EPS, double left_bound, double right_bound) {
	return hc_find_extrema<decltype(func), std::less<double> >(func, EPS, left_bound, right_bound);
}


template<typename F>
double hc_find_zero(F func, double const EPS, double left_bound, double right_bound) {
    double left = left_bound;
    double right = right_bound;

    double vleft = func(left);
    double vright = func(right);
    double mid = (left_bound+right_bound)/2.0;
    double vmid = func(mid);

    if(vleft > vright) {
    	std::swap(vleft, vright);
    	std::swap(left, right);
    }

    while((right - left) > EPS) {
		if(vmid >= 0.0) {
			vright = vmid;
			right = mid;
		} else {
			vleft = vmid;
			left = mid;
		}
        mid = (left+right)/2.0;
        vmid = func(mid);
    }

    return mid;

}

class sun_daily_data {
	geopoint const & _p;
	double _sun_rise_time;
	double _sun_set_time;
	double _sun_zenit_time;
	double _sun_begin_of_day_time;
	double _sun_end_of_day_time;

public:
	sun_daily_data(geopoint const & p);
	void update(double jd);

	double get_sun_rise_time() {
		return _sun_rise_time;
	}

	double get_sun_set_time() {
		return _sun_set_time;
	}

	double get_sun_zenit_time() {
		return _sun_zenit_time;
	}

	double get_sun_begin_of_day_time() {
		return _sun_begin_of_day_time;
	}

	double get_sun_end_of_day_time() {
		return _sun_end_of_day_time;
	}

};


} // namespace

#endif /* SG2_XX_HXX_ */
