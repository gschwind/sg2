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

#ifndef SG2_TOPOCENTRIC_HXX_
#define SG2_TOPOCENTRIC_HXX_

#include "sg2_typedef.hxx"
#include "sg2_geocentric.hxx"

namespace sg2 {

extern ellps const ELLPSTYPE_WGS84;
extern ellps const ELLPSTYPE_RGF83;
extern ellps const ELLPSTYPE_NTF;
extern ellps const ELLPSTYPE_AA;
extern ellps const ELLPSTYPE_SPA;
extern ellps const ELLPSTYPE_NGP;
extern ellps const ELLPSTYPE_SPHERE;

struct ellps {
	double a; /* Axis a (m) */
	double f; /* Flatness (-)*/

	ellps() { }
	ellps(ellps const &) = default;
	ellps(double a, double f);

};

struct geopoint_data {
	ellps  ellipse;
	double phi;        /* Latitude (rad) */
	double lambda;     /* Longitude (rad) */
	double h;          /* Altitude Above the Reference Ellipsoid */
	double cos_phi;
	double sin_phi;
	double u;
	double x;
	double y;

	geopoint_data() { }
	geopoint_data(geopoint_data const &) = default;

	/**
	 * @input lat: latitude in degrees
	 * @input lon: longitude in degrees
	 * @input h: altitude in meters
	 **/
	geopoint_data(double lon, double lat, double h, ellps const & p_data_ellps = ELLPSTYPE_WGS84);

	auto operator=(geopoint_data const &) -> geopoint_data & = default;

	double get_phi() const {
		return phi;
	}

	double get_lambda() const {
		return lambda;
	}

	double get_cos_phi() const {
		return cos_phi;
	}

	double get_sin_phi() const {
		return sin_phi;
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

enum correction_refraction_e {
	REFRACTION_NONE = 0,
	REFRACTION_SAE = 1,
	REFRACTION_ZIM = 2
};

struct topocentric_data {
	double r_alpha;  /* Topocentric right sun ascension (rad) */
	double delta;    /* Topocentric sun declination (rad) */
	double omega;    /* Topocentric local hour angle (rad) */
	double gamma_S0; /* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double alpha_S;  /* Topocentric sun azimuth (rad) */
	double toa_ni;   /* irradiation at top of atmosphere normal incidence (W/m2) */
	double toa_hi;   /* irradiation at top of atmosphere horizontal incidence (W/m2) */

	topocentric_data() { }
	topocentric_data(topocentric_data const &) = default;
	topocentric_data(geocentric_data const & geoc, geopoint_data const & geopoint);

	auto operator=(topocentric_data const &) -> topocentric_data & = default;

	double topocentric_correction_refraction(double const P, double const T, correction_refraction_e method = REFRACTION_NONE) const;

	/* Topocentric right sun ascension (rad) */
	double get_right_sun_ascension() const {
		return r_alpha;
	}

	/* Topocentric sun declination (rad) */
	double get_sun_declination() const {
		return delta;
	}

	/* Topocentric local hour angle (rad) */
	double get_omega() const {
		return omega;
	}

	/* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double get_geometric_sun_elevation() const {
		return gamma_S0;
	}

	/* Topocentric sun azimuth (rad) */
	double get_geometric_azimuth() const {
		return alpha_S;
	}

	/* irradiation at top of atmosphere normal incidence (W/m2) */
	double get_top_of_atmosphere_normal_incidence() const {
		return toa_ni;
	}

	/* irradiation at top of atmosphere horizontal incidence (W/m2) */
	double get_top_of_atmosphere_horizontal_incidence() const {
		return toa_hi;
	}

};



} // namespace sg2

#endif /* SG2_TOPOCENTRIC_HXX_ */
