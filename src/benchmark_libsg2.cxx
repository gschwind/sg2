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

#include <sg2.hxx>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstdint>
#include <chrono>

void usage(void) {
	printf("usage: benchmark_libsg2\n");
}

int main(int argc, char ** argv) {

	double const jd_begin = 2457868.500000;
	double const jd_delta = 1.0/24.0/60.0;
	int const jd_count = 1440*366*10;

	double const lat = 45.0;
	double const lon = 5.0;
	double const alt = 100.0;

	if(argc != 1) {
		usage();
		return -1;
	}

	auto tic = std::chrono::steady_clock::now();

	std::vector<double> out(jd_count);
	for (int i = 0; i < jd_count; ++i) {
		double jd = jd_begin + i * 1.0/24.0/60.0;
		sg2::time_data xjd{jd};
		/** location related data **/
		sg2::geopoint_data geopoint{lon, lat, alt, sg2::ELLPS_WGS84};
		/** time related data **/
		sg2::geocentric_data geoc{xjd};
		/** local-time related data **/
		sg2::topocentric_data topoc{geoc, geopoint};
		out[i] = topoc.gamma_S0;
	}

	auto toc = std::chrono::steady_clock::now();

	std::chrono::nanoseconds diff = toc-tic;

	printf("elapsed time: %f seconds\n", diff.count()/1e9);

	return 0;

}




