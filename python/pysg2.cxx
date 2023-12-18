/*
 * Copyright 2014-2021 MINES ParisTech
 * Copyright 2014-2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
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

#include "python-bind-helper.hxx"

#include <datetime.h>

#include "sg2.hxx"

#include <limits>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <regex>
#include <unordered_set>

using namespace sg2;
using namespace std;


struct module_state {
	PyObject * Error;
	PyObject * types;
	PyObject * ns;
};

static sg2::ellps const * el[] = {
		&sg2::ELLPS_WGS84,
		&sg2::ELLPS_RGF83,
		&sg2::ELLPS_NTF,
		&sg2::ELLPS_AA,
		&sg2::ELLPS_SPA,
		&sg2::ELLPS_NGP,
		&sg2::ELLPS_SPHERE
};

template<typename ... Args>
void set_python_exception(Args ... args) {
	char tmp[1024];
	snprintf(tmp, 1024, args...);
	PyErr_SetString(PyExc_RuntimeError, tmp);
}


template<>
void set_python_exception(char const * str) {
	PyErr_SetString(PyExc_RuntimeError, str);
}


template<typename T>
inline static T & PyArray_Get(PyArrayObject * arr, int i)
{
	return *reinterpret_cast<T*>(PyArray_GETPTR1(arr, i));
}

template<typename T>
inline static T & PyArray_Get(PyArrayObject * arr, int i, int j)
{
	return *reinterpret_cast<T*>(PyArray_GETPTR2(arr, i, j));
}

template<typename T>
inline static T & PyArray_Get(PyArrayObject * arr, int i, int j, int k)
{
	return *reinterpret_cast<T*>(PyArray_GETPTR3(arr, i, j, k));
}

static double _pysg2_ymdh_to_jd(int64_t year, int64_t month, int64_t day_of_month, double hours)
{
	sg2::ymdh date(year, month, day_of_month, hours);
	return sg2::julian(date).value;
}

// return year, month, day and microseconds
static tuple<int, int, int, double> _pysg2_jd_to_ymdh(double jd)
{
	sg2::ymdh date(jd);
	return tuple<int, int, int, double>{date.year, date.month, date.day_of_month, date.hour};
}

static PyArray_Descr * create_datetime64_ms_dtype()
{
	// Extrapolated from numpy sources
	PyArray_Descr * dtype = PyArray_DescrNewFromType(NPY_DATETIME);
	// TODO: Check for NULL ptr.
	reinterpret_cast<PyArray_DatetimeDTypeMetaData *>(dtype->c_metadata)->meta.base = NPY_FR_ms;
	return dtype;
}

namespace { // anonymous namespace


template<typename T>
struct _map_api {
	virtual auto create(int n) -> PyArrayObject * = 0;
	virtual auto create(int n, int j) -> PyArrayObject * = 0;
	virtual void apply(PyArrayObject * dst, T const & d, int n) = 0;
	virtual void apply(PyArrayObject * dst, T const & d, int n, int j) = 0;
	virtual void clear(PyArrayObject * dst, int n) = 0;
	virtual void clear(PyArrayObject * dst, int n, int j) = 0;
};


template<typename T>
struct _map_data {
	string const & key;
	PyArrayObject * dst;
	_map_api<T> * api;
};


template<typename T, typename U, U T::*m>
struct _map_api_objmember : public _map_api<T> {

	virtual void apply(PyArrayObject * dst, T const & d, int n) override
	{
		PyArray_Get<U>(dst, n) = d.*m;
	}

	virtual void apply(PyArrayObject * dst, T const & d, int n, int j) override
	{
		PyArray_Get<U>(dst, n, j) = d.*m;
	}

};

template<typename T, double T::*m>
struct _map_api_objmember_double : public _map_api_objmember<T, double, m> {

	static _map_api<T> * _new()
	{
		static _map_api_objmember_double singleton;
		return &singleton;
	}

	virtual auto create(int n) -> PyArrayObject * override
	{
		npy_intp dims[] = {n};
		return reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_DOUBLE));
	}

	virtual auto create(int n, int j) -> PyArrayObject * override
	{
		npy_intp dims[] = {n, j};
		return reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(2, dims, NPY_DOUBLE));
	}

	virtual void clear(PyArrayObject * dst, int n) override
	{
		PyArray_Get<double>(dst, n) = std::nan("");
	}

	virtual void clear(PyArrayObject * dst, int n, int j) override
	{
		PyArray_Get<double>(dst, n, j) = std::nan("");
	}

};


template<typename T, npy_int T::*m>
struct _map_api_objmember_int : public _map_api_objmember<T, npy_int, m> {

	static _map_api<T> * _new()
	{
		static _map_api_objmember_int singleton;
		return &singleton;
	}

	virtual auto create(int n) -> PyArrayObject * override
	{
		npy_intp dims[] = {n};
		return reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(1, dims, NPY_INT));
	}

	virtual auto create(int n, int j) -> PyArrayObject * override
	{
		npy_intp dims[] = {n, j};
		return reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(2, dims, NPY_INT));
	}

	virtual void clear(PyArrayObject * dst, int n) override
	{
		PyArray_Get<int>(dst, n) = std::numeric_limits<int>::min();
	}

	virtual void clear(PyArrayObject * dst, int n, int j) override
	{
		PyArray_Get<int>(dst, n, j) = std::numeric_limits<int>::min();
	}

};


template<typename T, date T::*m>
struct _map_api_objmember_date : public _map_api_objmember<T, date, m> {

	static _map_api<T> * _new()
	{
		static _map_api_objmember_date singleton;
		return &singleton;
	}

	virtual auto create(int n) -> PyArrayObject * override
	{
		npy_intp dims[] = {n};
		return reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromDescr(1, dims, create_datetime64_ms_dtype()));
	}

	virtual auto create(int n, int j) -> PyArrayObject * override
	{
		npy_intp dims[] = {n, j};
		return reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromDescr(2, dims, create_datetime64_ms_dtype()));
	}

	virtual void apply(PyArrayObject * dst, T const & d, int n) override
	{
		PyArray_Get<npy_int64>(dst, n) = (d.*m).msec;
	}

	virtual void apply(PyArrayObject * dst, T const & d, int n, int j) override
	{
		PyArray_Get<npy_int64>(dst, n, j) = (d.*m).msec;
	}

	virtual void clear(PyArrayObject * dst, int n) override
	{
		PyArray_Get<npy_int64>(dst, n) = NPY_DATETIME_NAT;
	}

	virtual void clear(PyArrayObject * dst, int n, int j) override
	{
		PyArray_Get<npy_int64>(dst, n, j) = NPY_DATETIME_NAT;
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

	void _construct(PyObject * fields, unordered_map<string, _map_api<_type> * > const & m, regex & r)
	{
		unordered_set<string> inserted;

		PyObject * iter = PyObject_GetIter(fields);
		if (iter == nullptr)
			return;
		for(auto obj = PyIter_Next(iter); obj != nullptr; obj = PyIter_Next(iter)) {
			if(!PyUnicode_Check(obj)) {
				Py_DECREF(obj);
				continue;
			}
			auto str = PyUnicode_AsUTF8(obj);
			if (str == nullptr) {
				Py_DECREF(obj);
				continue;
			}
			cmatch cm;
			if (!regex_match(str, cm, r)) {
				Py_DECREF(obj);
				continue;
			}

			// If the field is all, insert all known fields.
			if (cm[1] == "all") {
				for (auto & x: m) {
					if (inserted.count(x.first) == 0) {
						inserted.insert(x.first);
						ref.emplace_back(_map_data<_type>{x.first, nullptr, x.second});
					}
				}
				Py_DECREF(obj);
				continue;
			}

			auto x = m.find(cm[1]);
			if (x != m.end() && inserted.count(x->first) == 0) {
				inserted.insert(x->first);
				ref.emplace_back(_map_data<_type>{x->first,nullptr,x->second});
			}
			Py_DECREF(obj);
		}
		Py_DECREF(iter);
	}

	void apply(_type const & d, int n)
	{
		for(auto & r: ref) {
			r.api->apply(r.dst, d, n);
		}
	}

	void apply(_type const & d, int n, int j)
	{
		for(auto & r: ref) {
			r.api->apply(r.dst, d, n, j);
		}
	}

	PyObject * as_object (PyObject * m)
	{
		auto ms = reinterpret_cast<module_state*>(PyModule_GetState(m));
		PyObject * o = PyObject_CallObject(ms->ns, NULL);
		for (auto & r: ref) {
			PyObject_SetAttrString(o, r.key.c_str(), reinterpret_cast<PyObject*>(r.dst));
		}
		return o;
	}

	void clear(int n, int j) {
		for(auto & r: ref) {
			r.api->clear(r.dst, n, j);
		}
	}

	~_generic_handler()
	{
		for (auto & r: ref) {
			Py_XDECREF(reinterpret_cast<PyObject*>(r.dst));
		}
	}

};


/*
 * return a pointer map the map name of arg1 (a Python list) to sg2 objects
 *
 * returned pointer must be freed.
 */
