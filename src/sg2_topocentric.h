/*
 * sg2_topocentric.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_TOPOCENTRIC_H_
#define SG2_TOPOCENTRIC_H_

#ifdef _MINGW_
#include <windows.h>
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

EXPORT S_SG2_TABGEOPOINT *SG2_topocentric_create_tabgeopoint(unsigned long np,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, int *p_err);

EXPORT void SG2_topocentric_delete_tabgeopoint(S_SG2_TABGEOPOINT *p_gp,
		int *p_err);

EXPORT void SG2_topocecentric_set_tabgeopoint(double *lon, double *lat,
		double *h, S_SG2_TABGEOPOINT *p_gp, int *p_err);

EXPORT S_SG2_TOPOC_DATA *SG2_topocentric_create_topoc_data(unsigned long np,
		unsigned long nd, int *p_err);

EXPORT void SG2_topocentric_delete_topoc_data(S_SG2_TOPOC_DATA *p_topoc,
		int *p_err);

EXPORT void SG2_topocentric_set_topoc_data(S_SG2_GEOC_DATA *p_geoc,
		S_SG2_TABGEOPOINT *p_gp, S_SG2_TOPOC_DATA *p_topoc, int *p_err);

EXPORT void SG2_topocentric_correction_refraction(double *p_gamma_S0, unsigned long n,
		SG2_CORRECTION_REFRACTION method, double *p_data_corr, double *p_gamma_S, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_TOPOCENTRIC_H_ */
