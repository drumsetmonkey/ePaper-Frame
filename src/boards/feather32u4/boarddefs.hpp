/******************************************************************************
* File:             boarddefs.hpp
*
* Author:           Andy Belle-Isle (drumsetmonkey)  
* Created:          12/04/21 
* Description:      Adafruit 32u4 Feather Board Definitions
*****************************************************************************/

#pragma once

#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>

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
            const int CS   = A5;
            const int DC   = A4;
            const int RST  = A3;
            const int BUSY = A2;
        }

        /*************************
        *  SPI PIN DEFINITIONS  *
        *************************/
        namespace spi {
            const int SCK  = SCK;
            const int MISO = MISO;
            const int MOSI = MOSI;

            void reset(void)
            {
                SPI.end();
                SPI.begin();
            }
        }

        /**********************
        * SD CARD DEFINITIONS *
        ***********************/
        namespace sdcard {
            const int CS = 4;
            const int CD = 7;
        }

        
        /************************
        * POWER PIN DEFINITIONS *
        ************************/
        namespace power {
            const int DONE    = 5;
            const int BAT     = A9; // Pin 7
            const int LIGHT   = A7; // Pin 6
            const int RANDPIN = A1; // A1
            const int FORCE   = 10;

            const float LOW_BATTERY = 3.45;
        }
    }
}

