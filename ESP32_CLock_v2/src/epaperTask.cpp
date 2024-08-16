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

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage; // Red or Yellow
    UWORD Imagesize = ((EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1)) * EPD_2IN9B_V3_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        while(1);
    }
    
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    
    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    
    // show image for array    
    printf("show image for array\r\n");
    EPD_2IN9B_V3_Display(gImage_2in9bc_b, gImage_2in9bc_ry);
    DEV_Delay_ms(2000);
    
    //1.Draw black image
    printf("BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 0, "E-Papaer LED Ring Clock", &Font16, BLACK, WHITE);

    //2.Draw red image
    printf("RYImage\r\n");
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
*! \fn          void epaper_show_information
*  \brief       display the information on the epaper
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void epaper_show_information(){

    EPD_2IN9B_V3_Init();
    EPD_2IN9B_V3_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage; // Red or Yellow
    UWORD Imagesize = ((EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1)) * EPD_2IN9B_V3_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        while(1);
    }
    
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9B_V3_WIDTH, EPD_2IN9B_V3_HEIGHT, 270, WHITE);

    //clean
    //1.Draw black image
    printf("BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 0, "E-Papaer LED Ring Clock", &Font16, BLACK, WHITE);

}

