
EXTRA_DIST = \
	sg2_sun_position.cxx

all: @MATLAB_MEX@

%.mexa64: %.cxx
	mex -output $@ $^ -O -v -I$(top_srcdir)/src $(top_builddir)/src/.libs/libsg2.a

clean:
	rm -f sg2_sun_position.mexa64

