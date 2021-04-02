#/bin/bash
# -*- coding: utf-8 -*-
#
# copyright (2021) MINES ParisTech
# copyright (2021) Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
#
# This script is used to build wheel package for python.
#
# it is intended to be used in docker as follow:
# docker pull "quay.io/pypa/manylinux2014_x86_64"
# docker container run -t -v "$(pwd):/io" "quay.io/pypa/manylinux2014_x86_64" /io/build-wheel.sh
#

mkdir -p /io/_build_wheel
cd /io/_build_wheel
../configure --with-pic CXXFLAGS="-O3" CFLAGS="-O3"
make clean
make
make install
cd -

# Cleanup
rm -rf /io/_build_wheel

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel /io/ --no-deps -w /io/wheelhouse/ --build-option --static
done

