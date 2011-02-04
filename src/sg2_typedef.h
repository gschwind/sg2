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

/* Julian date en jour décimal */
typedef struct s_date_jd {
	unsigned long n;
	double *jd_ut; /* julian date UT (decimal day) */
	unsigned char jd_tt_set;
	double *jd_tt; /* TT : terrestrial time */
} S_SG2_DATE_JD, *PS_SG2_DATE_JD;

/* Heliocentric coordinates */
typedef struct s_hcoord {
	S_SG2_DATE_JD *p_jd;
	double *R; /* Radius Sun-Earth (ua) */
	double *L; /* Heliocentric Earth true longitude (radian) */
} S_SG2_HCOORD, *PS_SG2_HCOORD;

/* Geocentric coordinates */
typedef struct s_gcoord {
	S_SG2_DATE_JD *p_jd;
	double *R; /* Radius Sun-Earth (ua) */
	double *epsilon; /* Earth true obliquity (radian) */
	double *Theta_a; /* Geocentric Earth true longitude (radian) */
	double *r_alpha; /* Geocentric right ascension (radian) */
	double *delta; /* Geocentric declination (radian) */
} S_SG2_GCOORD, *PS_SG2_GCOORD;

typedef struct s_ellps {
	double a; /* Axis a (m) */
	double f; /* Flatness (-)*/
} S_SG2_ELLPS, *PS_SG2_ELLPS;

typedef enum {
	SG2_ELLPSTYPE_WGS84 = 0,
	SG2_ELLPSTYPE_RGF83 = 1,
	SG2_ELLPSTYPE_NTF = 2,
	SG2_ELLPSTYPE_AA = 3,
	SG2_ELLPSTYPE_SPA = 4,
	SG2_ELLPSTYPE_NGP = 5,
	SG2_ELLPSTYPE_SPHERE = 6,
	SG2_ELLPSTYPE_USER = 7,
} SG2_ELLPSTYPE;

typedef struct s_geopoint {
	S_SG2_ELLPS *p_ellps;
	unsigned long p;
	double *lat;
	double *lon;
	double *h; /* Altitude Above the Reference Ellipsoid */
} S_SG2_GEOPOINT, *PS_SG2_GEOPOINT;

typedef struct s_tcoord {

	unsigned long n; /* Number of dates */
	unsigned long p; /* Number of geopoints */
	S_SG2_GCOORD *p_gcoord;
	S_SG2_GEOPOINT *p_gp;

	double

} S_SG2_TCOORD, *PS_SG2_TCOORD;

#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
