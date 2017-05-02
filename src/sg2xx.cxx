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

#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <cmath>

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
	set(lon, lat, h, e);
}

void geopoint::set(double lon, double lat, double h, ellipse const & e) {
	int err = 0;
	sg2_topocecentric_set_tabgeopoint(&_p, lon, lat, h, &e.e, &err);
	if(err < 0)
		throw std::runtime_error("Fail to create point_handler");
}

geopoint::~geopoint() {

}

void geocentric_sun_position::set(double jd, double delta_tt) {
	int err = 0;
	sg2_geocentric_set_geocentric_sun_position(&_sun_position, jd, delta_tt, &err);
	if (err < 0) {
		throw std::runtime_error("Fail to set sun_position");
	}
}

geocentric_sun_position::geocentric_sun_position(double jd, double delta_tt) {
	set(jd, delta_tt);
}

geocentric_sun_position::~geocentric_sun_position() {

}

void topocentric_sun_position::set(geopoint const & geopoint, geocentric_sun_position const & sun_position) {
	int err = 0;
	sg2_topocentric_set_topoc_data(&_topoc, &sun_position._sun_position, &geopoint._p, &err);
	if(err < 0) {
		throw std::runtime_error("error while cumputing topocentric data\n");
	}
}

topocentric_sun_position::topocentric_sun_position(geopoint const & p, geocentric_sun_position const & t) {
	set(p, t);
}

topocentric_sun_position::~topocentric_sun_position() {

}

sun_day_parameters::sun_day_parameters(geopoint const & p) :
	_p(p),
	_sun_rise_time(std::numeric_limits<double>::quiet_NaN()),
	_sun_set_time(std::numeric_limits<double>::quiet_NaN()),
	_sun_zenit_time(std::numeric_limits<double>::quiet_NaN())
{

}

void sun_day_parameters::update(double jd) {
	jd = std::floor(jd + 0.5); // round to the nearest integer JD
	jd += _p.get_lambda() / M_PI; // aprox UT time for the given longitude

	auto get_sun_elevation = [this](double jd) -> double {
		geocentric_sun_position geoc(jd);
		topocentric_sun_position topoc(this->_p, geoc);
		return topoc.get_geometric_sun_elevation();
	};

	_sun_zenit_time = hc_find_max(get_sun_elevation, 1e-6,
			jd - 2.0/24.0, jd + 2.0/24.0);

	double sun_lower_time = hc_find_min(get_sun_elevation, 1e-6,
			_sun_zenit_time - 14.0/24.0, _sun_zenit_time);

	_sun_rise_time = hc_find_zero(get_sun_elevation, 1e-6,
			sun_lower_time, _sun_zenit_time);

	_sun_set_time = 2*_sun_zenit_time - _sun_rise_time;

}

} // namespace


