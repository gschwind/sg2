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

S_SG2_GEOPT *SG2_topocentric_create_geopt(unsigned long np,
		SG2_ELLPSTYPE ellpstype, int *p_err);

void SG2_topocentric_delete_geopt(S_SG2_GEOPT *p_gp, int *p_err);
S_SG2_TOPOC *SG2_topocentric_create_topoc(unsigned long n, unsigned long p,
		int *p_err);

S_SG2_TOPOC *SG2_topocentric_create_topoc(unsigned long nd, unsigned long np,
		int *p_err);

void SG2_topocentric_delete_topoc(S_SG2_TOPOC *p_topoc, int *p_err);

void SG2_topocentric_set_topoc(S_SG2_GEOC *p_geoc, S_SG2_GEOPT *p_gp,
		S_SG2_TOPOC *p_topoc, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_TOPOCENTRIC_H_ */