struct _geocentric : public _generic_handler<sg2::geocentric_data> {

	_geocentric(PyObject * fields, int nt)
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
		_construct(fields, m, r);

		for (auto & r: ref) {
			r.dst = r.api->create(nt);
		}

	}
};


struct _geopoint : public _generic_handler<sg2::geopoint> {

	_geopoint(PyObject * fields, int np)
	{
		static unordered_map<string, _map_api<_type> *> m = {
				{"lambda", _map_api_objmember_double<_type, &_type::lambda      >::_new()},
				{"phi",    _map_api_objmember_double<_type, &_type::phi         >::_new()},
				{"u",      _map_api_objmember_double<_type, &_type::u           >::_new()},
				{"x",      _map_api_objmember_double<_type, &_type::x           >::_new()},
				{"y",      _map_api_objmember_double<_type, &_type::y           >::_new()}
		};

		static regex r{"^gp\\.(.+)$"};
		_construct(fields, m, r);

		for (auto & r: ref) {
			r.dst = r.api->create(np);
		}

	}

};


struct _topoc : public _generic_handler<sg2::topocentric_data> {

	_topoc(PyObject * fields, int np, int nt)
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
		_construct(fields, m, r);

		for (auto & r: ref) {
			r.dst = r.api->create(np, nt);
		}

	}

};

} // anonymous namespace

