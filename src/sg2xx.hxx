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

#ifndef SG2_HXX_
#define SG2_HXX_

#include "sg2.h"

#include <limits>
#include <vector>
#include <functional>
#include <stdexcept>

namespace sg2 {

using namespace std;

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
				throw std::runtime_error("the function is constant");
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

class ellipse {
	struct s_ellps e;

public:
	ellipse(struct s_ellps const * el);

	/** new custom ellipse **/
	ellipse(double a, double f);

	ellipse(ellipse const & x);

	~ellipse();

	friend class geopoint;

};

/** available predefine ellipses **/
extern ellipse const WGS84;
extern ellipse const RGF83;
extern ellipse const NTF;
extern ellipse const AA;
extern ellipse const SPA;
extern ellipse const NGP;
extern ellipse const SPHERE;

class geopoint {
	sg2_geopoint_t _p;

public:
	geopoint();
	geopoint(double lon, double lat, double h = 0.0, ellipse const & e = WGS84);
	void set(double lon, double lat, double h = 0.0, ellipse const & e = WGS84);
	~geopoint();

	double get_phi() {
		return _p.phi;
	}

	double get_lambda() {
		return _p.lambda;
	}

	double get_cos_phi() {
		return _p.cos_phi;
	}

	double get_sin_phi() {
		return _p.sin_phi;
	}

	double get_u() {
		return _p.u;
	}

	double get_x() {
		return _p.x;
	}

	double get_y() {
		return _p.y;
	}

	friend class topocentric_sun_position;
};


class geocentric_sun_position {
	sg2_geocentric_sun_position_t _sun_position;

public:
	void set(double jd, double delta_tt = std::numeric_limits<double>::quiet_NaN());
	geocentric_sun_position();
	geocentric_sun_position(double jd, double delta_tt = std::numeric_limits<double>::quiet_NaN());
	~geocentric_sun_position();

	double get_universal_time() const {
		return _sun_position.jd.jd_ut;
	}

	double get_terrestrial_time() const {
		return _sun_position.jd.jd_tt;
	}

	double get_sun_earth_radius() const {
		return _sun_position.helioc.R;
	}

	double get_heliocentric_true_longitude() const {
		return _sun_position.helioc.L;
	}

	double get_delta_psi() const {
		return _sun_position.geoc.Delta_psi;
	}

	double get_earth_true_obliquity() const {
		return _sun_position.geoc.epsilon;
	}

	double get_geocentric_true_longitude() const {
		return _sun_position.geoc.Theta_a;
	}

	double get_geocentric_right_ascension() const {
		return _sun_position.geoc.r_alpha;
	}

	double get_geocentric_declination() const {
		return _sun_position.geoc.delta;
	}

	double get_apparent_sideral_time() const {
		return _sun_position.geoc.nu;
	}

	double get_mst_solar_time_difference() const {
		return _sun_position.geoc.EOT;
	}

	friend class topocentric_sun_position;
};


class topocentric_sun_position {
	sg2_topocentric_data_t _topoc;

public:
	topocentric_sun_position();
	topocentric_sun_position(geopoint const & p, geocentric_sun_position const & t);
	void set(geopoint const & p, geocentric_sun_position const & t);

	~topocentric_sun_position();

	double get_right_sun_ascension() const {
		return _topoc.r_alpha;
	}

	double get_sun_declination() {
		return _topoc.delta;
	}

	double get_omega() {
		return _topoc.omega;
	}

	double get_geometric_sun_elevation() {
		return _topoc.gamma_S0;
	}

	double get_geometric_azimuth() {
		return _topoc.alpha_S;
	}

	double get_top_of_atmosphere_normal_incidence() {
		return _topoc.toa_ni;
	}

	double get_top_of_atmosphere_horizontal_incidence() {
		return _topoc.toa_hi;
	}

};


} // namespace

#endif /* SG2_HXX_ */
