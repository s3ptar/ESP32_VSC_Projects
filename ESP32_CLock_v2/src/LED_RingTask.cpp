/***********************************************************************
*! \file:                   LED_RingTask.cpp
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
#include "LED_RingTask.h"
#include "esp_log.h"
#include <stdlib.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "DEV_Config.h"
#include "Time.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/log.html
/***********************************************************************
* Declarations
***********************************************************************/

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 18, NEO_GRB + NEO_KHZ800);

/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Global Variable
***********************************************************************/


/***********************************************************************
* local Variable
***********************************************************************/


/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Local Funtions
***********************************************************************/



/***********************************************************************
*! \fn          void colorWipe(uint32_t c, uint16_t wait)
*  \brief       Fill the dots one after the other with a color
*  \param       uint32_t c - color
*  \param       uint16_t wait - time for wait
*  \exception   none
*  \return      none
***********************************************************************/
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    DEV_Delay_ms(wait);
  }
}

/***********************************************************************
*! \fn          void led_ring_setup()
*  \brief       test led ring
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void led_ring_setup(){
    //setup led and test
    strip.begin();
    strip.setBrightness(100);
    strip.show(); // Initialize all pixels to 'off'
    colorWipe(strip.Color(255, 0, 0), 200); // Red
    colorWipe(strip.Color(0, 255, 0), 200); // Green
    colorWipe(strip.Color(0, 0, 255), 200); // Blue
    colorWipe(strip.Color(0, 0, 0), 200); // off
    strip.show();
}

/***********************************************************************
*! \fn          void led_ring_task()
*  \brief       set time to led ring
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void led_ring_task(){
    //setup led and test
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    uint8_t hour = timeinfo.tm_hour;
    uint8_t minutes = timeinfo.tm_min;

    char outpu_buf[64];
    sprintf(outpu_buf, "hour : %d and minutes %d", hour, minutes);

    Serial.println(outpu_buf);


  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}