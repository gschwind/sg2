#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from distutils.core import setup, Extension
from subprocess import check_output
from collections import defaultdict
import numpy
import os, sys, re

params = {
    'include_dirs': [
        numpy.get_include(),
        "third-parties/python-bind-helper",
        "src"
    ],
    'sources': [
        "python/pysg2.cxx",
        "src/sg2.cxx",
        "src/sg2_data_handler.cxx",
        "src/sg2_date.cxx",
        "src/sg2_geocentric.cxx",
        "src/sg2_geocentric_data.cxx",
        "src/sg2_geopoint.cxx",
        "src/sg2_topocentric.cxx",
        "src/sg2_utils.cxx"
    ],
    'depends': [
        "src/sg2_data_handler.hxx",
        "src/sg2_date.hxx",
        "src/sg2_err.hxx",
        "src/sg2_geocentric.hxx",
        "src/sg2_geopoint.hxx",
        "src/sg2.hxx",
        "src/sg2_math.hxx",
        "src/sg2_topocentric.hxx",
        "src/sg2_typedef.hxx",
        "src/sg2_utils.hxx",
        "third-parties/python-bind-helper/python-bind-helper.hxx"
    ]
}

if os.name == 'nt':
    params['extra_compile_args'] = ['/O2', '/DSG2_WIN32', '/std:c++17']
else:
    params['extra_compile_args'] = ['-O3', '-std=c++11']

setup(ext_modules = [Extension('sg2', **params)])

