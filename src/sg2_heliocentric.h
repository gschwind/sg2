/*
 * sg2_heliocentric.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_HELIOCENTRIC_H_
#define SG2_HELIOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

S_SG2_HELIOC *SG2_heliocentric_create_helioc(unsigned long n,
		int *p_err);
void SG2_heliocentric_delete_helioc(S_SG2_HELIOC *p_helioc,
		int *p_err);
void SG2_heliocentric_set_helioc(S_SG2_DATE_JD *p_jd,
		S_SG2_HELIOC *p_helioc, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_HELIOCENTRIC_H_ */
