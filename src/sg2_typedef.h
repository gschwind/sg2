/*
 *  sg2.h
 *
 *  Created on: feb. 2011
 *  Author: pblanc
 */

#ifndef SG2_TYPEDEF_H_
#define SG2_TYPEDEF_H_

#define SG2_PI 3.141592653589793

/* Date YMD + H en heure décimale UT */
typedef struct s_date_ymd_h {

	unsigned long n;
	short *y;
	short *m;
	short *d;
	double *h;

} S_SG2_DATE_YMD_H, *PS_SG2_DATE_YMD_H;

/* Date YDOY + H en heure décimale UT */
typedef struct s_date_ydoy_h {

	unsigned long n;
	short *y;
	short *doy;
	double *h;

} S_SG2_DATE_YDOY_H, *PS_SG2_DATE_YDOY_H;

/* Julian date en jour décimal en UT */
typedef struct s_date_jd {
	unsigned long n;
	double *jd;
	double *delta_tt_ut; /* en seconde */
} S_SG2_DATE_JD, *PS_SG2_DATE_JD;


#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
