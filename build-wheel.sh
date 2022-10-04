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

tar -xzf /io/sg2-2.3.0.tar.gz -C /tmp/
cd /tmp/sg2-2.3.0
./configure --with-pic CXXFLAGS="-O3" CFLAGS="-O3"
make clean
make
make install
cd -

# Cleanup
rm -rf /tmp/sg2-2.3.0

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel /io/ --no-deps -w /io/wheelhouse/
done

