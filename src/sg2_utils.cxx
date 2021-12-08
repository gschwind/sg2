/*
 * Copyright 2021 MINES ParisTech
 * Copyright 2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#include "sg2_utils.hxx"


namespace sg2 {

sinusoidal_approx<1> approx_Dpsi = {
		2444239.5, 0.0, 0.0,
		{ 2.0*PI/6791.164405 },
		{ 8.329092e-5 },
		{ -2.052757 }
};


sinusoidal_approx<1> approx_epsilon = {
		2444239.5, -6.216374e-9, 4.091383e-1,
		{ 2.0*PI/6791.164405 },
		{ 4.456183e-5 },
		{ 2.660352 }
};

sinusoidal_approx<0> approx_nu0 = {
		2444239.5, 6.300388099, 1.742079140,
		{ },
		{ },
		{ }
};

sinusoidal_approx<0> approx_M_0 = {
		2444239.5, 1.0/58.130099904, -1.399410798,
		{ },
		{ },
		{ }
};

polynomial_approx<5, 6> approx_deltat_msc = {
		{
			{{1941,1961}, 1950, {0.0, 0.0, 1.0/2547.0, -1.0/233.0, 0.407, 29.07}},
			{{1961,1986}, 1975, {0.0, 0.0, -1.0/718.0, -1.0/260.0, 1.067, 45.45}},
			{{1986,2005}, 2000, {0.00002373599, 0.000651814, 0.0017275, -0.060374, 0.3345, 63.86}},
			{{2005,2050}, 2000, {0.0, 0.0, 0.0, 0.005576068, 0.32100612, 62.8938127}},
			{{2050,2150}, 2000, {0.0, 0.0, 0.0, 1.0/312.5, 1.7148, -0.74-0.1-0.01}}
		},
};


} // namepsace sg2

