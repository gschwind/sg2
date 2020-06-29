/*
 * sg2_date.h
 *
 *  Created on: 2 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_DATE_H_
#define SG2_DATE_H_

#ifdef  __cplusplus
extern "C"
{
#endif

S_SG2_DATE_JD *SG2_date_create_jd(unsigned long nd, int *p_err);
void SG2_date_delete_jd(S_SG2_DATE_JD *p_jd, int *p_err);

S_SG2_DATE_YMD_H *SG2_date_create_ymd_h(unsigned long nd, int *p_err);

void SG2_date_delete_ymd_h(S_SG2_DATE_YMD_H *p_ymdh, int *p_err);
S_SG2_DATE_YDOY_H *SG2_date_create_ydoy_h(unsigned long nd, int *p_err);

void SG2_date_delete_ydoy_h(S_SG2_DATE_YDOY_H *p_ydoyh, int *p_err);

void SG2_date_jd_to_ymdh(S_SG2_DATE_JD *p_jd, S_SG2_DATE_YMD_H *p_ymdh,
		int *p_err);
void SG2_date_ymdh_to_jd(S_SG2_DATE_YMD_H *p_ymdh, S_SG2_DATE_JD *p_jd,
		int *p_err);
void SG2_date_ymdh_to_ydoy(S_SG2_DATE_YMD_H *p_ymdh,
		S_SG2_DATE_YDOY_H *p_ydoyh, int *p_err);
void SG2_date_ydoy_to_ymdh(S_SG2_DATE_YDOY_H *p_ydoyh,
		S_SG2_DATE_YMD_H *p_ymdh, int *p_err);

void SG2_date_jd_set_jd_tt(S_SG2_DATE_JD *p_jd, double *p_delta_tt, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_DATE_H_ */
