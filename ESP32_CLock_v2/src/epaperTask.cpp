/***********************************************************************
*! \file:                   main.cpp
*  \projekt:                Ring Clock epaper handler
*  \created on:             2024 05 31
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
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
#include "LoggingTask.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://os.mbed.com/platforms/FRDM-K64F/#board-pinout
/***********************************************************************
* Declarations
***********************************************************************/



/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Global Variable
***********************************************************************/
//Create a new image cache named IMAGE_BW and fill it with white
UBYTE *BlackImage, *RYImage; // Red or Yellow
UWORD Imagesize = ((EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1)) * EPD_2IN9B_V3_HEIGHT;


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
*! \fn          void init_epaper(void)
*  \brief       start up function for epaper
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void init_epaper(void){
   
    EPD_2IN9B_V3_Init();
    EPD_2IN9B_V3_Clear();
    DEV_Delay_ms(500);
    char str_buffer[128];

    
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        //sprintf(str_buffer, "[WiFi-event] event: %d\n", event);
        write_to_log(log_lvl_errors, "E-Paper" , "Failed to apply for black memory..."); 
        while(1);
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        write_to_log(log_lvl_errors, "E-Paper" , "Failed to apply for red memory..."); 
        while(1);
    }
    
    printf("NewImage:BlackImage and RYImage\r\n");
    write_to_log(log_lvl_information, "E-Paper" , "NewImage:BlackImage and RYImage"); 


    Paint_NewImage(BlackImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    
    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    
    // show image for array    
    write_to_log(log_lvl_information, "E-Paper" , "show image for array"); 
    EPD_2IN9B_V3_Display(gImage_2in9bc_b, gImage_2in9bc_ry);
    DEV_Delay_ms(2000);
    
    //1.Draw black image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 0, "E-Papaer LED Ring Clock", &Font16, BLACK, WHITE);

    //2.Draw red image
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 50, "...booting...", &Font20, BLACK, WHITE);

    EPD_2IN9B_V3_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);
    //EPD_2IN9B_V3_Sleep(); 

    DEV_Delay_ms(10000);

    Paint_NewImage(BlackImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE); 
    EPD_2IN9B_V3_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);
    EPD_2IN9B_V3_Sleep(); 

}


/***********************************************************************
*! \fn          void set_info_epaper(struct_info_epaper infomation);
*  \brief       display the information on the epaper
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void set_info_epaper(const DateTime& dt){

    EPD_2IN9B_V3_Init();
    EPD_2IN9B_V3_Clear();
    DEV_Delay_ms(500);
    char datestring[128];
    char str_buffer[128];
    char day_buf[32];

    switch (dt.dayOfTheWeek()){
        case 1:
            sprintf(day_buf, "Montag");
            break;
        case 2:
            sprintf(day_buf, "Dienstag");
            break;
        case 3:
            sprintf(day_buf, "Mittwoch");
            break;
        case 4:
            sprintf(day_buf, "Donnerstag");
            break;
        case 5:
            sprintf(day_buf, "Freitag");
            break;
        case 6:
            sprintf(day_buf, "Samstag");
            break;
        case 0:
            sprintf(day_buf, "Sonntag");
            break;
        default:
            sprintf(day_buf, "Bergfest :-)");
    }

    write_to_log(log_lvl_information, "E-Paper" , "show information"); 

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u.%02u.%04u %s"),
            dt.month(),
            dt.day(),
            dt.year(),
            day_buf );
    //Serial.println(datestring);


    //Create a new image cache named IMAGE_BW and fill it with white
    //UBYTE *BlackImage, *RYImage; // Red or Yellow
    //UWORD Imagesize = ((EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1)) * EPD_2IN9B_V3_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        write_to_log(log_lvl_errors, "E-Paper" , "Failed to apply for black memory"); 
        while(1);
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        write_to_log(log_lvl_errors, "E-Paper" , "Failed to apply for red memory"); 
        while(1);
    }
    write_to_log(log_lvl_information, "E-Paper" , "NewImage:BlackImage and RYImage"); 
    Paint_NewImage(BlackImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);

    //clean
    //1.Draw black image

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);

    Paint_DrawString_EN(20, 20, datestring, &Font20, WHITE, BLACK);

    EPD_2IN9B_V3_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);
    //EPD_2IN9B_V3_Sleep(); 

    //DEV_Delay_ms(10000);

    EPD_2IN9B_V3_Sleep(); 
    sprintf(str_buffer, "Set time to %s", datestring );
    write_to_log(log_lvl_information, "e - Paper", str_buffer);

}

