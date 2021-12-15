/******************************************************************************
* File:             main.cpp
*
* Author:           Andy Belle-Isle (drumsetmonkey)  
* Created:          12/04/21 
* Description:      ePaper Picture Frame
*****************************************************************************/

#include <boarddefs.hpp>

#include <GxEPD2.h>
#include <GxEPD2_7C.h>

//#include <demo_image.hpp>

#include <stdint.h>

#define GxEPD2_DISPLAY_CLASS GxEPD2_7C
#define GxEPD2_DRIVER_CLASS GxEPD2_565c

// somehow there should be an easier way to do this
#define GxEPD2_BW_IS_GxEPD2_BW false
#define GxEPD2_3C_IS_GxEPD2_3C false
#define GxEPD2_7C_IS_GxEPD2_7C true
#define GxEPD2_1248_IS_GxEPD2_1248 false

#define IS_GxEPD(c, x) (c##x)
#define IS_GxEPD2_BW(x) IS_GxEPD(GxEPD2_BW_IS_, x)
#define IS_GxEPD2_3C(x) IS_GxEPD(GxEPD2_3C_IS_, x)
#define IS_GxEPD2_7C(x) IS_GxEPD(GxEPD2_7C_IS_, x)
#define IS_GxEPD2_1248(x) IS_GxEPD(GxEPD2_1248_IS_, x)

#if defined(ESP32)
    #define MAX_DISPLAY_BUFFER_SIZE 65536ul // e.g.
#else
    #define MAX_DISPLAY_BUFFER_SIZE 448
#endif

#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
    #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS)
    #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
    #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif

GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> 
    display(GxEPD2_DRIVER_CLASS(config::board::epaper::CS,
                                config::board::epaper::DC,
                                config::board::epaper::RST,
                                config::board::epaper::BUSY));

#include <SD/sdcard.hpp>

void setup(void)
{
    // randomize changing the picture. Timer set to two hours, 24 hour change on
    // average is one in every twelve. However, this will be disabled at night,
    // say 8 hours. 24-8 = 16/2 is every one in 8

    Serial.begin(115200);
    display.init();
    config::board::spi::reset();

    display.setRotation(0);
    display.firstPage();

    if (sdcard::setup() != sdcard::error::SUCCESS)
    {
        // TODO: handle error on screen
    }
}

void loop(void)
{
    Serial.print("Random File: ");
    sdcard::File_t file;
    sdcard::randomFile(file);
    sdcard::close(file);
    Serial.println();

    delay(2500);
}

/*
    uint16_t displayColor = 0x0000;
    displayColor |= ((rgba[0] >> 3) << 11); // Red
    displayColor |= ((rgba[1] >> 2) <<  5); // Green
    displayColor |= ((rgba[2] >> 3) <<  0); // Blue
    display.fillRect(x, y, w, h, displayColor);
*/
