# -*- coding: utf-8 -*-

AC_PREREQ([2.69])
AC_INIT([sg2], [2.3.0])
AC_CONFIG_MACRO_DIR([m4])
AM_MAINTAINER_MODE([disable])

LT_PREREQ([2.4.2])
LT_INIT()
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([src/sg2.cxx])

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AM_PROG_AS

# check for c++11
AX_CXX_COMPILE_STDCXX_11(noext, mandatory)

AC_ARG_ENABLE(vdt, [AS_HELP_STRING([--enable-vdt], [Use VDT library for fast trigonometric math])], [
	PKG_CHECK_MODULES(VDT, vdt)
	AC_SUBST(VDT_DEFINE, [-DSG2_HAVE_VDT])
	AC_SUBST(VDT_CFLAGS)
	AC_SUBST(VDT_LIBS)
])

AC_ARG_ENABLE(matlab, [AS_HELP_STRING([--enable-matlab], [Generate matlab mex files])], [
	AC_PATH_PROG(MEX, [mex])
	if test -z "$ac_cv_path_MEX"; then
		AC_MSG_ERROR([mex executable not found.])
	fi
	AC_SUBST(MEX)
	AC_SUBST(MATLAB_MEX, ["sg2_sun_position.mexa64 sg2_topocentric_correction_refraction_ZIM.mexa64 sg2_topocentric_correction_refraction_SAE.mexa64 sg2_sun_rise.mexa64"])
])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  matlab/Makefile
  sg2.pc
])
AC_OUTPUT
