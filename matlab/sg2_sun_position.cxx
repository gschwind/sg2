/*
 * Copyright 2022 MINES ParisTech
 * Copyright 2022 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#include <mex.h>
#include <matrix.h>

#include <sg2.hxx>

#include <vector>
#include <unordered_map>
#include <string>
#include <regex>
#include <unordered_set>

using namespace std;

namespace { // anonymous namespace

// This structucture is a convenient structure to avoid to constantly call
// mxGetM, mxGetData or mxGetN
struct mxArray_handler {
	mxArray const * arr;
	void * data_ptr;
	int M; // N row
	int N; // N col

	mxArray_handler(mxArray_handler const &) = default;
	mxArray_handler & operator=(mxArray_handler const &) = default;

	mxArray_handler(mxArray const * arr) : arr{arr} {
		data_ptr = mxGetData(arr);
		M = mxGetM(arr);
		N = mxGetN(arr);
	}

	template<typename T>
	T& get(int y, int x) const
	{
		return reinterpret_cast<T*>(data_ptr)[y + M*x];
	}

};


template<typename T>
struct _map_api {
	virtual auto create(int n, int j) const -> mxArray * = 0;
	virtual void apply(mxArray_handler & dst, int y, int x, T const & d) const = 0;
	virtual void clear(mxArray_handler & dst, int y, int x) const = 0;
};


template<typename T>
struct _map_data {
	string const & key;
	mxArray_handler dst;
	_map_api<T> * api;
};


template<typename T, typename U, U T::*m>
struct _map_api_objmember : public _map_api<T> {

	virtual void apply(mxArray_handler & dst, int y, int x, T const & d) const override
	{
		dst.get<U>(y, x) = d.*m;
	}

};

template<typename T, double T::*m>
struct _map_api_objmember_double : public _map_api_objmember<T, mxDouble, m> {

	static _map_api<T> * _new()
	{
		static _map_api_objmember_double singleton;
		return &singleton;
	}

	virtual auto create(int n, int j) const -> mxArray * override
	{
		return mxCreateDoubleMatrix(n, j, mxREAL);
	}

	virtual void clear(mxArray_handler & dst, int n, int j) const override
	{
		dst.get<mxDouble>(n, j) = std::nan("");
	}

};


template<typename T, sg2::date T::*m>
struct _map_api_objmember_date : public _map_api_objmember<T, sg2::date, m> {

	static _map_api<T> * _new()
	{
		static _map_api_objmember_date singleton;
		return &singleton;
	}

	virtual auto create(int n, int j) const -> mxArray * override
	{
		return mxCreateNumericMatrix(n, j, mxINT64_CLASS, mxREAL);
	}

	virtual void apply(mxArray_handler & dst, int y, int x, T const & d) const override
	{
		dst.get<mxInt64>(y, x) = (d.*m).msec;
	}

	virtual void clear(mxArray_handler & dst, int y, int x) const override
	{
		dst.get<mxInt64>(y, x) = std::numeric_limits<mxInt64>::min();
	}

};


/*
 * return a pointer map the map name of arg1 (a Python list) to sg2 objects
 *
 * returned pointer must be freed.
 */
template<typename T>
struct _generic_handler {
	using _type = T;
	vector<_map_data<_type>> ref;

	void _construct(mxArray const * fields, unordered_map<string, _map_api<_type> * > const & m, regex & r, int n_row, int n_col)
	{
		unordered_set<string> inserted;
		int nfields = mxGetN(fields);
		for (int i = 0; i < nfields; ++i) {
			mxArray * cell = mxGetCell(fields, i);
			char fields_value[100];
			mxGetString(cell, fields_value, 100);
			cmatch cm;
			if (!regex_match(fields_value, cm, r)) {
				continue;
			}

			// If the field is all, insert all known fields.
			if (cm[1] == "all") {
				for (auto & x: m) {
					if (inserted.count(x.first) == 0) {
						inserted.insert(x.first);
						ref.emplace_back(_map_data<_type>{x.first, x.second->create(n_row, n_col), x.second});
					}
				}
				continue;
			}

			auto x = m.find(cm[1]);
			if (x != m.end() && inserted.count(x->first) == 0) {
				inserted.insert(x->first);
				ref.emplace_back(_map_data<_type>{x->first, x->second->create(n_row, n_col), x->second});
			}
		}
	}

	void apply(int y, int x, _type const & d)
	{
		for(auto & r: ref) {
			r.api->apply(r.dst, y, x, d);
		}
	}

	mxArray * as_object ()
	{
		vector<char const *> field_def;
		for (auto & r: ref) {
			field_def.push_back(r.key.c_str());
		}

		mxArray * o = mxCreateStructMatrix(1, 1, field_def.size(), &field_def[0]);
		for (int i = 0; i < ref.size(); ++i) {
			mxSetFieldByNumber(o, 0, i, const_cast<mxArray *>(ref[i].dst.arr));
		}
		return o;
	}

