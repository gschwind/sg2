/*
 * sg2_geocentric.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_GEOCENTRIC_H_
#define SG2_GEOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

S_SG2_GCOORD *SG2_geocentric_create_gcoord(unsigned long n,
		int *p_err);
void SG2_geocentric_delete_gcoord(S_SG2_GCOORD *p_gcoord,
		int *p_err);
void SG2_geocentric_set_gcoord(S_SG2_DATE_JD *p_jd,
		S_SG2_GCOORD *p_gcoord, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_GEOCENTRIC_H_ */
