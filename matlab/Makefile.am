
EXTRA_DIST = \
	sg2_jd_to_ymdh.m \
	sg2_sun_position.m \
	sg2_topocentric_correction_refraction_SAE.m \
	sg2_topocentric_correction_refraction_ZIM.m \
	sg2_ymdh_to_jd.m \
	sg2_sun_position.cxx \
	sg2_topocentric_correction_refraction_ZIM.cxx \
	sg2_topocentric_correction_refraction_SAE.cxx \
	sg2_sun_rise.cxx

all: @MATLAB_MEX@

%.mexa64: %.cxx
	@MEX@ -output $@ $^ -O -v -I$(top_srcdir)/src $(top_builddir)/src/.libs/libsg2.a

clean:
	rm -f sg2_sun_position.mexa64
	rm -f sg2_topocentric_correction_refraction_ZIM.mexa64
	rm -f sg2_topocentric_correction_refraction_SAE.mexa64
	rm -f sg2_sun_rise.mexa64