	void clear(int y, int x) {
		for(auto & r: ref) {
			r.api->clear(r.dst, y, x);
		}
	}

	~_generic_handler()
	{

	}

};


/*
 * return a pointer map the map name of arg1 (a Python list) to sg2 objects
 *
 * returned pointer must be freed.
 */
struct _geocentric : public _generic_handler<sg2::geocentric_data> {

	_geocentric(mxArray const * fields, int nt)
	{
		static unordered_map<string, _map_api<_type> * > const m = {
				{"ut",       _map_api_objmember_date  <_type, &_type::ut        >::_new()},
				{"tt",       _map_api_objmember_date  <_type, &_type::tt        >::_new()},
				{"R",        _map_api_objmember_double<_type, &_type::R         >::_new()},
				{"L",        _map_api_objmember_double<_type, &_type::L         >::_new()},
				{"delta",    _map_api_objmember_double<_type, &_type::delta     >::_new()},
				{"EOT",      _map_api_objmember_double<_type, &_type::EOT       >::_new()},
				{"Theta_a",  _map_api_objmember_double<_type, &_type::Theta_a   >::_new()},
				{"epsilon",  _map_api_objmember_double<_type, &_type::epsilon   >::_new()},
				{"nu",       _map_api_objmember_double<_type, &_type::nu        >::_new()},
				{"r_alpha",  _map_api_objmember_double<_type, &_type::r_alpha   >::_new()},
				{"Delta_psi",_map_api_objmember_double<_type, &_type::Delta_psi >::_new()}
		};

		static regex r{"^geoc\\.(.+)$"};
		_construct(fields, m, r, 1, nt);

	}
};


struct _geopoint : public _generic_handler<sg2::geopoint> {

	_geopoint(mxArray const * fields, int np)
	{
		static unordered_map<string, _map_api<_type> *> m = {
				{"lambda", _map_api_objmember_double<_type, &_type::lambda      >::_new()},
				{"phi",    _map_api_objmember_double<_type, &_type::phi         >::_new()},
				{"u",      _map_api_objmember_double<_type, &_type::u           >::_new()},
				{"x",      _map_api_objmember_double<_type, &_type::x           >::_new()},
				{"y",      _map_api_objmember_double<_type, &_type::y           >::_new()}
		};

		static regex r{"^gp\\.(.+)$"};
		_construct(fields, m, r, 1, np);

	}

};


struct _topoc : public _generic_handler<sg2::topocentric_data> {

	_topoc(mxArray const * fields, int np, int nt)
	{
		static unordered_map<string, _map_api<_type> *> m = {
				{"delta",       _map_api_objmember_double<_type, &_type::delta         >::_new()},
				{"alpha_S",     _map_api_objmember_double<_type, &_type::alpha_S       >::_new()},
				{"gamma_S0",    _map_api_objmember_double<_type, &_type::gamma_S0      >::_new()},
				{"omega",       _map_api_objmember_double<_type, &_type::omega         >::_new()},
				{"r_alpha",     _map_api_objmember_double<_type, &_type::r_alpha       >::_new()},
				{"toa_hi",      _map_api_objmember_double<_type, &_type::toa_hi        >::_new()},
				{"toa_ni",      _map_api_objmember_double<_type, &_type::toa_ni        >::_new()}
		};

		static regex r{"^topoc\\.(.+)$"};
		_construct(fields, m, r, np, nt);

	}

};

} // anonymous namespace


static inline bool mxIsInteger(mxArray * a)
{
	auto c = mxGetClassID(a);
	return (c==mxUINT8_CLASS)
		|| (c==mxUINT16_CLASS)
		|| (c==mxUINT32_CLASS)
		|| (c==mxUINT32_CLASS)
	    || (c==mxINT8_CLASS)
		|| (c==mxINT16_CLASS)
		|| (c==mxINT32_CLASS)
		|| (c==mxINT32_CLASS);
}

static inline bool mxIsFloating(mxArray * a)
{
	auto c = mxGetClassID(a);
	return (c==mxDOUBLE_CLASS)
		|| (c==mxSINGLE_CLASS);
}

template<typename T>
static inline void read_time(mxArray const * in, vector<sg2::geocentric_data> & geoc_list, _geocentric & geocx, int nt)
{
	mxArray_handler arr{in};
	if (mxGetM(in) == 1) {
		for (int i = 0; i < nt; ++i) {
			geoc_list[i] = sg2::geocentric_data{arr.get<T>(0, i)};
			geocx.apply(0, i, geoc_list[i]);
		}
	} else { // nd1 == 2
		for (int i = 0; i < nt; ++i) {
			geoc_list[i] = sg2::geocentric_data{
				arr.get<T>(0, i),
				arr.get<T>(1, i)
			};
			geocx.apply(0, i, geoc_list[i]);
		}
	}
}


