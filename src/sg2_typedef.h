/* -*- coding: utf-8 -*-
 *
 * Copyright 2011 Philippe Blanc <philippe.blanc@mines-paristech.fr>
 *
 * This file is part of libsg2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef SG2_TYPEDEF_H_
#define SG2_TYPEDEF_H_

#ifdef	__cplusplus
extern "C"
  {
#endif

#define SG2_PI  3.1415926535897931
#define SG2_2PI 6.2831853071795862
#define SG2_DEG2RAD 1.745329251994330e-002
#define SG2_RAD2DEG 5.729577951308232e+001
/* Top of atmosphere broadband irradiance on normal incidence (W/m2) */
#define SG2_SOLAR_CONSTANT 1367.0
/* Astronomical unit : annual average of the Sun-Earth distance (m) */
#define SG2_AU 149597870691.0 /* +/- 6 m (McCarthy et Petit, 2003) */

#define SG2_MACRO_0_2PI(x) ((x)-floor((x)/(SG2_2PI))*(SG2_2PI))
#define SG2_MACRO_PI_PI(x) ((x)-round((x)/(SG2_2PI))*(SG2_2PI))

/* Date YMD + H en heure décimale UT */
typedef struct s_date_ymd_h {

	unsigned long nd;
	short *y;
	short *m;
	short *d;
	double *h;

} S_SG2_DATE_TABYMDH;

/* Date YMD + H en heure décimale UT */
typedef struct _date_ymdh_t {
	short year;
	char month;
	char day_of_month;
	double hour;
} date_ymdh_t;

/* Date YDOY + H en heure décimale UT */
typedef struct s_date_ydoy_h {

	unsigned long nd;
	short *y;
	short *doy;
	double *h;

} S_SG2_DATE_TABYDOYH;

typedef struct _date_ydoyh_t {
	short year;
	short day_of_year;
	double hour;
} date_ydoyh_t;

typedef struct {
	double jd_ut;            /* julian date UT (decimal day) */
	double jd_tt;            /* TT : terrestrial time */
} time_data_t;

/* Heliocentric coordinates */
typedef struct {
	double R; /* Radius Sun-Earth (ua) */
	double L; /* Heliocentric Earth true longitude (rad) */
} sg2_heliocentric_data_t;

/* Geocentric coordinates */
typedef struct {
	double Delta_psi;  /* Nutation in Geocentric Sun longitude (rad) */
	double epsilon;    /* Earth true obliquity (rad) */
	double Theta_a;    /* Geocentric Earth true longitude (rad) */
	double r_alpha;    /* Geocentric right ascension (rad) */
	double delta;      /* Geocentric declination (rad) */
	double nu;         /* Apparent sideral time (rad) */
	double EOT;        /* Equation of Time (rad) : difference between apparent solar time and mean solar time */
} sg2_geocentric_data_t;

/** handle time related geocentric sun position **/
typedef struct {
	time_data_t jd;
	sg2_heliocentric_data_t helioc;
	sg2_geocentric_data_t geoc;
} sg2_geocentric_sun_position_t;

typedef struct s_ellps {
	double a; /* Axis a (m) */
	double f; /* Flatness (-)*/
} S_SG2_ELLPS, *PS_SG2_ELLPS;

extern S_SG2_ELLPS const * SG2_ELLPSTYPE_WGS84;
extern S_SG2_ELLPS const * SG2_ELLPSTYPE_RGF83;
extern S_SG2_ELLPS const * SG2_ELLPSTYPE_NTF;
extern S_SG2_ELLPS const * SG2_ELLPSTYPE_AA;
extern S_SG2_ELLPS const * SG2_ELLPSTYPE_SPA;
extern S_SG2_ELLPS const * SG2_ELLPSTYPE_NGP;
extern S_SG2_ELLPS const * SG2_ELLPSTYPE_SPHERE;

typedef struct {
	S_SG2_ELLPS const * ellps;
	double phi;        /* Latitude (rad) */
	double lambda;     /* Longitude (rad) */
	double h;          /* Altitude Above the Reference Ellipsoid */
	double cos_phi;
	double sin_phi;
	double u;
	double x;
	double y;
} sg2_geopoint_t;

typedef struct {
	sg2_geocentric_sun_position_t const * sun_position;
	sg2_geopoint_t const * geopoint;
	double r_alpha;  /* Topocentric right sun ascension (rad) */
	double delta;    /* Topocentric sun declination (rad) */
	double omega;    /* Topocentric local hour angle (rad) */
	double gamma_S0; /* Topocentric sun elevation angle without correction of atm. corr. (rad)*/
	double alpha_S;  /* Topocentric sun azimuth (rad) */
	double toa_ni;   /* irradiation at top of atmosphere normal incidence (W/m2) */
	double toa_hi;   /* irradiation at top of atmosphere horizontal incidence (W/m2) */
} sg2_topocentric_data_t;

typedef enum {
	SG2_CORRECTION_REFRACTION_NONE = 0,
	SG2_CORRECTION_REFRACTION_SAE = 1,
	SG2_CORRECTION_REFRACTION_ZIM = 2,
} SG2_CORRECTION_REFRACTION;

//typedef struct s_sunpos {
//
//	unsigned long np;
//	unsigned long nd;
//
//	time_data_t *p_jd;
//
//	double **omega;
//	double **delta;
//	double **alpha_S;
//	double **gamma_S0;
//	double **gamma_S;
//	double **E0;
//
//} S_SG2_SUNPOS;

//typedef struct {
//	double omega;
//	double delta;
//	double alpha_S;
//	double gamma_S0;
//	double gamma_S;
//	double E0;
//} sunpos_t;

//typedef struct s_tabtilt {
//	unsigned long na;
//	double *alpha; /* azimuth of the plane, from North, eastward (rad) */
//	double *beta; /* slope of the plane from horizontal (rad) */
//} S_SG2_TABTILT;

//typedef struct {
//	double alpha;  /* azimuth of the plane, from North, eastward (rad) */
//	double beta;   /* slope of the plane from horizontal (rad) */
//} tabtilt_t;


//typedef struct s_toa_irrad {
//
//	unsigned long np;
//	unsigned long nd;
//	unsigned long na;
//
//	double dt;
//	double eta; /* 0 : left, 0.5 : middle, 1 : right */
//
//	S_SG2_DATE_TABJD *p_jd;
//	S_SG2_TABTILT *p_tilt;
//	S_SG2_SUNPOS *p_sunpos;
//	double ***toa_dt; /* toa_dt[0..na-1][0..np-1][0..nd-1] */
//
//} S_SG2_TOA_IRRAD;



#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
