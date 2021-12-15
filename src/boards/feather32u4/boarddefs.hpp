/******************************************************************************
* File:             boarddefs.hpp
*
* Author:           Andy Belle-Isle (drumsetmonkey)  
* Created:          12/04/21 
* Description:      Adafruit 32u4 Feather Board Definitions
*****************************************************************************/

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
    }
}