void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{

	double * arr0 = NULL;
	double * arr1 = NULL;

	try {

	double * arg0 = NULL;
	int arg3 = 0;

	int err = 0;
	int i, j;
	int warning_count = 0;
	vector<sg2::geocentric_data> geoc_list;
	vector<sg2::geopoint> geopoint_list;
	int nd0, nd1, np, nt;

	/* get args */
	if (nrhs != 3) {
		mexErrMsgIdAndTxt("sg2:InvalidArgumentCount", "Invalid inputs arguments, expect 3 arguments");
	}

	/* Get an array from any python object */
	arr0 = mxGetPr(prhs[0]);
	if (arr0 == NULL) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid inputs arguments, the first arguments must be an array");
	}

	arr1 = mxGetPr(prhs[1]);
	if (arr1 == NULL) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid inputs arguments, the second arguments must be an array");
	}

	if (arr1 == NULL) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid inputs arguments, the second arguments must be an array");
	}

	if(!mxIsCell(prhs[2])) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid inputs arguments, the third arguments must be a list");
	}

	nd0 = mxGetNumberOfDimensions(prhs[0]);
	if (nd0 != 2) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Input arguments rank is invalid, got %d expect 2", nd0);
	}

	nd1 = mxGetNumberOfDimensions(prhs[1]);
	if ((nd1 != 1) && (nd1 != 2)) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Input arguments rank is invalid, got %u expect 1 or 2", nd1);
	}

	if (mxGetN(prhs[0]) != 3) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Input arguments shape is invalid, got (N,%lu) expect (N,3)", mxGetN(prhs[0]));
	}

	np = mxGetM(prhs[0]);
	if (np <= 0) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Input arguments shape is invalid, got (N,3) expect (%d,3)\n", np);
	}

	nt = mxGetN(prhs[1]);
	if (nt <= 0) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Input arguments shape is invalid, got (M,1) expect (1,%d)\n", nt);
	}

	if (mxGetClassID(prhs[0]) != mxDOUBLE_CLASS) {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid type for the first argument location, expected double\n");
	}

	_geocentric geocx{prhs[2], nt};
	_geopoint gpx{prhs[2], np};
	_topoc topocx{prhs[2], np, nt};

	geoc_list.resize(nt);

	if (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS) {
		read_time<mxDouble>(prhs[1], geoc_list, geocx, nt);
	} else if (mxGetClassID(prhs[1]) == mxINT64_CLASS) {
		read_time<mxInt64>(prhs[1], geoc_list, geocx, nt);
	} else {
		mexErrMsgIdAndTxt("sg2:InvalidArgument", "Invalid type for time argument, expected int64 or double\n");
	}

	geopoint_list.resize(np);

	{
		mxArray_handler arr0{prhs[0]};
		for (int i = 0; i < np; ++i) {
			double lon = arr0.get<mxDouble>(i, 0);
			double lat = arr0.get<mxDouble>(i, 1);
			double alt = arr0.get<mxDouble>(i, 2);
#ifndef USE_SPA_ELLPS
			geopoint_list[i] = sg2::geopoint{lon, lat, alt, sg2::ELLPS_WGS84};
#else
			geopoint_list[i] = sg2::geopoint{lon, lat, alt, sg2::ELLPS_SPA};
#endif
			gpx.apply(0, i, geopoint_list[i]);
		}
	}

	if (topocx.ref.size() > 0) {
		for (int p = 0; p < np; ++p) {
			for (int t = 0; t < nt; ++t) {
				auto const & geoc = geoc_list[t];
				auto const & geopoint = geopoint_list[p];
				try {
					sg2::topocentric_data topoc(geoc, geopoint);
					topocx.apply(p, t, topoc);
				} catch(int & e) {
					topocx.clear(p, t);
					warning_count += 1;
					continue;
				}
			}
		}
	}

	if(warning_count > 0)
		mexPrintf("Warning: %d invalid value(s) encountered\n", warning_count);

	char const * xfields[] ={"geoc", "gp", "topoc"};
	mxArray * dict = mxCreateStructMatrix(1, 1, 3, xfields);
	{
		auto obj = geocx.as_object();
		mxSetFieldByNumber(dict, 0, 0, obj);
	}

	{
		auto obj = gpx.as_object();
		mxSetFieldByNumber(dict, 0, 1, obj);
	}

	{
		auto obj = topocx.as_object();
		mxSetFieldByNumber(dict, 0, 2, obj);
	}

	plhs[0] = dict;

	} catch (...) {
		mexErrMsgIdAndTxt("sg2:UnknownError", "Unknown exception occurred in sg2_sun_position");
	}

}
