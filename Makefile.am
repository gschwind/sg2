SUBDIRS = src

ACLOCAL_AMFLAGS = -I m4 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sg2.pc

EXTRA_DIST = resources

dist-hook:
	chmod u+w $(distdir)/resources/
	$(CC) -c -I$(disdir)/resources/ -o $(distdir)/resources/sg2_geocentric_data.o $(distdir)/src/sg2_geocentric_data.s
	objcopy --output-target pe-x86-64 $(distdir)/resources/sg2_geocentric_data.o $(distdir)/resources/sg2_geocentric_data.obj
