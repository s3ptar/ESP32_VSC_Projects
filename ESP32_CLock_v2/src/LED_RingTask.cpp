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
#include "LoggingTask.h"
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
#define countof(a) (sizeof(a) / sizeof(a[0]))


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
*! \fn          void led_ring_task(const DateTime& dt)
*  \brief       set time to led ring
*  \param       const dateTime& dt
*  \exception   none
*  \return      none
***********************************************************************/
void led_ring_task(const DateTime& dt){
    //setup led and test
    /*struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    

    char outpu_buf[64];
    sprintf(outpu_buf, "hour : %d and minutes %d", hour, minutes);

    Serial.println(outpu_buf);*/

    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.month(),
            dt.day(),
            dt.year(),
            dt.hour(),
            dt.minute(),
            dt.second() );
    //Serial.println(datestring);

    
    uint8_t hour = dt.hour();
    //wenn größer 12 dann halbieren
    if(hour > 12)
        hour -= 12;
    //led positions korrektur 0 = 7 Uhr
    switch(hour){
      case 0:
          hour = 5;
          break;
      case 1:
          hour = 6;
          break;
      case 2:
          hour = 7;
          break;
      case 3:
          hour = 8;
          break;
      case 4:
          hour = 9;
          break;
      case 5:
          hour = 10;
          break;
      case 6:
          hour = 11;
          break;
      case 7:
          hour = 0;
          break;
      case 8:
          hour = 1;
          break;
      case 9:
          hour = 2;
          break;
      case 10:
          hour = 3;
          break;
      case 11:
          hour = 4;
          break;

    }
    uint8_t minutes = dt.minute();
    
    switch(minutes){
      case 0 ... 4:
          minutes = 5;
          break;
      case 5 ... 9:
          minutes = 6;
          break;
      case 10 ... 14:
          minutes = 7;
          break;
      case 15 ... 19:
          minutes = 8;
          break;
      case 20 ... 24:
          minutes = 9;
          break;
      case 25 ... 29:
          minutes = 10;
          break;
      case 30 ... 34:
          minutes = 11;
          break;
      case 35 ... 39:
          minutes = 0;
          break;
      case 40 ... 44:
          minutes = 1;
          break;
      case 45 ... 49:
          minutes = 2;
          break;
      case 50 ... 54:
          minutes = 3;
          break;
      case 55 ... 59:
          minutes = 4;
          break;

    }

    

    strip.clear();
    strip.setPixelColor(minutes, strip.Color(0, 0, 255));
    strip.setPixelColor(hour, strip.Color(0, 255, 0));
    strip.show();


  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}