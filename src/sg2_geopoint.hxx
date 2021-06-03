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

#ifndef SRC_SG2_GEOPOINT_HXX_
#define SRC_SG2_GEOPOINT_HXX_

#include "sg2_typedef.hxx"
#include "sg2_math.hxx"

namespace sg2 {

extern ellps const ELLPS_WGS84;
extern ellps const ELLPS_RGF83;
extern ellps const ELLPS_NTF;
extern ellps const ELLPS_AA;
extern ellps const ELLPS_SPA;
extern ellps const ELLPS_NGP;
extern ellps const ELLPS_SPHERE;

struct ellps {
	double a; /* Axis a (m) */
	double f; /* Flatness (-)*/

	ellps() { }
	ellps(ellps const &) = default;
	ellps(double a, double f);

};

struct geopoint_data {
	ellps  ellipse;
	double  phi;        /* Latitude (rad) */
	double lambda;     /* Longitude (rad) */
	double h;          /* Altitude Above the Reference Ellipsoid */
	double u;          ///< phi geocentric
	double x;
	double y;

	double cos_phi_kp; ///< Usefull cache
	double sin_phi_kp; ///< Usefull cache

	geopoint_data(geopoint_data const &) = default;

	/**
	 * @input lat: latitude in degrees
	 * @input lon: longitude in degrees
	 * @input h: altitude in meters
	 **/
	geopoint_data(double lon, double lat, double h, ellps const & p_data_ellps = ELLPS_WGS84);

	auto operator=(geopoint_data const &) -> geopoint_data & = default;

	double const & get_phi() const {
		return phi;
	}

	double get_lambda() const {
		return lambda;
	}

	double get_u() const {
		return u;
	}

	double get_x() const {
		return x;
	}

	double get_y() const {
		return y;
	}

};

inline ellps::ellps(double a, double f) :
		a{a},
		f{f}
{

}

inline geopoint_data::geopoint_data(double lon, double lat, double h, ellps const & ellipse) :
	ellipse{ellipse},
	lambda{RAD(lon)},
	phi{RAD(lat)},
	h{h}
{

	double a = ellipse.a;
	double app = 1.0 - ellipse.f;

	cos_phi_kp = math::cos(phi);
	sin_phi_kp = math::sin(phi);
	double tan_phi_kp = math::tan(phi);

	double h_a_kp = h / a;
	double u_kp = math::atan(app * tan_phi_kp);
	x = math::cos(u_kp) + h_a_kp * cos_phi_kp;
	y = app * math::sin(u_kp) + h_a_kp * sin_phi_kp;
	u = u_kp;

}

} // namespace sg2

#endif /* SRC_SG2_GEOPOINT_HXX_ */
