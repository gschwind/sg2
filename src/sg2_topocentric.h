/*
 * sg2_topocentric.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_TOPOCENTRIC_H_
#define SG2_TOPOCENTRIC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

S_SG2_GEOPOINT *SG2_topocentric_create_geopoint(unsigned long p,
		SG2_ELLPSTYPE ellpstype, int *p_err);

void SG2_topocentric_delete_geopoint(S_SG2_GEOPOINT *p_gp, int *p_err);
S_SG2_TCOORD *SG2_topocentric_create_tcoord(unsigned long n, unsigned long p,
		int *p_err);

void SG2_topocentric_delete_tcoord(S_SG2_TCOORD *p_tcoord, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_TOPOCENTRIC_H_ */
