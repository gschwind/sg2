/*
 * Copyright 2014 MINES ParisTech
 * Copyright 2014 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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


/**
 * Find the local extrema of the function 'func'
 * between left_bound and right_bound.
 * by default try to find the minimum
 **/
template<typename F, typename comp = std::less<double> >
double hc_find_extrema(F func, double const EPS, double left_bound, double right_bound) {
	comp _comp;

    double left = left_bound;
    double right = right_bound;
    double mid = (left_bound+right_bound)/2.0;

    double vleft = func(left);
    double vmid = func(mid);
    double vright = func(right);

    while((right - left) > EPS) {
        double mleft = (mid+left)/2.0;
        double mright = (mid+right)/2.0;

    	double vmleft = func(mleft);
		double vmright = func(mright);

		if(_comp(vmleft, vleft) and _comp(vmleft, vmid) and _comp(vmleft, vmright) and _comp(vmleft, vright)) {
				vright = vmid;
				right = mid;
				vmid = vmleft;
				mid = mleft;
		} else if(_comp(vmid, vleft) and _comp(vmid, vmleft) and _comp(vmid, vmright) and _comp(vmid, vright)) {
				vleft = vmleft;
				left = mleft;
				vright = vmright;
				right = mright;
		} else if(_comp(vmright, vleft) and _comp(vmright, vmleft) and _comp(vmright, vmid) and _comp(vmright, vright)) {
				vleft = vmid;
				left = mid;
				vmid = vmright;
				mid = mright;
		} else {
				throw std::runtime_error("Unexpected function shape");
		}
    }

    return mid;

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
	geopoint_data const & _p;
	double _sun_rise_time;
	double _sun_set_time;
	double _sun_zenit_time;

public:
	sun_daily_data(geopoint_data const & p);
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

};


} // namespace

#endif /* SG2_XX_HXX_ */
