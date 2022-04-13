SUBDIRS = src matlab

ACLOCAL_AMFLAGS = -I m4 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sg2.pc

EXTRA_DIST = \
	resources \
	python/pysg2.cxx \
	third-parties/python-bind-helper/python-bind-helper.hxx \
	setup.py \
	build-wheel.sh \
	build-win-wheels.ps1 \
	dev-requirements.txt\
	README.linux \
	README.win 


dist-hook:
	chmod u+w $(distdir)/resources/
	$(CC) -c -I$(distdir)/resources/ -o $(distdir)/resources/sg2_geocentric_data.o $(distdir)/src/sg2_geocentric_data.s
	objcopy --output-target pe-x86-64 $(distdir)/resources/sg2_geocentric_data.o $(distdir)/resources/sg2_geocentric_data.obj
