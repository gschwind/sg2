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


# Install sg2 system wide
tar -xzf /io/sg2-2.3.1.tar.gz -C /tmp/
cd /tmp/sg2-2.3.1
./configure --with-pic CXXFLAGS="-O3" CFLAGS="-O3"
make clean
make
make install
cd -

# Cleanup
rm -rf /tmp/sg2-2.3.1

# Create user en for pip
useradd -m user
cp /io/sg2-2.3.1.tar.gz /home/user/sg2-2.3.1.tar.gz
chown user /home/user/sg2-2.3.1.tar.gz

tee /home/user/script.sh <<"EOF"
export PATH=$PATH:/home/user/.local/bin
tar -xzf sg2-2.3.1.tar.gz
cd sg2-2.3.1

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel . --no-deps -w /home/user/wheelhouse/
done
EOF

chown user /home/user/script.sh

cd /home/user/
runuser -u user /bin/bash /home/user/script.sh
cp /home/user/wheelhouse/* /io/wheelhouse/

