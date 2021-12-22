/******************************************************************************
* File:             main.cpp
*
* Author:           Andy Belle-Isle (drumsetmonkey)  
* Created:          12/04/21 
* Description:      ePaper Picture Frame
*****************************************************************************/

#include <boarddefs.hpp>

#include <power/power.hpp>
#include <SD/sdcard.hpp>
#include <SD/raw.hpp>

#include <epd5in65f/epd5in65f.hpp>

#define DEBUG (0)
#if DEBUG
#define dprint(x) Serial.println(x)
#else
#define dprint(x)
#endif

bool forceChange = false;

void setup(void)
{
    pinMode(config::board::power::FORCE, INPUT);
    if (digitalRead(config::board::power::FORCE)) {
        forceChange = true;
    }

    if (DEBUG == 1) {
        Serial.begin(115200);
        dprint("Booting...");
        delay(5000);
        dprint("Starting");
    }
    power::setup();

    /* If the force change button was pressed, do not shut off */
    if (!forceChange) {

        /* If it is light, go to sleep */
        if (false && power::light()) {
            dprint("There is light, no update");
            power::shutoff();
        }

        /* Randomize changing the picture. Timer set to two hours, 24 hour change on
        * average is one in every twelve. However, this will be disabled at night,
        * say 8 hours. 24-8 = 16/2 is every one in 8
        */
        int randLoop = random(0, 4);
        int randDraw = random(32767);
        for (int i = 0; i < randLoop; i++)
            randDraw = random(32000);
        dprint(randDraw);
        if (randDraw > 8156) {
            dprint("Nope no picture this time");
            power::shutoff();
        }
    } // !forceChange

    /* Initialize the epaper */
    EPD_565c epd(config::board::epaper::RST,
                 config::board::epaper::DC,
                 config::board::epaper::CS,
                 config::board::epaper::BUSY);

    if (epd.setup() != EPD_Base::error::SUCCESS) {
        power::shutoff();
    }

    /* If the battery is low, display an error and shut off */
    if (power::battery_voltage() <= config::board::power::LOW_BATTERY) {
        epd.clear(0x4); // Red is low battery
        power::shutoff();
    }

    /* Set up the SD card, if failure, display error */
    sdcard::error newerror = sdcard::setup();
    if (newerror != sdcard::error::SUCCESS) {
        if (newerror == sdcard::error::NO_CARD) {
            epd.clear(0x3); // Blue is no SD card
        }

        epd.clear(0x2); // Green is SD error
        power::shutoff();
    }

    /* Find random file and open it it */
    sdcard::File_t file;
    if (sdcard::randomFile(file) != sdcard::error::SUCCESS) {
        epd.clear(0x0); // Black is no random file
        power::shutoff();
    };

    /* Create and validate the image */
    RAW img(file);
    if (img.validate(0x565c) != RAW::error::VALID) {
        epd.clear(0x5); // Yellow is invalid image
        power::shutoff();
    }

    /* Draw the image */
    img.seekData();
    epd.start();
    for (int i = 0; i < 448; i++)
    {
        for (int j = 0; j < 600 / 2; j++) {
            epd.sendData(img.readRaw());
        }
    }
    epd.stop();

    /* Cleanup */
    sdcard::close(file); // Cya
    power::shutoff();
}

/* We should never get here */
void loop(void) {
    if (DEBUG) {
        setup();
    }
    power::shutoff(); // Just in case, shut down
}
