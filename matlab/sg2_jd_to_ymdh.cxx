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

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{

	if (nrhs != 1)
		mexErrMsgIdAndTxt("sg2:InvalidArgumentsCount", "sg2_ymdh_to_jd need 1 arguments got %d", nrhs);

	if (nlhs != 4)
		mexErrMsgIdAndTxt("sg2:InvalidArgumentsCount", "sg2_ymdh_to_jd need 4 return values got %d", nlhs);

	for (int i = 0; i < 1; ++i) {
		if (mxGetClassID(prhs[i]) != mxDOUBLE_CLASS)
			mexErrMsgIdAndTxt("sg2:TypeError", "Arguments %d must be array of doubles", i);
	}

	double * jd = reinterpret_cast<double *>(mxGetData(prhs[0]));

	plhs[0] = mxCreateDoubleMatrix(mxGetM(prhs[0]), mxGetN(prhs[0]), mxREAL);
	double * year = reinterpret_cast<double *>(mxGetData(plhs[0]));
	plhs[1] = mxCreateDoubleMatrix(mxGetM(prhs[0]), mxGetN(prhs[0]), mxREAL);
	double * month = reinterpret_cast<double *>(mxGetData(plhs[1]));
	plhs[2] = mxCreateDoubleMatrix(mxGetM(prhs[0]), mxGetN(prhs[0]), mxREAL);
	double * day = reinterpret_cast<double *>(mxGetData(plhs[2]));
	plhs[3] = mxCreateDoubleMatrix(mxGetM(prhs[0]), mxGetN(prhs[0]), mxREAL);
	double * hours = reinterpret_cast<double *>(mxGetData(plhs[3]));

	int const n = mxGetM(prhs[0]) * mxGetN(prhs[0]);
	for (int i = 0; i < n; ++i) {
		sg2::ymdh d{jd[i]};
		year[i] = d.year;
		month[i] = d.month;
		day[i] = d.day_of_month;
		hours[i] = d.hour;
	}

}


