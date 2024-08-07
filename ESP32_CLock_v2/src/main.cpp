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

#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"

#include "epaperTask.h"
#include "DEV_Config.h"
#include <stdlib.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "RTClib.h"
#include <WiFi.h>
#include "time.h"
#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <config_data.h>

#include <filesystem.h>
#include <ESPNtpClient.h>
#include "LoggingTask.h"
#include "esp_log.h"
#include "LED_RingTask.h"

/***********************************************************************
* Informations
***********************************************************************/
//https://www.waveshare.com/wiki/E-Paper_ESP32_Driver_Board
//https://files.waveshare.com/upload/8/80/E-Paper_ESP32_Driver_Board_Schematic.pdf
//https://microcontrollerslab.com/esp32-ds1307-real-time-clock-rtc-oled/
//https://github.com/gmag11/NtpClient/blob/master/examples/NTPClientESP32/NTPClientESP32.ino
//board_build.filesystem = littlefs
//https://www.bing.com/images/search?view=detailV2&ccid=1nP4%2fWBC&id=B2BCFF8B182D76770AB4812E6973054BC467AAC3&thid=OIP.1nP4_WBClfQR0vL8HcnsNwHaES&mediaurl=https%3a%2f%2ffile.vishnumaiea.in%2fdownload%2fesp32%2fESP32-Devkit-Pinout-Rev-12-9600p.png&cdnurl=https%3a%2f%2fth.bing.com%2fth%2fid%2fR.d673f8fd604295f411d2f2fc1dc9ec37%3frik%3dw6pnxEsFc2kugQ%26pid%3dImgRaw%26r%3d0&exph=5569&expw=9600&q=esp32+pinout&simid=608015860527427334&FORM=IRPRST&ck=46B3AD8587E3E7AF133BC4CA002C2833&selectedIndex=0&itb=0&idpp=overlayview&ajaxhist=0&ajaxserp=0

/*Pinning
SCL = GPIO22
SDA = GPIO21
LED = GPIO18

*/

/***********************************************************************
* Declarations
***********************************************************************/

RTC_DS1307 DS1307_RTC;
#define NTP_TIMEOUT 5000
#define SHOW_TIME_PERIOD 1000
#define REFRESH_LED_TIME_PERIOD 10000
/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Global Variable
***********************************************************************/
static const char* tag = "Zeitserver";
bool filesystemOK = false;
bool RTC_Ok = true;
boolean syncEventTriggered = false; // True if a time even has been triggered
NTPEvent_t ntpEvent; // Last triggered event
double offset;
double timedelay;
bool wifiFirstConnected = false;
const PROGMEM char* ntpServer = "pool.ntp.org";


/***********************************************************************
* local Variable
***********************************************************************/



/***********************************************************************
*! \fn          void onWifiEvent (arduino_event_id_t event, arduino_event_info_t info) 
*  \brief       event handler wifi
*  \param       arduino_event_id_t
*  \param       arduino_event_info_t
*  \exception   none
*  \return      none
***********************************************************************/
void onWifiEvent (arduino_event_id_t event, arduino_event_info_t info) {

    Serial.printf ("[WiFi-event] event: %d\n", event);

    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.printf ("Connected to %s. Asking for IP address.\r\n", info.wifi_sta_connected.ssid);
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.printf ("Got IP: %s\r\n", IPAddress (info.got_ip.ip_info.ip.addr).toString ().c_str ());
            Serial.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
            //digitalWrite (ONBOARDLED, LOW); // Turn on LED
            wifiFirstConnected = true;
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.printf ("Disconnected from SSID: %s\n", info.wifi_sta_disconnected.ssid);
            Serial.printf ("Reason: %d\n", info.wifi_sta_disconnected.reason);
            //digitalWrite (ONBOARDLED, HIGH); // Turn off LED
            //NTP.stop(); // NTP sync can be disabled to avoid sync errors
            WiFi.reconnect ();
            break;
        default:
            break;
    }
}