static char const _doc_sun_position[] =
"Compute the Sun-Earth position (geocentric) and the local sun position (topocentric).\n"
"\n"
"Parameters\n"
"----------\n"
"\n"
"geopoints : 2D array of double\n"
"    This is a list of geopoints as 2D array of (N,3) where each row is\n"
"    repectively longitude in degrees, latitude in degrees and altitude in\n"
"    meters.\n"
"\n"
"timstamp : 2D array of double, 2D array of numpy.datetime64\n"
"    Array of timestamp at wich the computation will be done.  If the 2-D array\n"
"    is of type double, the timestamps are read as julian date (decimal number\n"
"    of continuous day from Julian Day 0, which began at noon on January 1, 4713\n"
"    B. C.).  Moreover the shape of the array can be (M,1) or (M,2) in the later\n"
"    case the row are respectively a timestamp in ut and the terrestrial time in\n"
"    ut.  In the case of (M,1) it must contain only the timestamp in ut, in that\n"
"    case the terrestrial time will be computed with a predefined piecewise\n"
"    polynomial function.\n"
"\n"
"fields : List of str.\n"
"    This is the list of needed outputs. See returns value for more details\n"
"\n"
"elipsotid : Constant, optional\n"
"    The reference elipsoid can be specified in the optional parameter ellipse and can be:\n"
"\n"
"    * WGS84 (default)\n"
"    * RGF83\n"
"    * NTF\n"
"    * AA\n"
"    * SPA\n"
"    * NGP\n"
"    * SPHERE\n"
"\n"
"    If not specified reference geoid WGS84 will be used.\n"
"\n"
"\n"
"Returns\n"
"-------\n"
"\n"
"output : Object\n"
"    The output is a python list of fields identified by python strings.  Each\n"
"    given fields will be computed and provided in the output o as follow :\n"
"    *\"field.name\"* will be *o.field.name*.  Possible fields and there output\n"
"    type are:\n"
"\n"
"    * \"geoc.ut\" **timestamp** as array of datetime64[ms]\n"
"    * \"geoc.tt\" **terrestrial time** as array of datetime64[ms]\n"
"    * \"geoc.R\" **Radius Sun-Earth** in astronomical unit (au) as array of double\n"
"    * \"geoc.L\" **Heliocentric Earth true longitude** in radians as array of double\n"
"    * \"geoc.delta\" **Geocentric declination** in radians as array of double\n"
"    * \"geoc.EOT\" **Equation of Time: difference between apparent solar time and\n"
"       mean solar time** in radians as array of double to get the time difference in hour\n"
"       you need : h = (EOT/2/PI-floor(EOT/2/PI+0.5))*12.0\n"
"    * \"geoc.Theta_a\" **Geocentric Earth true longitude** in radians as array of double\n"
"    * \"geoc.epsilon\" **Earth true obliquity** in radians as array of double\n"
"    * \"geoc.nu\" **Apparent sideral time** in radians as array of double\n"
"    * \"geoc.r_alpha\" **Geocentric right ascension** in radians as array of double\n"
"    * \"geoc.Delta_psi\" **Nutation in Geocentric Sun longitude** in radians as\n"
"      array of double\n"
"    * \"gp.lambda\" **Longitude** in radians as array of double\n"
"    * \"gp.phi\" **Latitude** in radians as array of double\n"
"    * \"gp.u\" **phi geocentric**\n"
"    * \"gp.x\"\n"
"    * \"gp.y\"\n"
"    * \"topoc.delta\" **Topocentric sun declination** in radians as array of double\n"
"    * \"topoc.alpha_S\" **Topocentric sun azimuth** in radians as array of double\n"
"    * \"topoc.gamma_S0\" **Topocentric sun elevation angle without correction of\n"
"      atm. refraction.** in radians as array of double\n"
"    * \"topoc.omega\" **Topocentric local hour angle** in radians as array of double\n"
"    * \"topoc.r_alpha\" **Topocentric right sun ascension** in radians as array of double\n"
"    * \"topoc.toa_hi\" **Top of atmosphere horizontal irradiance** in W.m^{-2} as array of double\n"
"    * \"topoc.toa_ni\" **Top of atmosphere normal irradiance** in W.m^{-2} as array of double\n"
"\n"
"    The abreviations geoc and topoc stand for respectively geocentric and topocentric.\n"
"\n"
"    Moreover the size of arrays of geoc.* fields will be (M,), the size of\n"
"    arrays of gp.* fields will be (N,) and the size of arrays of topoc.* will\n"
"    be (N,M).\n"
"\n"
"Examples\n"
"--------\n"
">>> sun_position([[45.0, 30.0, 100.0]], [2458130.0, 2458131.0], [\"topoc.gamma_S0\", \"topoc.alpha_S\"])\n"
;

