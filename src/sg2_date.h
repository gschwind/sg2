/*
 * Copyright 2011 Philippe Blanc <philippe.blanc@mines-paristech.fr>
 *
 * This file is part of libsg2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
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

EXPORT S_SG2_DATE_TABYDOYH *SG2_date_create_tabydoyh(unsigned long nd, int *p_err);

EXPORT void SG2_date_delete_tabydoyh(S_SG2_DATE_TABYDOYH *p_ydoyh, int *p_err);

EXPORT void SG2_date_tabjd_to_tabymdh(S_SG2_DATE_TABJD *p_jd, S_SG2_DATE_TABYMDH *p_ymdh,
		int *p_err);

EXPORT void SG2_date_tabymdh_to_tabjd(S_SG2_DATE_TABYMDH *p_ymdh, S_SG2_DATE_TABJD *p_jd,
		int *p_err);

EXPORT void SG2_date_tabymdh_to_tabydoyh(S_SG2_DATE_TABYMDH *p_ymdh,
		S_SG2_DATE_TABYDOYH *p_ydoyh, int *p_err);

EXPORT void SG2_date_tabydoyh_to_tabymdh(S_SG2_DATE_TABYDOYH *p_ydoyh,
		S_SG2_DATE_TABYMDH *p_ymdh, int *p_err);

EXPORT void SG2_date_set_tabjd_tt(double *p_delta_tt, S_SG2_DATE_TABJD *p_jd, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_DATE_H_ */
