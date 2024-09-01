/***********************************************************************
*! \file:                   main.cpp
*  \projekt:                Ring Clock logging handler
*  \created on:             2024 05 31
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/


/***********************************************************************
* Includes
***********************************************************************/
#include "LoggingTask.h"
#include "esp_log.h"
#include <stdlib.h>
#include <Arduino.h>
/***********************************************************************
* Informations
***********************************************************************/
//https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/log.html
/***********************************************************************
* Declarations
***********************************************************************/



/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Global Variable
***********************************************************************/

uint32_t msg_cnt = 0;
uint8_t set_log_level = 0;

/***********************************************************************
* local Variable
***********************************************************************/


/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Local Funtions
***********************************************************************/
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define runtime() millis ()

/***********************************************************************
*! \fn          void setup_logging(esp_log_level_t log_level)
*  \brief       set the logging level
*  \param       uint8_t log_level
*  \exception   none
*  \return      none
***********************************************************************/
void setup_logging(esp_log_level_t log_level){

    set_log_level = log_level;

}

/***********************************************************************
*! \fn          void setup_logging(uint8_t log_level)
*  \brief       set the logging level
*  \param       uint8_t log_level
*  \exception   none
*  \return      none
***********************************************************************/
void write_to_log(log_level_t level, const char* fct, const char* msg){

    char datestring[256];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%06u,%010u,%04u,%s,%s"),
            msg_cnt,
            runtime(),
            level,
            fct,
            msg
            );
    Serial.println(datestring);

    msg_cnt++;

}