/**
 * @input arg0: array(N,3) with lon, lat, alt in each row
 * @input arg1: array(M) with jd
 * @input arg2: list of outputs elements.
 * @return array(M,N,len(arg2))
 */
static PyObject * py_sun_position(PyObject * self, PyObject * args)
{
	PyArrayObject * arr0 = NULL;
	PyArrayObject * arr1 = NULL;

	try {

	PyObject * arg0 = NULL;
	PyObject * arg1 = NULL;
	PyObject * arg2 = NULL;
	int arg3 = 0;

	int warning_count = 0;
	vector<sg2::geocentric_data> geoc_list;
	vector<sg2::geopoint> geopoint_list;
	int nd0, nd1, np, nt;

	/* get args */
	if (!PyArg_ParseTuple(args, "OOO|i", &arg0, &arg1, &arg2, &arg3)) {
		set_python_exception("Invalid inputs arguments, expect 3 arguments\n");
		throw 0;
	}

	/* Get an array from any python object */
	arr0 = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OTF(arg0, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY));
	if (arr0 == NULL) {
		set_python_exception("Invalid inputs arguments, the first arguments must be an array\n");
		throw 0;
	}

	arr1 = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OF(arg1, NPY_ARRAY_IN_ARRAY));
	if (arr1 == NULL) {
		set_python_exception("Invalid inputs arguments, the second arguments must be an array\n");
		throw 0;
	}

	if (PyArray_ISINTEGER(reinterpret_cast<PyArrayObject*>(arr1))) {
		// Expected in millisecond.
		fprintf(stderr, "Warning: using integer array as input will be interpreted as datetime64[ms]\n");
		auto tmp = reinterpret_cast<PyArrayObject*>(PyArray_FromArray(arr1, PyArray_DescrFromType(NPY_INT64), NPY_ARRAY_IN_ARRAY));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
	} else if (PyArray_ISFLOAT(reinterpret_cast<PyArrayObject*>(arr1))) {
		auto tmp = reinterpret_cast<PyArrayObject*>(PyArray_FromArray(arr1, PyArray_DescrFromType(NPY_DOUBLE), NPY_ARRAY_IN_ARRAY));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
	} else if (PyArray_TYPE(arr1) == NPY_DATETIME) {
		// Ensure datetime64[ns]
		auto tmp = reinterpret_cast<PyArrayObject*>(PyObject_CallMethod(reinterpret_cast<PyObject*>(arr1), "astype", "(s)", "datetime64[ms]"));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
		// Ensure integer
		tmp = reinterpret_cast<PyArrayObject*>(PyObject_CallMethod(reinterpret_cast<PyObject*>(arr1), "astype", "(s)", "i8"));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
		tmp = reinterpret_cast<PyArrayObject*>(PyArray_FromArray(arr1, PyArray_DescrFromType(NPY_INT64), NPY_ARRAY_IN_ARRAY));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
	} else {
		set_python_exception("Invalid inputs arguments, the second arguments must be an array of int or float\n");
		throw 0;
	}

	if (arr1 == NULL) {
		set_python_exception("Invalid inputs arguments, the second arguments must be an array\n");
		throw 0;
	}

	if(!PyList_Check(arg2)) {
		set_python_exception("Invalid inputs arguments, the third arguments must be a list\n");
		throw 0;
	}

	nd0 = PyArray_NDIM(arr0);
	if (nd0 != 2) {
		set_python_exception("Input arguments rank is invalid, got %d expect 2\n", nd0);
		throw 0;
	}

	nd1 = PyArray_NDIM(arr1);
	if ((nd1 != 1) && (nd1 != 2)) {
		set_python_exception("Input arguments rank is invalid, got %u expect 1 or 2 \n", nd1);
		throw 0;
	}

	if (PyArray_DIM(arr0, 1) != 3) {
		set_python_exception("Input arguments shape is invalid, got (N,%lu) expect (N,3)\n", PyArray_DIM(arr0, 1));
		throw 0;
	}

	np = PyArray_DIM(arr0, 0);
	if (np <= 0) {
		set_python_exception("Input arguments shape is invalid, got (N,3) expect (%lu,3)\n", PyArray_DIM(arr0, 0));
		throw 0;
	}

	nt = PyArray_DIM(arr1, 0);
	if (nt <= 0) {
		set_python_exception("Input arguments shape is invalid, got (M,) expect (%lu,)\n", PyArray_DIM(arr0, 0));
		throw 0;
	}

	_geocentric geocx{arg2, nt};
	_geopoint gpx{arg2, np};
	_topoc topocx{arg2, np, nt};

	geoc_list.resize(nt);

	if (PyArray_ISFLOAT(arr1)) {
		if (nd1 == 1) {
			for (int i = 0; i < nt; ++i) {
				geoc_list[i] = sg2::geocentric_data{PyArray_Get<double>(arr1, i)};
				geocx.apply(geoc_list[i], i);
			}
		} else { // nd1 == 2
			for (int i = 0; i < nt; ++i) {
				geoc_list[i] = sg2::geocentric_data{
					PyArray_Get<double>(arr1, i, 0),
					PyArray_Get<double>(arr1, i, 1)
				};
				geocx.apply(geoc_list[i], i);
			}
		}
	} else {
		if (nd1 == 1) {
			for (int i = 0; i < nt; ++i) {
				geoc_list[i] = sg2::geocentric_data{PyArray_Get<int64_t>(arr1, i)};
				geocx.apply(geoc_list[i], i);
			}
		} else { // nd1 == 2
			for (int i = 0; i < nt; ++i) {
				geoc_list[i] = sg2::geocentric_data{
					PyArray_Get<int64_t>(arr1, i, 0),
					PyArray_Get<int64_t>(arr1, i, 1)
				};
				geocx.apply(geoc_list[i], i);
			}
		}
	}

	geopoint_list.resize(np);

	for (int i = 0; i < np; ++i) {
		double lon = PyArray_Get<double>(arr0, i, 0);
		double lat = PyArray_Get<double>(arr0, i, 1);
		double alt = PyArray_Get<double>(arr0, i, 2);
		geopoint_list[i] = sg2::geopoint{lon, lat, alt, *el[arg3]};
		gpx.apply(geopoint_list[i], i);
	}

	if (topocx.ref.size() > 0) {
		for (int p = 0; p < np; ++p) {
			for (int t = 0; t < nt; ++t) {
				auto const & geoc = geoc_list[t];
				auto const & geopoint = geopoint_list[p];
				try {
					sg2::topocentric_data topoc(geoc, geopoint);
					topocx.apply(topoc, p, t);
				} catch(int & e) {
					topocx.clear(p, t);
					warning_count += 1;
					continue;
				}
			}
		}
	}

	if(warning_count > 0)
		fprintf(stderr, "Warning: %d invalid value(s) encountered\n", warning_count);

	auto ms = reinterpret_cast<module_state*>(PyModule_GetState(self));
	PyObject * dict = PyObject_CallObject(ms->ns, NULL);
	{
		auto obj = geocx.as_object(self);
		PyObject_SetAttrString(dict, "geoc", obj);
		Py_DECREF(obj);
	}

	{
		auto obj = gpx.as_object(self);
		PyObject_SetAttrString(dict, "gp", obj);
		Py_DECREF(obj);
	}

	{
		auto obj = topocx.as_object(self);
		PyObject_SetAttrString(dict, "topoc", obj);
		Py_DECREF(obj);
	}

	Py_XDECREF(arr0); /* release reference to this arr0 */
	Py_XDECREF(arr1);
	return dict;

	} catch (...) {
		if(arr0)
			Py_XDECREF(arr0);
		if(arr1)
			Py_XDECREF(arr1);
		return NULL;
	}
}

