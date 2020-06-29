/*
 *  sg2.h
 *
 *  Created on: feb. 2011
 *  Author: pblanc
 */

#ifndef SG2_TYPEDEF_H_
#define SG2_TYPEDEF_H_

#define SG2_PI 3.141592653589793
#define SG2_DEG2RAD 1.745329251994330e-002
#define SG2_RAD2DEG 5.729577951308232e+001
/* Top of atmosphere broadband irradce on normal incidence (W/m2) */
#define SG2_SOLAR_CONSTANT 1367
/* Astronomical unit : annual average of the Sun-Earth distance (m) */
#define SG2_AU 149597870691.0 /* +/- 6 m (McCarthy et Petit, 2003) */

/* Date YMD + H en heure décimale UT */
typedef struct s_date_ymd_h {

	unsigned long nd;
	short *y;
	short *m;
	short *d;
	double *h;

} S_SG2_DATE_YMD_H, *PS_SG2_DATE_YMD_H;

/* Date YDOY + H en heure décimale UT */
typedef struct s_date_ydoy_h {

	unsigned long nd;
	short *y;
	short *doy;
	double *h;

} S_SG2_DATE_YDOY_H, *PS_SG2_DATE_YDOY_H;

/* Julian date */
typedef struct s_date_jd {
	unsigned long nd;
	double *jd_ut; /* julian date UT (decimal day) */
	unsigned char jd_tt_set;
	double *jd_tt; /* TT : terrestrial time */
} S_SG2_DATE_JD, *PS_SG2_DATE_JD;

/* Heliocentric coordinates */
typedef struct s_helioc {
	S_SG2_DATE_JD *p_jd;
	unsigned long nd;
	double *R; /* Radius Sun-Earth (ua) */
	double *L; /* Heliocentric Earth true longitude (rad) */
} S_SG2_HELIOC, *PS_SG2_HELIOC;

/* Geocentric coordinates */
typedef struct s_geoc {
	S_SG2_DATE_JD *p_jd;
	S_SG2_HELIOC *p_helioc;
	unsigned long nd;

	double *Delta_psi; /* Nutation in Geocentric Sun longitude (rad) */
	double *epsilon; /* Earth true obliquity (rad) */
	double *Theta_a; /* Geocentric Earth true longitude (rad) */
	double *r_alpha; /* Geocentric right ascension (rad) */
	double *delta; /* Geocentric declination (rad) */

	double *nu; /* Apparent sideral time (rad) */
	double *EOT; /* Equation of Time (rad) : difference between apparent solar time and mean solar time */

} S_SG2_GEOC, *PS_SG2_GEOC;

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

typedef struct s_geopt {
	S_SG2_ELLPS *p_ellps;
	unsigned long np;
	double *phi; /* Latitude (rad) */
	double *lambda; /* Longitude (rad) */
	double *h; /* Altitude Above the Reference Ellipsoid */
} S_SG2_GEOPT, *PS_SG2_GEOPT;

typedef struct s_topocentric {

	unsigned long nd; /* Number of dates */
	unsigned long np; /* Number of geopoints */

	S_SG2_DATE_JD *p_jd;
	S_SG2_HELIOC *p_helioc;
	S_SG2_GEOC *p_geoc;
	S_SG2_GEOPT *p_gp;

	double **r_alpha; /* Topocentric right sun ascension (rad) */
	double **delta; /* Topocentric sun declination (rad) */
	double **omega; /* Topocentric local hour angle (rad) */
	double **gamma_S0; /* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double **alpha_S; /* Topocentric sun azimuth (rad) */

} S_SG2_TOPOC, *PS_SG2_TOPOC;

#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
