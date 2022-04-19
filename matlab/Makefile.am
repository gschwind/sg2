
EXTRA_DIST = \
	sg2_sun_position.cxx \
	sg2_ymdh_to_jd.cxx \
	sg2_jd_to_ymdh.cxx \
	sg2_topocentric_correction_refraction_ZIM.cxx \
	sg2_topocentric_correction_refraction_SAE.cxx

all: @MATLAB_MEX@

%.mexa64: %.cxx
	@MEX@ -output $@ $^ -O -v -I$(top_srcdir)/src $(top_builddir)/src/.libs/libsg2.a

clean:
	rm -f sg2_sun_position.mexa64
	rm -f sg2_ymdh_to_jd.mexa64
	rm -f sg2_jd_to_ymdh.mexa64
	rm -f sg2_topocentric_correction_refraction_ZIM.mexa64
	rm -f sg2_topocentric_correction_refraction_SAE.mexa64