static char const _doc_sun_rise[] =
"Compute time of sun rise, sun set and sun transit.\n"
"\n"
"Parameters\n"
"----------\n"
"\n"
"geopoint : 2D array of double\n"
"	(N,3) of double each row is lon in degrees, lat in degrees, altitude in metter.\n"
"\n"
"timespamp : 1D array of double, 1D array of numpy.datetime64\n"
"	List of requested timestamp in ut\n"
"\n"
"Returns\n"
"-------\n"
"\n"
"output : 3D array of datetime64[ms]\n"
"	(N,M,3) datetime64[ms] with repectively sun rise, sun transit and sun set\n"
"	in ut.\n"
;

static PyObject * py_sun_rise(PyObject * self, PyObject * args)
{
	PyArrayObject * arr0 = NULL;
	PyArrayObject * arr1 = NULL;

	try {

	PyObject * arg0 = NULL;
	PyObject * arg1 = NULL;


	double lon;
	double lat;
	double alt;

	int err = 0;
	int i, j;
	int warning_count = 0;
	npy_intp out_dims[3];
	PyArrayObject * out_arr;
	int nd0, nd1, np, nt;

	/* get args */
	if (!PyArg_ParseTuple(args, "OO", &arg0, &arg1)) {
		set_python_exception("Invalid inputs arguments, expect 2 arguments\n");
		throw 0;
	}

	/* Get an array from any python object */
	arr0 = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OTF(arg0, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY));
	if (arr0 == NULL) {
		set_python_exception("Invalid inputs arguments, the first arguments must be an array\n");
		throw 0;
	}

	arr1 = reinterpret_cast<PyArrayObject*>(PyArray_FROM_OF(arg1, NPY_ARRAY_IN_ARRAY));
	if (arr1 == NULL) {
		set_python_exception("Invalid inputs arguments, the second arguments must be an array\n");
		throw 0;
	}

	if (PyArray_ISINTEGER(reinterpret_cast<PyArrayObject*>(arr1))) {
		auto tmp = reinterpret_cast<PyArrayObject*>(PyArray_FromArray(arr1, PyArray_DescrFromType(NPY_INT64), NPY_ARRAY_IN_ARRAY));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
	} else if (PyArray_ISFLOAT(reinterpret_cast<PyArrayObject*>(arr1))) {
		auto tmp = reinterpret_cast<PyArrayObject*>(PyArray_FromArray(arr1, PyArray_DescrFromType(NPY_DOUBLE), NPY_ARRAY_IN_ARRAY));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
	} else if (PyArray_TYPE(arr1) == NPY_DATETIME) {
		// Ensure datetime64[ms]
		auto tmp = reinterpret_cast<PyArrayObject*>(PyObject_CallMethod(reinterpret_cast<PyObject*>(arr1), "astype", "(s)", "datetime64[ms]"));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
		// Ensure integer
		tmp = reinterpret_cast<PyArrayObject*>(PyObject_CallMethod(reinterpret_cast<PyObject*>(arr1), "astype", "(s)", "i8"));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
		tmp = reinterpret_cast<PyArrayObject*>(PyArray_FromArray(arr1, PyArray_DescrFromType(NPY_INT64), NPY_ARRAY_IN_ARRAY));
		std::swap(arr1, tmp);
		Py_XDECREF(tmp);
	} else {
		set_python_exception("Invalid inputs arguments, the second arguments must be an array of int or float\n");
		throw 0;
	}

	if (arr1 == NULL) {
		set_python_exception("Invalid inputs arguments, the second arguments must be an array\n");
		throw 0;
	}

	nd0 = PyArray_NDIM(arr0);
	if (nd0 != 2) {
		set_python_exception("Input arguments rank is invalid, got %d expect 2\n", nd0);
		throw 0;
	}

	nd1 = PyArray_NDIM(arr1);
	if ((nd1 != 1)) {
		set_python_exception("Input arguments rank is invalid, got %u expect 1 \n", nd1);
		throw 0;
	}

	if (PyArray_DIM(arr0, 1) != 3) {
		set_python_exception("Input arguments shape is invalid, got (N,%lu) expect (N,3)\n", PyArray_DIM(arr0, 1));
		throw 0;
	}

	np = PyArray_DIM(arr0, 0);
	if (np <= 0) {
		set_python_exception("Input arguments shape is invalid, got (N,3) expect (%lu,3)\n", PyArray_DIM(arr0, 0));
		throw 0;
	}

	nt = PyArray_DIM(arr1, 0);
	if (nt <= 0) {
		set_python_exception("Input arguments shape is invalid, got (M,) expect (%lu,)\n", PyArray_DIM(arr0, 0));
		throw 0;
	}

	vector<sg2::date> time_list;
	time_list.resize(nt);

	if (PyArray_ISFLOAT(arr1)) {
		for (int i = 0; i < nt; ++i) {
			time_list[i] = sg2::date{PyArray_Get<double>(arr1, i)};
		}
	} else {
		for (int i = 0; i < nt; ++i) {
			time_list[i] = sg2::date{PyArray_Get<int64_t>(arr1, i)};
		}
	}

	vector<sg2::geopoint> geopoint_list;
	geopoint_list.resize(np);

	for (int i = 0; i < np; ++i) {
		double lon = PyArray_Get<double>(arr0, i, 0);
		double lat = PyArray_Get<double>(arr0, i, 1);
		double alt = PyArray_Get<double>(arr0, i, 2);
		geopoint_list[i] = sg2::geopoint{lon, lat, alt};
	}

	npy_intp dims[] = {nt, np, 3};
	out_arr = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromDescr(3, dims, create_datetime64_ms_dtype()));

	for (int t = 0; t < nt; ++t) {
		/** TODO: maybe avoid this memcopy **/
		auto const & time = time_list[t];
		for (int p = 0; p < np; ++p) {
			/** TODO: maybe avoid this memcopy **/
			auto const & geopoint = geopoint_list[p];
			try {
				auto r = sg2::sunrise(time, geopoint);

				PyArray_Get<npy_int64>(out_arr, t, p, 0) = get<0>(r).msec;
				PyArray_Get<npy_int64>(out_arr, t, p, 1) = get<1>(r).msec;
				PyArray_Get<npy_int64>(out_arr, t, p, 2) = get<2>(r).msec;

			} catch(int & e) {

				PyArray_Get<npy_int64>(out_arr, t, p, 0) = NPY_DATETIME_NAT;
				PyArray_Get<npy_int64>(out_arr, t, p, 1) = NPY_DATETIME_NAT;
				PyArray_Get<npy_int64>(out_arr, t, p, 2) = NPY_DATETIME_NAT;

				warning_count += 1;
				continue;
			}
		}
	}

	if(warning_count > 0)
		fprintf(stderr, "Warning: %d invalid value(s) encountered\n", warning_count);


	Py_XDECREF(arr0); /* release reference to this arr0 */
	Py_XDECREF(arr1);

	return reinterpret_cast<PyObject*>(out_arr);

	} catch (...) {
		if(arr0)
			Py_XDECREF(arr0);
		if(arr1)
			Py_XDECREF(arr1);
		return NULL;
	}
}


