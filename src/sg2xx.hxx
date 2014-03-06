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
#include <vector>

using namespace std;

namespace sg2 {

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
	struct s_tabgeopt * _p;

public:
	geopoint(double lon, double lat, double h = 0.0, ellipse const & e = WGS84);

	~geopoint();

	double get_phi() {
		return _p->phi[0];
	}

	double get_lambda() {
		return _p->lambda[0];
	}

	double get_cos_phi() {
		return _p->cos_phi[0];
	}

	double get_sin_phi() {
		return _p->sin_phi[0];
	}

	double get_u() {
		return _p->u[0];
	}

	double get_x() {
		return _p->x[0];
	}

	double get_y() {
		return _p->y[0];
	}

	friend class topocentric_sun_position;

};


class geocentric_sun_position {

	struct s_date_tabjd * _jd;
	struct s_helioc_data * _helioc;
	struct s_geoc_data * _geoc;

	void _initialize();

public:
	geocentric_sun_position(double jd, double delta = 0.0, int n = 1, double * delta_tt = NULL);
	geocentric_sun_position(vector<double> & vjd, double * delta_tt = NULL);
	/** append jd from [first,last[ **/
	geocentric_sun_position(double * first, double * last, double * delta_tt = NULL);

	~geocentric_sun_position();

	/** S_SG2_DATE_TABJD **/
	double const * get_terrestrial_time() const {
		return _jd->jd_tt;
	}

	/** S_SG2_HELIOC_DATA **/
	double const * get_sun_earth_radius() const {
		return _helioc->R;
	}

	double const * get_heliocentric_true_longitude() const {
		return _helioc->L;
	}

	/** S_SG2_GEOC_DATA **/
	double const * get_delta_psi() const {
		return _geoc->Delta_psi;
	}

	double const * get_earth_true_obliquity() const {
		return _geoc->epsilon;
	}


	double const * get_geocentric_true_longitude() const {
		return _geoc->Theta_a;
	}

	double const * get_geocentric_right_ascension() const {
		return _geoc->r_alpha;
	}

	double const * get_geocentric_declination() const {
		return _geoc->delta;
	}

	double const * get_apparent_sideral_time() const {
		return _geoc->nu;
	}

	double const * get_mst_solar_time_difference() const {
		return _geoc->EOT;
	}

	friend class topocentric_sun_position;

};


class topocentric_sun_position {
	struct s_topoc_data * _topoc;

	void _initialize(geopoint const & p, geocentric_sun_position const & t);

public:
	topocentric_sun_position(geopoint const & p, geocentric_sun_position const & t);
	topocentric_sun_position(double lon, double lat, double h, double jd, double delta = 0.0, int n = 1);

	~topocentric_sun_position();

	double const * get_right_sun_ascension();
	double const * get_sun_declination();
	double const * get_omega();
	double const * get_geometric_sun_elevation();
	double const * get_geometric_azimuth();

};


}

#endif /* SG2_HXX_ */
