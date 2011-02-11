/*
 * sg2.h
 *
 *  Created on: 2 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_H_
#define SG2_H_

#ifdef _MINGW_
#include <windows.h>
#endif

#ifdef EXPORT
#undef EXPORT
#endif

#ifdef _VISUAL_
#define EXPORT _declspec (dllexport)
#else
#define EXPORT
#endif

#ifdef  __cplusplus
extern "C"
{
#endif

#include "SG2_err.h"
#include "SG2_typedef.h"
#include "SG2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"
#include <stdio.h>

FILE *fp_log;

EXPORT S_SG2_SUNPOS *sg2_create_sunpos(unsigned long np, unsigned long nd,
		int *p_err);

EXPORT void sg2_delete_sunpos(S_SG2_SUNPOS *p_sunpos, int *p_err);

EXPORT void sg2_set_sunpos(double *p_lon, double *p_lat, double *p_elevation,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, double *p_jd_ut,
		double *p_delta_tt,
		SG2_CORRECTION_REFRACTION method, double *data_corr,
		S_SG2_SUNPOS *p_sunpos, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