#define TPL_FUNCTION(name) {#name, py_##name, METH_VARARGS, "Not documented"}

static PyMethodDef methods[] =
{
	{"sun_position", py_sun_position, METH_VARARGS, _doc_sun_position},
	{"sun_rise", py_sun_rise, METH_VARARGS, _doc_sun_rise},
	{NULL, NULL, 0, NULL}
};

static int pysg2_traverse(PyObject *m, visitproc visit, void *arg) {
	auto ms = reinterpret_cast<module_state*>(PyModule_GetState(m));
	Py_VISIT(ms->Error);
	Py_VISIT(ms->types);
	Py_VISIT(ms->ns);
	return 0;
}

static int pysg2_clear(PyObject *m) {
	auto ms = reinterpret_cast<module_state*>(PyModule_GetState(m));
	Py_CLEAR(ms->Error);
	Py_CLEAR(ms->types);
	Py_CLEAR(ms->ns);
	return 0;
}

static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	"sg2",
	NULL,
	sizeof(module_state),
	methods,
	NULL,
	pysg2_traverse,
	pysg2_clear,
	NULL
};


static string const _doc_ymdh_to_jd =
"Convert date in year-month-day_of_month-hours from to julian date\n"
"\n"
"Parameters\n"
"----------\n"
"\n"
"y : 1D array\n"
"	The list year numbers\n"
"\n"
"m : 1D array\n"
"	The list of month number in [1,12]\n"
"\n"
"d : 1D array\n"
"	The list of day of month in [1,31]\n"
"\n"
"h : 1D array\n"
"	array of decimal hour within the day, ex. 12:30 is 12.5\n"
"\n"
"Returns\n"
"-------\n"
"\n"
"julian_days : 1D array\n"
" List of julian day\n"
"\n"
"\n"
"Notes\n"
"-----\n"
"\n"
"Inputs array must have the same size\n"
;

