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

#ifndef SG2_H_
#define SG2_H_

#ifdef _WIN32
#pragma once

// Modifiez les d�finitions suivantes si vous devez cibler une plate-forme avant celles sp�cifi�es ci-dessous.
// Reportez-vous � MSDN pour obtenir les derni�res informations sur les valeurs correspondantes pour les diff�rentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.
#define WINVER 0x0501		// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�98 ou version ult�rieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�Me ou version ult�rieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Internet Explorer�6.0 ou version ult�rieure.
#define _WIN32_IE 0x0600	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions d'Internet Explorer.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclure les en-t�tes Windows rarement utilis�s

// Fichiers d'en-t�te Windows�:
#include <windows.h>
#endif

#ifdef _MINGW_
#include <windows.h>
#endif

#ifdef EXPORT
#undef EXPORT
#endif

#ifdef PUBLIC
#undef PUBLIC
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

#include "sg2_typedef.h"
#include "sg2_err.h"
#include "sg2_date.h"
#include "sg2_heliocentric.h"
#include "sg2_geocentric.h"
#include "sg2_topocentric.h"

#include <stdio.h>

EXPORT S_SG2_SUNPOS *sg2_create_sunpos(unsigned long np, unsigned long nd,
		int *p_err);

EXPORT void sg2_delete_sunpos(S_SG2_SUNPOS *p_sunpos, int *p_err);

EXPORT S_SG2_TABTILT *sg2_create_tabtilt(unsigned long na, int *p_err);

EXPORT void sg2_set_tabtilt(double *p_azimuth, double *p_slope,
		S_SG2_TABTILT *p_tilt, int *p_err);

EXPORT void sg2_delete_tabtilt(S_SG2_TABTILT *p_tilt, int *p_err);

EXPORT S_SG2_TOA_IRRAD *sg2_create_toa_irrad(unsigned long na,
		unsigned long np, unsigned long nd, int *p_err);

EXPORT void sg2_delete_toa_irrad(S_SG2_TOA_IRRAD *p_toa_irrad, int *p_err);

EXPORT void sg2_set_sunpos(double *p_lon, double *p_lat, double *p_elevation,
		SG2_ELLPSTYPE ellpstype, double *p_data_ellps, double *p_jd_ut,
		double *p_delta_tt, SG2_CORRECTION_REFRACTION method,
		double *data_corr, S_SG2_SUNPOS *p_sunpos, int *p_err);

#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
