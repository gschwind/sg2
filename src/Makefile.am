bin_PROGRAMS = sg2_query sg2_benchmark sg2_convert_date

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

# /!\ Be causious when updating version number, then do not match PACKAGE_VERSION on purpose
# The version-info is defined as follow: current:revision:age
# The rule to apply are explained in [1] as follow:
#
# If either revision or age are omitted, they default to 0. Also note that age
# must be less than or equal to the current interface number.
#
# Here are a set of rules to help you update your library version information:
#
#    1. Start with version information of ‘0:0:0’ for each libtool library.
#    2. Update the version information only immediately before a public release
#       of your software. More frequent updates are unnecessary, and only
#       guarantee that the current interface number gets larger faster.
#    3. If the library source code has changed at all since the last update,
#       then increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
#    4. If any interfaces have been added, removed, or changed since the last
#       update, increment current, and set revision to 0.
#    5. If any interfaces have been added since the last public release, then
#       increment age.
#    6. If any interfaces have been removed or changed since the last public
#       release, then set age to 0.
#
# [1] https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
#
libsg2_la_LDFLAGS = -version-info 3:0:0
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

sg2_benchmark_LDADD = @VDT_LIBS@ libsg2.la
sg2_benchmark_SOURCES = \
	sg2_benchmark.cxx

sg2_convert_date_LDADD = @VDT_LIBS@ libsg2.la
sg2_convert_date_SOURCES = \
	sg2_convert_date.cxx

