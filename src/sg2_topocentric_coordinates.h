/*
 * sg2_topocentric_coordinates.h
 *
 *  Created on: 4 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_TOPOCENTRIC_COORDINATES_H_
#define SG2_TOPOCENTRIC_COORDINATES_H_

#ifdef  __cplusplus
extern "C"
{
#endif

static S_SG2_ELLPS[7] = {
		{ .a = 6378137.0, .f = 3.352810664747481e-003 }, /* WGS84 */
		{ .a = 6378137.0, .f = 3.352810681182319e-003 }, /* RFG83 */
		{ .a = 6378249.2, .f = 3.407549520015651e-003 }, /* NTF / CLARKE1880 */
		{ .a = 6378136.6, .f = 3.352819697896193e-003 }, /* AA */
		{ .a = 6378140.0, .f = 3.352810000000000e-003 }, /* SPA */
		{ .a = 6378169.0, .f = 3.384231430681783e-003 }, /* NGP*/
		{ .a = 6378130.0, .f = 0.0 } /* SPHERE */
};

#ifdef	__cplusplus
}
#endif

#endif /* SG2_TOPOCENTRIC_COORDINATES_H_ */
