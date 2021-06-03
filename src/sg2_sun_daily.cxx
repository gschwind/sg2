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

#include "sg2_sun_daily.hxx"

#include "sg2_topocentric.hxx"

#include <stdexcept>
#include <cstdlib>

namespace sg2 {

sun_daily_data::sun_daily_data(geopoint_data const & p) :
	_p(p),
	_sun_rise_time(NAN),
	_sun_set_time(NAN),
	_sun_zenit_time(NAN),
	_sun_begin_of_day_time(NAN),
	_sun_end_of_day_time(NAN)
{

}

void sun_daily_data::update(double jd) {
	jd = std::floor(jd + 0.5); // round to the nearest integer JD
	jd -= _p.lambda / D_PI; // approximate noon UT time for the given longitude

	auto get_sun_elevation = [this](double jd) -> double {
		geocentric_data geoc{date{julian{jd}}};
		topocentric_data topoc{geoc, this->_p};
		return topoc.gamma_S0;
	};

	_sun_zenit_time = hc_find_max(get_sun_elevation, 1e-6,
			jd - 1.0/72.0, jd + 1.0/72.0);

	_sun_begin_of_day_time = hc_find_min(get_sun_elevation, 1e-6,
			_sun_zenit_time - (1.0/72.0) - 0.5, _sun_zenit_time + (1.0/72.0) - 0.5);

	_sun_end_of_day_time = hc_find_min(get_sun_elevation, 1e-6,
			_sun_zenit_time - (1.0/72.0) + 0.5, _sun_zenit_time + (1.0/72.0) + 0.5);

	// if the sun zenit is below the horizon, the sun never rise
	if(get_sun_elevation(_sun_zenit_time) < 0.0) {
		_sun_set_time = NAN;
		_sun_rise_time = NAN;
		return;
	}

	// if the sun is above the horizon at begin of the day, the sun is already rise
	if (get_sun_elevation(_sun_begin_of_day_time) < 0.0) {
		_sun_rise_time = hc_find_zero(get_sun_elevation, 1e-6,
				_sun_begin_of_day_time, _sun_zenit_time);
	} else {
		_sun_rise_time = _sun_begin_of_day_time;
	}

	// if the sun is above the horizon at begin of the day, the sun never set
	if (get_sun_elevation(_sun_end_of_day_time) < 0.0) {
		_sun_set_time = hc_find_zero(get_sun_elevation, 1e-6,
				_sun_zenit_time, _sun_end_of_day_time);
	} else {
		_sun_set_time = _sun_end_of_day_time;
	}

}

} // namespace


