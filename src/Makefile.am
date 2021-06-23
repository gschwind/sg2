bin_PROGRAMS = sg2_query benchmark_libsg2 sg2_convert_date

AM_CXXFLAGS = \
	@VDT_DEFINE@ \
	@VDT_CFLAGS@

AM_CCASFLAGS = \
	-I$(srcdir)

lib_LTLIBRARIES = libsg2.la
include_HEADERS = \
	sg2.hxx \
	sg2_math.hxx \
	sg2_typedef.hxx \
	sg2_err.hxx \
	sg2_date.hxx \
	sg2_geocentric.hxx \
	sg2_topocentric.hxx

libsg2_la_LDFLAGS = -version-info 2:0:0
libsg2_la_SOURCES = \
	sg2.cxx \
	sg2.hxx \
	sg2_date.cxx \
	sg2_date.hxx \
	sg2_geopoint.cxx \
	sg2_geopoint.hxx \
	sg2_geocentric.cxx \
	sg2_geocentric.hxx \
	sg2_topocentric.cxx \
	sg2_topocentric.hxx \
	sg2_data_handler.cxx \
	sg2_data_handler.hxx \
	sg2_utils.cxx \
	sg2_utils.hxx \
	sg2_geocentric_data.s

sg2_query_LDADD = @VDT_LIBS@ libsg2.la
sg2_query_SOURCES = \
	sg2_query.cxx

benchmark_libsg2_LDADD = @VDT_LIBS@ libsg2.la
benchmark_libsg2_SOURCES = \
	benchmark_libsg2.cxx

sg2_convert_date_LDADD = @VDT_LIBS@ libsg2.la
sg2_convert_date_SOURCES = \
	sg2_convert_date.cxx