static string const _doc_jd_to_ymdh =
"Convert julian day to year-month-day_of_month-hours form\n"
"\n"
"Parameters\n"
"----------\n"
"\n"
"julian_days : 1D array of double\n"
"	List of julian days to convert\n"
"\n"
"Returns\n"
"-------\n"
"\n"
"y : 1D array\n"
"	Array of year number\n"
"\n"
"m : 1D array\n"
"	Array of month number in [1,12]\n"
"\n"
"\n"
"d : 1D array\n"
"	Array of day of month in [1,31]\n"
"\n"
"h : 1D array\n"
"	Array of decimal hour within the day, ex. 12:30 is 12.5\n"
;

static string const _doc_topocentric_correction_refraction_SAE =
"Compute the atmosphere refraction using SAE correction\n"
"\n"
"Parameters\n"
"----------\n"
"\n"
"gamma_S0 : 1D array of double\n"
"	Array of gamma_S0 (solar elevation with no refraction correction), in radians\n"
"\n"
"P : 1D array of double\n"
"	Array of pressure in Pa\n"
"\n"
"T : 1D array of double\n"
"	Array of temperature in degrees Celsus\n"
"\n"
"Returns\n"
"-------\n"
"\n"
"gamma : 1D array of double\n"
"	Array of corrected solar elevation\n"
;

