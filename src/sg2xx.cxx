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


#include "sg2xx.hxx"

#include <stdexcept>

#include <cstdlib>

namespace sg2 {

ellipse const WGS84 = SG2_ELLPSTYPE_WGS84;
ellipse const RGF83 = SG2_ELLPSTYPE_RGF83;
ellipse const NTF = SG2_ELLPSTYPE_NTF;
ellipse const AA = SG2_ELLPSTYPE_AA;
ellipse const SPA = SG2_ELLPSTYPE_SPA;
ellipse const NGP = SG2_ELLPSTYPE_NGP;
ellipse const SPHERE = SG2_ELLPSTYPE_SPHERE;

/** new TYPED ellipse **/
ellipse::ellipse(struct s_ellps const * el) {
	e.a = el->a;
	e.f = el->f;
}

/** new custom ellipse **/
ellipse::ellipse(double a, double f) {
	e.a = a;
	e.f = f;
}

ellipse::ellipse(ellipse const & x) {
	if(this == &x)
		return;
	e.a = x.e.a;
	e.f = x.e.f;
}

ellipse::~ellipse() {

}

geopoint::geopoint(double lon, double lat, double h, ellipse const & e) {
	_p = 0;

	int err = 0;
	_p = SG2_topocentric_create_tabgeopoint(1, &e.e, &err);
	if(err < 0)
		throw std::runtime_error("Fail to create point_handler");
	SG2_topocecentric_set_tabgeopoint(&lon, &lat, &h, _p, &err);
	if(err < 0)
		throw std::runtime_error("Fail to create point_handler");

}

geopoint::~geopoint() {
	int err = 0;
	if (_p != 0) {
		SG2_topocentric_delete_tabgeopoint(_p, &err);
		if (err < 0)
			throw std::runtime_error("Fail to destroy geopoint");
	}
}

void geocentric_sun_position::_initialize() {
	int err = 0;
	_helioc = SG2_heliocentric_create_helioc_data(_jd->nd, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create geocentric_sun_position");

	SG2_heliocentric_set_helioc_data(_jd, _helioc, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create geocentric_sun_position");

	_geoc = SG2_geocentric_create_geoc_data(_jd->nd, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create geocentric_sun_position");

	SG2_geocentric_set_geoc_data(_jd, _helioc, _geoc, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create geocentric_sun_position");
}

geocentric_sun_position::geocentric_sun_position(double jd, double delta, int n, double * delta_tt) {
	int err = 0;

	_jd = SG2_date_create_tabjd(n, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create geocentric_sun_position");

	_jd->jd_ut[0] = jd;
	for (int kd = 1; kd < n; kd++) {
		_jd->jd_ut[kd] = _jd->jd_ut[kd - 1] + delta;
	}

	SG2_date_set_tabjd_tt(delta_tt, _jd, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create geocentric_sun_position");

	_initialize();

}

geocentric_sun_position::geocentric_sun_position(vector<double> & vjd, double * delta_tt) {
	int err = 0;

	_jd = SG2_date_create_tabjd(vjd.size(), &err);
	if (err < 0)
		throw std::runtime_error("Fail to create earth_position_handler");
	for (int kd = 0; kd < vjd.size(); kd++) {
		_jd->jd_ut[kd] = vjd[kd];
	}

	SG2_date_set_tabjd_tt(delta_tt, _jd, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create earth_position_handler");

	_initialize();
}

/** append jd from [first,last[ **/
geocentric_sun_position::geocentric_sun_position(double * first, double * last, double * delta_tt) {
	int err = 0;
	unsigned int n = last - first;

	_jd = SG2_date_create_tabjd(n, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create earth_position_handler");

	for (int kd = 0; kd < n; kd++) {
		_jd->jd_ut[kd] = first[kd];
	}

	SG2_date_set_tabjd_tt(delta_tt, _jd, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create earth_position_handler");

	_initialize();
}

geocentric_sun_position::~geocentric_sun_position() {
	int err = 0;
	if(_jd != 0) {
		SG2_date_delete_tabjd(_jd, &err);
		if(err < 0)
			throw std::runtime_error("Fail to destroy geocentric_sun_position");
	}

	if(this->_helioc != 0) {
		SG2_heliocentric_delete_helioc_data(_helioc, &err);
		if(err < 0)
			throw std::runtime_error("Fail to destroy geocentric_sun_position");
	}

	if(this->_geoc != 0) {
		SG2_geocentric_delete_geoc_data(_geoc, &err);
		if(err < 0)
			throw std::runtime_error("Fail to destroy geocentric_sun_position");
	}
}

void topocentric_sun_position::_initialize(geopoint const & p, geocentric_sun_position const & t) {
	int err = 0;
	_topoc = SG2_topocentric_create_topoc_data(1, t._jd->nd, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create local_sun_position_handler");
	SG2_topocentric_set_topoc_data(t._geoc, p._p, _topoc, &err);
	if (err < 0)
		throw std::runtime_error("Fail to create local_sun_position_handler");
}

topocentric_sun_position::topocentric_sun_position(geopoint const & p, geocentric_sun_position const & t) {
	_initialize(p, t);
}

topocentric_sun_position::topocentric_sun_position(double lon, double lat, double h, double jd, double delta, int n) {
	geopoint p(lon, lat, h);
	geocentric_sun_position t(jd, delta, n);
	_initialize(p, t);
}

topocentric_sun_position::~topocentric_sun_position() {
	int err = 0;
	if(this->_topoc != 0) {
		SG2_topocentric_delete_topoc_data(_topoc, &err);
		if(err < 0)
			throw std::runtime_error("Fail to destroy topocentric_sun_position");
	}
}



}


