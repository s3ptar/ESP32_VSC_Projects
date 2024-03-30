/***********************************************************************
*! \file:                   main.cpp
*  \projekt:                ESP32 Clock with epaper
*  \created on:             2024 03 24
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   extend E-Paper ESP32 Driver Board from 
*                           waveshare
***********************************************************************/


/***********************************************************************
* Includes
***********************************************************************/

#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "RTClib.h"
#include "secrets.h"
#include <WiFi.h>
#include "time.h"
#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <filesystem.h>

/***********************************************************************
* Informations
***********************************************************************/
//https://www.waveshare.com/wiki/E-Paper_ESP32_Driver_Board
//https://files.waveshare.com/upload/8/80/E-Paper_ESP32_Driver_Board_Schematic.pdf
//https://microcontrollerslab.com/esp32-ds1307-real-time-clock-rtc-oled/
//;board_build.filesystem = littlefs
//https://www.bing.com/images/search?view=detailV2&ccid=1nP4%2fWBC&id=B2BCFF8B182D76770AB4812E6973054BC467AAC3&thid=OIP.1nP4_WBClfQR0vL8HcnsNwHaES&mediaurl=https%3a%2f%2ffile.vishnumaiea.in%2fdownload%2fesp32%2fESP32-Devkit-Pinout-Rev-12-9600p.png&cdnurl=https%3a%2f%2fth.bing.com%2fth%2fid%2fR.d673f8fd604295f411d2f2fc1dc9ec37%3frik%3dw6pnxEsFc2kugQ%26pid%3dImgRaw%26r%3d0&exph=5569&expw=9600&q=esp32+pinout&simid=608015860527427334&FORM=IRPRST&ck=46B3AD8587E3E7AF133BC4CA002C2833&selectedIndex=0&itb=0&idpp=overlayview&ajaxhist=0&ajaxserp=0
/***********************************************************************
* Declarations
***********************************************************************/

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 18, NEO_GRB + NEO_KHZ800);
RTC_DS1307 DS1307_RTC;

/***********************************************************************
* Constant
***********************************************************************/

char Week_days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;


/***********************************************************************
* Global Variable
***********************************************************************/
bool filesystemOK = false;

/***********************************************************************
* local Variable
***********************************************************************/

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    DEV_Delay_ms(wait);
  }
}


/***********************************************************************
*! \fn          int main(){
*  \brief       start up function
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void print_multiple(const char* sign_to_print,  uint8_t num_of_print){
    while(num_of_print--)
        Serial.print(sign_to_print);
}


/* Entry point ----------------------------------------------------------------*/
void setup()
{
    Serial.println("ESP32 Clock with EPaper");

    DEV_Module_Init();
    Serial.println("----------------------------------------");
    Serial.println("           Check File System            ");
    Serial.println("----------------------------------------");
    if (!LittleFS.begin(false /* false: Do not format if mount failed */)) {
        Serial.println("Failed to mount LittleFS");
        if (!LittleFS.begin(true /* true: format */)) {
            Serial.println("Failed to format LittleFS");
        } else {
            Serial.println("LittleFS formatted successfully");
            filesystemOK = true;
        }
  } else { // Initial mount success
      filesystemOK = true;
      Serial.println("LittleFS formatted successfully");
      listDir(LittleFS, "/", 0);
  }



    /*
    ######## start check Wlan, if nossid found start ap mode
    */
    WiFi.begin(_secrect_ssid_, _secrect_wlan_pass_);
    while(WiFi.status() != WL_CONNECTED){
      DEV_Delay_ms(500); 
      Serial.print(".");
    } 
    Serial.println("WiFi connected");




    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    //setup led and test
    strip.begin();
    strip.setBrightness(100);
    strip.show(); // Initialize all pixels to 'off'
    colorWipe(strip.Color(255, 0, 0), 200); // Red
    colorWipe(strip.Color(0, 255, 0), 200); // Green
    colorWipe(strip.Color(0, 0, 255), 200); // Blue
    colorWipe(strip.Color(0, 0, 0), 200); // off
    strip.show();
    
    //check RTC
    if (!DS1307_RTC.begin()) {
      Serial.println("Couldn't find RTC");
      while(1);
    }
    DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

    Serial.println("Setup End\r\n");
    
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
    //printf("loop\r\n");
    DateTime now = DS1307_RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(Week_days[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    DEV_Delay_ms(20000);
  // 
}
