/*
 * sg2_geocentric.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_GEOCENTRIC_H_
#define SG2_GEOCENTRIC_H_

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

EXPORT S_SG2_GEOC_DATA *SG2_geocentric_create_geoc_data(unsigned long n, int *p_err);

EXPORT void SG2_geocentric_delete_geoc_data(S_SG2_GEOC_DATA *p_geoc, int *p_err);

EXPORT void SG2_geocentric_set_geoc_data(S_SG2_DATE_TABJD *p_jd, S_SG2_HELIOC_DATA *p_helioc,
		S_SG2_GEOC_DATA *p_geoc, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_GEOCENTRIC_H_ */
