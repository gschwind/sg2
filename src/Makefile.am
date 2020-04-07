bin_PROGRAMS = query_sg2 benchmark_libsg2

AM_CXXFLAGS = \
	@VDT_DEFINE@ \
	@VDT_CFLAGS@

lib_LTLIBRARIES = libsg2.la
include_HEADERS = \
	sg2.hxx \
	sg2_math.hxx \
	sg2_angle.hxx \
	sg2_typedef.hxx \
	sg2_err.hxx \
	sg2_date.hxx \
	sg2_geocentric.hxx \
	sg2_topocentric.hxx \
	sg2_sun_daily.hxx

libsg2_la_LDFLAGS = -version-info 2:0:0
libsg2_la_SOURCES = \
	sg2.cxx \
	sg2.hxx \
	sg2_date.cxx \
	sg2_date.hxx \
	sg2_geocentric.cxx \
	sg2_geocentric.hxx \
	sg2_topocentric.cxx \
	sg2_topocentric.hxx \
	sg2_sun_daily.cxx \
	sg2_sun_daily.hxx \
	sg2_precomputed_delta_tt.cxx \
	sg2_precomputed_delta_tt.hxx \
	sg2_precomputed_geocentric.cxx \
	sg2_precomputed_geocentric.hxx \
	sg2_precomputed_heliocentric.cxx \
	sg2_precomputed_heliocentric.hxx

query_sg2_LDADD = @VDT_LIBS@ libsg2.la
query_sg2_SOURCES = \
	query_sg2.cxx

benchmark_libsg2_LDADD = @VDT_LIBS@ libsg2.la
benchmark_libsg2_SOURCES = \
	benchmark_libsg2.cxx