static string const _doc_topocentric_correction_refraction_ZIM =
"Compute the atmosphere refraction using SAE correction\n"
"\n"
"Paramters\n"
"---------\n"
"\n"
"gamma_S0 : 1D array of double\n"
"	Array of gamma_S0 (solar elevation with no refraction correction), in radians\n"
"\n"
"P : 1D array of double\n"
"	Array of pressure in Pa\n"
"\n"
"T : 1D array of double\n"
"	Array of temperature in degrees Celsus as double\n"
"\n"
"Returns\n"
"-------\n"
"\n"
"gamma : 1D array of double\n"
"	Array of corrected solar elevation\n"
;

PyMODINIT_FUNC
PyInit_sg2(void)
{
	PyObject * m = PyModule_Create(&moduledef);
	if (m == NULL)
		return NULL;

	auto ms = reinterpret_cast<module_state*>(PyModule_GetState(m));

	ms->Error = PyErr_NewException("sg2.error", NULL, NULL);
	Py_INCREF(ms->Error);
	PyModule_AddObject(m, "error", ms->Error);

	/** init numpy **/
	import_array();
	import_umath();
	PyDateTime_IMPORT;

	ms->types = PyImport_ImportModule("types");
	if (ms->types == NULL)
		return NULL;
	ms->ns = PyObject_GetAttrString(ms->types, "SimpleNamespace");
	if (ms->ns == NULL)
		return NULL;

	PyModule_AddIntConstant(m, "WGS84", 0);
	PyModule_AddIntConstant(m, "RGF83", 1);
	PyModule_AddIntConstant(m, "NTF", 2);
	PyModule_AddIntConstant(m, "AA", 3);
	PyModule_AddIntConstant(m, "SPA", 4);
	PyModule_AddIntConstant(m, "NGP", 5);;
	PyModule_AddIntConstant(m, "SPHERE", 6);

 	static python_bind_helper::build_ufunc<decltype(_pysg2_ymdh_to_jd), _pysg2_ymdh_to_jd> ufunc_ymdh_to_jd("ymdh_to_jd", _doc_ymdh_to_jd);
	ufunc_ymdh_to_jd.register_to(m);
	static python_bind_helper::build_ufunc<decltype(_pysg2_jd_to_ymdh), _pysg2_jd_to_ymdh> ufunc_jd_to_ymdh("jd_to_ymdh", _doc_jd_to_ymdh);
	ufunc_jd_to_ymdh.register_to(m);
	static python_bind_helper::build_ufunc<decltype(sg2::topocentric_correction_refraction_SAE), sg2::topocentric_correction_refraction_SAE> ufunc_topocentric_correction_refraction_SAE("topocentric_correction_refraction_SAE", _doc_topocentric_correction_refraction_SAE);
	ufunc_topocentric_correction_refraction_SAE.register_to(m);
	static python_bind_helper::build_ufunc<decltype(sg2::topocentric_correction_refraction_ZIM), sg2::topocentric_correction_refraction_ZIM> ufunc_topocentric_correction_refraction_ZIM("topocentric_correction_refraction_ZIM", _doc_topocentric_correction_refraction_ZIM);
	ufunc_topocentric_correction_refraction_ZIM.register_to(m);

	return m;

}

