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

struct ellps {
	double a; /* Axis a (m) */
	double f; /* Flatness (-)*/

	ellps(double a, double f);

};

extern ellps const ELLPSTYPE_WGS84;
extern ellps const ELLPSTYPE_RGF83;
extern ellps const ELLPSTYPE_NTF;
extern ellps const ELLPSTYPE_AA;
extern ellps const ELLPSTYPE_SPA;
extern ellps const ELLPSTYPE_NGP;
extern ellps const ELLPSTYPE_SPHERE;

struct geopoint {
	ellps const ellipse;
	double phi;        /* Latitude (rad) */
	double lambda;     /* Longitude (rad) */
	double h;          /* Altitude Above the Reference Ellipsoid */
	double cos_phi;
	double sin_phi;
	double u;
	double x;
	double y;

	geopoint(geopoint const &) = default;

	/**
	 * @input lat: latitude in degrees
	 * @input lon: longitude in degrees
	 * @input h: altitude in meters
	 **/
	geopoint(double lon, double lat, double h, ellps const & p_data_ellps = ELLPSTYPE_WGS84);

};

enum CORRECTION_REFRACTION {
	SG2_CORRECTION_REFRACTION_NONE = 0,
	SG2_CORRECTION_REFRACTION_SAE = 1,
	SG2_CORRECTION_REFRACTION_ZIM = 2
};

struct topocentric_data {
	geocentric_sun_position sun_position;
	geopoint point;
	double r_alpha;  /* Topocentric right sun ascension (rad) */
	double delta;    /* Topocentric sun declination (rad) */
	double omega;    /* Topocentric local hour angle (rad) */
	double gamma_S0; /* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double alpha_S;  /* Topocentric sun azimuth (rad) */
	double toa_ni;   /* irradiation at top of atmosphere normal incidence (W/m2) */
	double toa_hi;   /* irradiation at top of atmosphere horizontal incidence (W/m2) */

	topocentric_data(topocentric_data const &) = default;
	topocentric_data(geocentric_sun_position const & sun_position, geopoint const & geopoint);

	static void topocentric_correction_refraction(double *p_gamma_S0, unsigned long n,
			CORRECTION_REFRACTION method, double *p_data_corr, double *p_gamma_S);


};



} // namespace sg2

#endif /* SG2_TOPOCENTRIC_HXX_ */