/***********************************************************************
*! \fn          void syncRTC()
*  \brief       store ntp time in rtc
*  \param       ntpEvent
*  \exception   none
*  \return      none
***********************************************************************/
void syncRTC(){
    struct tm tm;
    if(!getLocalTime(&tm)){
        Serial.println("Failed to obtain time");
        return;
    }
    DS1307_RTC.adjust(DateTime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec));

}

/***********************************************************************
*! \fn          void processSyncEvent (NTPEvent_t ntpEvent)
*  \brief       ntp event handler
*  \param       ntpEvent
*  \exception   none
*  \return      none
***********************************************************************/
void processSyncEvent (NTPEvent_t ntpEvent) {
    switch (ntpEvent.event) {
        case timeSyncd:
            Serial.printf ("[NTP-event] %s\n", NTP.ntpEvent2str (ntpEvent));
            //DS1307_RTC.adjust(DateTime(2023, 6, 18, (timeClient.getHours()), (timeClient.getMinutes()), (timeClient.getSeconds())));
            Serial.println("NTP synced to RTC");
            syncRTC();
        case partlySync:
        case syncNotNeeded:
        case accuracyError:
            Serial.printf ("[NTP-event] %s\n", NTP.ntpEvent2str (ntpEvent));
            break;
        default:
            break;
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

    setup_logging(ESP_LOG_VERBOSE);
    Serial.println("ESP32 Clock with EPaper");


    Serial.println("----------------------------------------");
    Serial.println("              Read Config               ");
    Serial.println("----------------------------------------");
    ReadConfig(LittleFS, "/config.json");


    Serial.println("----------------------------------------");
    Serial.println("              Check E - Paper           ");
    Serial.println("----------------------------------------");

    //init_epaper();

    /*
    ######## start check Wlan, if nossid found start ap mode
    */
    const char* wlan_ssid = _wlan_ssid_;
    const char* wlan_pass = _wlan_pass_;
    WiFi.begin(wlan_ssid, wlan_pass);
    //WiFi.begin("chilihotdog24" ,"bxJHckMMkGqEPfY3Jf3nZnAn5FtGYwKZSkzVvbzFHNbpUZfv79GXm8afDuNu");
    /*while(WiFi.status() != WL_CONNECTED){
      DEV_Delay_ms(500); 
      Serial.print(".");
    } */
    Serial.println("WiFi connected");
    WiFi.onEvent (onWifiEvent);

    //start ntp
    NTP.onNTPSyncEvent ([] (NTPEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });

    led_ring_setup();
    
    //check RTC
    if (!DS1307_RTC.begin()) {
        Serial.println("Couldn't find RTC");
        RTC_Ok = false;
    }else {
        DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
        Serial.println("RTC Okay");
    }


    
}

/* The main loop -------------------------------------------------------------*/
void loop(){

    static int i = 0;
    static int last = 0;

    if (wifiFirstConnected) {
        wifiFirstConnected = false;
        NTP.setTimeZone (TZ_Europe_Berlin);
        NTP.setInterval (600);
        NTP.setNTPTimeout (NTP_TIMEOUT);
        // NTP.setMinSyncAccuracy (5000);
        // NTP.settimeSyncThreshold (3000);
        NTP.begin (ntpServer);
    }

    if (syncEventTriggered) {
        syncEventTriggered = false;
        processSyncEvent (ntpEvent);
    }

    //led_ring_set_time();



    if ((millis () - last) > REFRESH_LED_TIME_PERIOD) {
        last = millis ();
        //Serial.print (i); Serial.print (" ");
        //ESP_LOGI(tag, "time is on your side");
        //Serial.println("write to log");
        //Serial.print ("WiFi is ");
        //Serial.print (WiFi.isConnected () ? "connected" : "not connected"); Serial.print (". ");
        //Serial.print ("Uptime: ");
        //Serial.print (NTP.getUptimeString ()); Serial.print (" since ");
        //Serial.println (NTP.getTimeDateString (NTP.getFirstSyncUs ()));
        //Serial.printf ("Free heap: %u\n", ESP.getFreeHeap ());
        DateTime dt = DS1307_RTC.now();
        led_ring_task(dt);
        i++;
        
    } 
  
}
