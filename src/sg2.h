/*
 * sg2.h
 *
 *  Created on: 2 févr. 2011
 *      Author: pblanc
 */

#ifndef SG2_H_
#define SG2_H_

#ifdef _MINGW_
#include <windows.h>
#endif

#ifdef EXPORT
#undef EXPORT
#endif

#ifdef _VISUAL_
#define EXPORT _declspec (dllexport)
#else
#define EXPORT
#endif

#ifdef  __cplusplus
extern "C"
{
#endif

#include "SG2_err.h"
#include "SG2_typedef.h"
#include "SG2_date.h"
#include "sg2_heliocentric_coordinates.h"

#ifdef	__cplusplus
}
#endif

#endif /* SG2_H_ */
