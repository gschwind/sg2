#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from distutils.core import setup, Extension
from subprocess import check_output
from collections import defaultdict
import numpy
import os, sys, re


if os.name == 'nt':
    # Window build is simplier but not standalone, read README.win
    params = {
        'extra_compile_args': ['/O2', '/DSG2_WIN32', '/std:c++17'],
        'include_dirs': [numpy.get_include(), "third-parties/python-bind-helper", "src"],
        'sources': [
            "python/pysg2.cxx",
            "src/sg2.cxx",
            "src/sg2_date.cxx",
            "src/sg2_utils.cxx",
            "src/sg2_geocentric.cxx",
            "src/sg2_geopoint.cxx",
            "src/sg2_topocentric.cxx",
            "src/sg2_data_handler.cxx"
        ],
        # See README.win to generate this file.
        'extra_objects': ["resources/sg2_geocentric_data.obj"]
    }

else:
    def pkgconfig(*packages, **kw):
     flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
     ret = defaultdict(list)
     ret.update(kw)
     for token in check_output(["pkg-config", "--libs", "--cflags"] + list(packages)).decode('UTF-8').split():
      if token[:2] in flag_map:
       ret[flag_map.get(token[:2])].append(token[2:])
      else:
       ret['extra_link_args'].append(token)
     for k, v in kw.items(): # remove duplicated
      ret[k] = list(set(ret[k]))
     return ret

    params = pkgconfig('sg2')
    params['extra_compile_args'] = ['-O3', '-std=c++11']
    params["include_dirs"] += [numpy.get_include(), "third-parties/python-bind-helper", "src"]
    params["sources"] = [ 'python/pysg2.cxx' ]

    # This section try to find the required
    # static libraries
    if "--no-static" not in sys.argv:
        new_libraries = []
        for l in params["libraries"]:
            static_lib_found = False
            for p in params["library_dirs"]:
                f = "%s/lib%s.a"%(p,l)
                if os.path.exists(f):
                    params["extra_objects"].append(f)
                    static_lib_found = True
                    break
            if not static_lib_found:
                new_libraries.append(l)

        params["libraries"] = new_libraries
    else:
        sys.argv.remove("--no-static")


setup(
    ext_modules = [Extension('sg2', **params)]
)

