/*
 * Copyright 2022 MINES ParisTech
 * Copyright 2022-2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

// Matlab headers
#include <mex.h>
#include <matrix.h>

#include <sg2.hxx>

#include <vector>
#include <unordered_map>
#include <string>
#include <regex>

using namespace std;

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{

	/* get args */
	if (nrhs != 2) {
		mexErrMsgIdAndTxt("sg2:InvalidArgumentCount", "Invalid inputs arguments, expect 3 arguments");
	}

	/* Get an array from any python object */
	auto arr0 = reinterpret_cast<mxDouble*>(mxGetData(prhs[0]));
	if (arr0 == NULL) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid inputs arguments, the first arguments must be an array");
	}

	auto arr1 = reinterpret_cast<double*>(mxGetData(prhs[1]));
	if (arr1 == NULL) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid inputs arguments, the second arguments must be an array");
	}

	if (mxGetClassID(prhs[0]) != mxDOUBLE_CLASS) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid type for the first argument location, expected double\n");
	}

	if (mxGetNumberOfDimensions(prhs[0]) != 2) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Location size must have 2 dimensions got %d dimensions\n", mxGetNumberOfDimensions(prhs[0]));
	}

	if (mxGetN(prhs[0]) != 3) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Location size must be (M, 3) got (M, %d)\n", mxGetN(prhs[0]));
	}

	int const nt = mxGetM(prhs[1])*mxGetN(prhs[1]);
	vector<sg2::date> time;
	time.resize(nt);
	if (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS) {
		mxDouble * data = reinterpret_cast<mxDouble *>(mxGetData(prhs[1]));
		for (int i = 0; i < nt; ++i) {
			time[i] = sg2::date{data[i]};
		}
	} else if (mxGetClassID(prhs[1]) == mxINT64_CLASS) {
		mxInt64 * data = reinterpret_cast<mxInt64 *>(mxGetData(prhs[1]));
		for (int i = 0; i < nt; ++i) {
			time[i] = sg2::date{data[i]};
		}
	} else {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid type for time argument, expected int64 or double\n");
	}


	vector<sg2::geopoint> geopoint_list;
	int const np = mxGetM(prhs[0]);
	geopoint_list.resize(np);

	for (int i = 0; i < np; ++i) {
		double lon = arr0[i + np*0];
		double lat = arr0[i + np*1];
		double alt = arr0[i + np*2];
		geopoint_list[i] = sg2::geopoint{lon, lat, alt, sg2::ELLPS_WGS84};
	}

	mwSize const dims[3] = {np, nt, 3};
	plhs[0] = mxCreateNumericArray(3, dims, mxINT64_CLASS, mxREAL);
	mxInt64 * out = reinterpret_cast<mxInt64 *>(mxGetData(plhs[0]));

	for (int i = 0; i < np; ++i) {
		for (int j = 0; j < nt; ++j) {
			try {
				sg2::date sunrise, transit, sunset;
				std::tie(sunrise, transit, sunset) = sg2::sunrise(time[j], geopoint_list[i]);
				out[i + dims[0]*j + dims[0]*dims[1]*0] = sunrise.msec;
				out[i + dims[0]*j + dims[0]*dims[1]*1] = transit.msec;
				out[i + dims[0]*j + dims[0]*dims[1]*2] = sunset.msec;
			} catch (...) {
				out[i + dims[0]*j + dims[0]*dims[1]*0] = std::numeric_limits<int64_t>::max();
				out[i + dims[0]*j + dims[0]*dims[1]*1] = std::numeric_limits<int64_t>::max();
				out[i + dims[0]*j + dims[0]*dims[1]*2] = std::numeric_limits<int64_t>::max();
			}
		}
	}

}


