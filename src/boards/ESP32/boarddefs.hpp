/******************************************************************************
* File:             boarddefs.hpp
*
* Author:           Andy Belle-Isle (drumsetmonkey)  
* Created:          12/04/21 
* Description:      ESP32 Board Definitions
*****************************************************************************/

#pragma once

#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>

/**********************
*  CONFIG NAMESPACE  *
**********************/
namespace config {

    /******************
    *  BOARD CONFIG  *
    ******************/
    namespace board {

        /***************************
        *  EPAPER PIN DEFINITIONS  *
        ****************************/
        namespace epaper {
            const int CS   = 15;
            const int DC   = 27;
            const int RST  = 26;
            const int BUSY = 25;
        }

        /*************************
        *  SPI PIN DEFINITIONS  *
        *************************/
        namespace spi {
            const int SCK  = 13;
            const int MISO = 12;
            const int MOSI = 14;
            void reset(void)
            {
                SPI.end();
                SPI.begin(spi::SCK,
                          spi::MISO,
                          spi::MOSI,
                          epaper::CS);
            }
        }

        /**********************
        * SD CARD DEFINITIONS *
        ***********************/
        namespace sdcard {
            const int CS = 22;
            const int CD = 23;
        }
    }
}

