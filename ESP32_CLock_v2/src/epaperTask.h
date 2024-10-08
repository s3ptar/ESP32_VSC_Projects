/***********************************************************************
*! \file:                   epaperTask.h
*  \projekt:                Ring Clock
*  \created on:             2024 05 31
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/


#ifndef _epaperTask_H_
#define _epaperTask_H_


/***********************************************************************
 * Includes
 **********************************************************************/
#include "Time.h"
#include <stdlib.h>
#include <Arduino.h>
#include "RTClib.h"
#include "DEV_Config.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array


/***********************************************************************
 * Declarations
 **********************************************************************/

/***********************************************************************
 * Global Variable
 **********************************************************************/


/***********************************************************************
 * Constant
 **********************************************************************/


/***********************************************************************
 * Macros
 **********************************************************************/


/***********************************************************************
 * Funtions
 **********************************************************************/

void init_epaper(void);
void set_info_epaper(const DateTime& dt);




#endif /* _FT800_HAL.h_H_ */

