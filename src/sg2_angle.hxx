/*
 * Copyright 2017 MINES ParisTech
 * Copyright 2017 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#ifndef SRC_SG2_ANGLE_HXX_
#define SRC_SG2_ANGLE_HXX_

#include <sg2_math.hxx>

#include <cassert>
#include <utility>

namespace sg2 {

class angle {
    double _angle;       ///< angle in radians
    mutable double _sin; ///< sin(_angle) cache
    mutable double _cos; ///< cos(_angle) cache

public:

    angle(double const a, double const sin = NAN, double const cos = NAN) :
    	_angle{a},
		_sin{sin},
		_cos{cos}
    {
    	assert(not std::isnan(a));
    }

    angle() { } /* keep for performace, unititialized angle are in invalid state */

    angle(angle const &) = default;
    angle & operator=(angle const &) = default;

    auto ang() const -> double {
    	return _angle;
    }

    auto rad() const -> double {
    	return _angle;
    }

    auto deg() const -> double {
    	return _angle * 180.0/PI;
    }

    auto sin() const -> double {
    	if(std::isnan(_sin))
    		_sin = math::sin(_angle);
    	return _sin;
    }

    auto cos() const -> double {
    	if(std::isnan(_cos))
    		_cos = math::cos(_angle);
    	return _cos;
    }

    auto tan() const -> double {
		if (cos() == 0.0)
			return std::signbit(sin()) ?
					-std::numeric_limits<double>::infinity() :
					std::numeric_limits<double>::infinity();
    	return sin()/cos();
    }

	auto operator-() -> angle {
		return angle{-_angle, -_sin, _cos};
	}

	void force_init_sincos() {
		sin();
		cos();
	}

	// Return PI/2 - a
	auto pi_2_minus_angle() -> angle & {
		_angle = PI / 2.0 - _angle;
		std::swap(_sin, _cos);
		return *this;
	}

	// Return a + PI
	auto angle_plus_pi() -> angle & {
		_angle = _angle + PI;
		_cos = -_cos;
		_sin = -_sin;
		return *this;
	}

	// Return a + 2*PI
	auto angle_plus_2pi() -> angle & {
		_angle = _angle + 2.0 * PI;
		return *this;
	}

	// Return a - 2*PI
	auto angle_minus_2pi() -> angle & {
		_angle = _angle - 2.0 * PI;
		return *this;
	}

};

/* some usefull functions */
namespace math {

inline static double sin(angle const & a) {
	return a.sin();
}

inline static double cos(angle const & a) {
	return a.cos();
}

inline static double tan(angle const & a) {
	return a.tan();
}

} // namespace math

} // namespace sg2

#endif /* SRC_SG2_ANGLE_HXX_ */
