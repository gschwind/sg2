/*
 * sg2_heliocentric_coordinates.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_HELIOCENTRIC_COORDINATES_H_
#define SG2_HELIOCENTRIC_COORDINATES_H_

#ifdef  __cplusplus
extern "C"
{
#endif

S_SG2_HCOORD *SG2_heliocentric_coordinates_create_hcoord(unsigned long n,
		int *p_err);
void SG2_heliocentric_coordinates_delete_hcoord(S_SG2_HCOORD *p_hcoord,
		int *p_err);
void SG2_heliocentric_coordinates_set_hcoord(S_SG2_DATE_JD *p_jd,
		S_SG2_HCOORD *p_hcoord, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_HELIOCENTRIC_COORDINATES_H_ */
