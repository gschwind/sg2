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

	if (nrhs != 3)
		mexErrMsgIdAndTxt("sg2:InvalidArgumentsCount", "sg2_ymdh_to_jd need 4 arguments got %d", nrhs);

	for (int i = 0; i < 3; ++i) {
		if (mxGetClassID(prhs[i]) != mxDOUBLE_CLASS)
			mexErrMsgIdAndTxt("sg2:TypeError", "Arguments %d must be array of doubles", i);
	}

	for (int i = 0; i < 3; ++i) {
		if (mxGetM(prhs[0]) != mxGetM(prhs[i]))
			mexErrMsgIdAndTxt("sg2:SizeError", "Arguments must have the same size");
		if (mxGetN(prhs[0]) != mxGetN(prhs[i]))
			mexErrMsgIdAndTxt("sg2:SizeError", "Arguments must have the same size");
	}

	mxDouble * gamma = reinterpret_cast<mxDouble *>(mxGetData(prhs[0]));
	mxDouble * P = reinterpret_cast<mxDouble *>(mxGetData(prhs[1]));
	mxDouble * T = reinterpret_cast<mxDouble *>(mxGetData(prhs[2]));

	plhs[0] = mxCreateDoubleMatrix(mxGetM(prhs[0]), mxGetN(prhs[0]), mxREAL);
	mxDouble * gamma_corrected = reinterpret_cast<mxDouble *>(mxGetData(plhs[0]));

	int const n = mxGetM(prhs[0]) * mxGetN(prhs[0]);
	for (int i = 0; i < n; ++i) {
		gamma_corrected[i] = sg2::topocentric_correction_refraction_SAE(gamma[i], P[i], T[i]);
	}

}


