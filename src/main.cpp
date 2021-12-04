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
#include <pngle.h>

#include <demo_image.hpp>

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

void init_screen(pngle_t*, uint32_t, uint32_t);
void draw_pixel(pngle_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t[4]);
void finish_screen(pngle_t*);

void setup(void)
{
    display.init(115200);
    SPI.end();
    SPI.begin(config::board::spi::SCK,
              config::board::spi::MISO,
              config::board::spi::MOSI,
              config::board::spi::SS);

    display.setRotation(0);
    display.firstPage();

    do {
        pngle_t *pngle = pngle_new();
        pngle_set_init_callback(pngle, init_screen);
        pngle_set_draw_callback(pngle, draw_pixel);
        pngle_set_done_callback(pngle, finish_screen);

        pngle_feed(pngle, IMAGE_DATA, IMAGE_SIZE);
    } while (display.nextPage());

}

void loop(void)
{

}

void init_screen(pngle_t *pngle, uint32_t w, uint32_t h)
{
}

void draw_pixel(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
    uint16_t displayColor = 0x0000;
    displayColor |= ((rgba[0] >> 3) << 11); // Red
    displayColor |= ((rgba[1] >> 2) <<  5); // Green
    displayColor |= ((rgba[2] >> 3) <<  0); // Blue
    display.fillRect(x, y, w, h, displayColor);
}

void finish_screen(pngle_t *pngle)
{
}
