/*
 * sg2_date.h
 *
 *  Created on: 2 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_DATE_H_
#define SG2_DATE_H_

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

EXPORT S_SG2_DATE_TABJD *SG2_date_create_tabjd(unsigned long nd, int *p_err);
EXPORT void SG2_date_delete_tabjd(S_SG2_DATE_TABJD *p_jd, int *p_err);

EXPORT S_SG2_DATE_TABYMDH *SG2_date_create_tabymdh(unsigned long nd, int *p_err);

EXPORT void SG2_date_delete_tabymdh(S_SG2_DATE_TABYMDH *p_ymdh, int *p_err);

EXPORT S_SG2_DATE_TABYDOYH *SG2_date_create_tabydoy(unsigned long nd, int *p_err);

EXPORT void SG2_date_delete_tabydoy(S_SG2_DATE_TABYDOYH *p_ydoyh, int *p_err);

EXPORT void SG2_date_tabjd_to_tabymdh(S_SG2_DATE_TABJD *p_jd, S_SG2_DATE_TABYMDH *p_ymdh,
		int *p_err);

EXPORT void SG2_date_tabymdh_to_tabjd(S_SG2_DATE_TABYMDH *p_ymdh, S_SG2_DATE_TABJD *p_jd,
		int *p_err);

EXPORT void SG2_date_tabymdh_to_tabydoyh(S_SG2_DATE_TABYMDH *p_ymdh,
		S_SG2_DATE_TABYDOYH *p_ydoyh, int *p_err);

EXPORT void SG2_date_tabydoyh_to_tabymdh(S_SG2_DATE_TABYDOYH *p_ydoyh,
		S_SG2_DATE_TABYMDH *p_ymdh, int *p_err);

EXPORT void SG2_date_set_tabjd_tt(S_SG2_DATE_TABJD *p_jd, double *p_delta_tt, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_DATE_H_ */
