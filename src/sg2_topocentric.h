/*
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

#ifndef SG2_TOPOCENTRIC_H_
#define SG2_TOPOCENTRIC_H_

#ifdef _WIN32
#pragma once

// Modifiez les définitions suivantes si vous devez cibler une plate-forme avant celles spécifiées ci-dessous.
// Reportez-vous à MSDN pour obtenir les dernières informations sur les valeurs correspondantes pour les différentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.
#define WINVER 0x0501		// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalités spécifiques à Windows 98 ou version ultérieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropriée à cet élément pour cibler Windows Me ou version ultérieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalités spécifiques à Internet Explorer 6.0 ou version ultérieure.
#define _WIN32_IE 0x0600	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions d'Internet Explorer.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclure les en-têtes Windows rarement utilisés

// Fichiers d'en-tête Windows :
#include <windows.h>
#endif

#ifdef _MINGW_
#include <windows.h>
#endif


#ifdef EXPORT
#undef EXPORT
#endif


#ifdef _WIN32
#define EXPORT _declspec (dllexport)
#else
#define EXPORT
#endif

#ifdef  __cplusplus
extern "C"
{
#endif

EXPORT S_SG2_TABGEOPOINT *SG2_topocentric_create_tabgeopoint(unsigned long np,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, int *p_err);

EXPORT void SG2_topocentric_delete_tabgeopoint(S_SG2_TABGEOPOINT *p_gp,
		int *p_err);

EXPORT void SG2_topocecentric_set_tabgeopoint(double *lon, double *lat,
		double *h, S_SG2_TABGEOPOINT *p_gp, int *p_err);

EXPORT S_SG2_TOPOC_DATA *SG2_topocentric_create_topoc_data(unsigned long np,
		unsigned long nd, int *p_err);

EXPORT void SG2_topocentric_delete_topoc_data(S_SG2_TOPOC_DATA *p_topoc,
		int *p_err);

EXPORT void SG2_topocentric_set_topoc_data(S_SG2_GEOC_DATA *p_geoc,
		S_SG2_TABGEOPOINT *p_gp, S_SG2_TOPOC_DATA *p_topoc, int *p_err);

EXPORT void SG2_topocentric_correction_refraction(double *p_gamma_S0, unsigned long n,
		SG2_CORRECTION_REFRACTION method, double *p_data_corr, double *p_gamma_S, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_TOPOCENTRIC_H_ */
