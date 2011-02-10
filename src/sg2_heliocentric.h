/*
 * sg2_heliocentric.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_HELIOCENTRIC_H_
#define SG2_HELIOCENTRIC_H_

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

EXPORT S_SG2_HELIOC_DATA
*SG2_heliocentric_create_helioc_data(unsigned long n, int *p_err);

EXPORT void SG2_heliocentric_delete_helioc_data(S_SG2_HELIOC_DATA *p_helioc,
		int *p_err);

EXPORT void SG2_heliocentric_set_helioc_data(S_SG2_DATE_TABJD *p_jd,
		S_SG2_HELIOC_DATA *p_helioc, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_HELIOCENTRIC_H_ */
