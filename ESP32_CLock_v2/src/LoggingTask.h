/***********************************************************************
*! \file:                   loggingTask.h
*  \projekt:                Ring Clock
*  \created on:             2024 05 31
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/


#ifndef _loggingTask_H_
#define _loggingTask_H_


/***********************************************************************
 * Includes
 **********************************************************************/
#include "esp_log.h"
#include "stdint.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array


/***********************************************************************
 * Declarations
 **********************************************************************/

struct logging_information {

    const char* fct_name;
    uint8_t logging_level;
    const char* log_msg;

};

enum log_level_t{

    log_lvl_debug = 0 ,
    log_lvl_verbose,
    log_lvl_information,
    log_lvl_warning,
    log_lvl_errors
    
} ;
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

void setup_logging(esp_log_level_t log_level);
//void write_to_log_old(struct logging_information *infomation);
void write_to_log(log_level_t level, const char* fct, const char* msg);

 




#endif /* _FT800_HAL.h_H_ */

