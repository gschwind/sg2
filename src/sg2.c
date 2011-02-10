/*
 * sg2.c
 *
 *  Created on: 10 févr. 2011
 *      Author: pblanc
 */

#define SG2_C_

#include "sg2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SG2_MACRO_CHECKERR(ret) if (*p_err != 0) { return (ret);}

void sg2_sunpos(double *p_lon, double *p_lat, double *p_elevation,
		unsigned long np, SG2_ELLPSTYPE ellpstype, double *p_data_ellps,
		double jd_ut0, double jd_ut1, double dtj, double *p_delta_tt, int *p_err) {

	unsigned long nd;
	S_SG2_DATE_TABJD *p_jd;
	S_SG2_HELIOC_DATA *p_helioc;
	S_SG2_GEOC_DATA *p_geoc;
	S_SG2_TABGEOPOINT *p_gp;
	S_SG2_TOPOC_DATA *p_topoc;
	int kd;

	nd = (unsigned long) floor((jd_ut1-jd_ut0)/dtj);
	p_jd = SG2_date_create_tabjd(nd, p_err);
	p_jd->jd_ut[0] = jd_ut0;
	for (kd = 1; kd < nd; kd ++) {
		p_jd->jd_ut[kd] = p_jd->jd_ut[kd-1] + dtj;
	}
	SG2_date_set_tabjd_tt(p_jd,p_delta_tt,p_err);

	p_gp = SG2_topocentric_create_tabgeopoint(np, ellpstype, p_data_ellps, p_err);

	SG2_topocecentric_set_tabgeopoint(p_lon,p_lat,p_elevation,p_gp,p_err);

	p_helioc = SG2_heliocentric_create_helioc_data(p_jd->nd, p_err);

	SG2_heliocentric_set_helioc_data(p_jd, p_helioc, p_err);

	p_geoc = SG2_geocentric_create_geoc_data(p_jd->nd, p_err);

	SG2_geocentric_set_geoc_data(p_jd, p_helioc, p_geoc, p_err);

	p_topoc = SG2_topocentric_create_topoc_data(nd, np, p_err);

	SG2_topocentric_set_topoc_data(p_geoc, p_gp, p_topoc, p_err);

}
