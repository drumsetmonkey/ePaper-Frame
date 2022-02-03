#include "epd_565c.hpp"

#include <stdlib.h>

EPD_565c::EPD_565c(int RST, int DC, int CS, int BUSY)
    :EPD(RST,DC,CS,BUSY)
{};

EPD_565c::~EPD_565c()
{};

/*******************************************************************************
*                                   PUBLIC                                    *
*******************************************************************************/

/**
 * @brief Setup the screen.
 * Puts the screen into operation mode
 *
 * @return[EPD::Error] EPD::Error::SUCCESS if everything succeeded
 */
EPD::Error EPD_565c::setup(void) const
{
    EPD::Error err = EPD::setup();
    if (err != EPD::Error::SUCCESS) // I don't love this
        return err;
    reset();

    busyHigh();
    sendCommand(0x00);
    sendData(0xEF);
    sendData(0x08);
    sendCommand(0x01);
    sendData(0x37);
    sendData(0x00);
    sendData(0x23);
    sendData(0x23);
    sendCommand(0x03);
    sendData(0x00);
    sendCommand(0x06);
    sendData(0xC7);
    sendData(0xC7);
    sendData(0x1D);
    sendCommand(0x30);
    sendData(0x3C);
    sendCommand(0x40);
    sendData(0x00);
    sendCommand(0x50);
    sendData(0x37);
    sendCommand(0x60);
    sendData(0x22);
    sendCommand(0x61);
    sendData(0x02);
    sendData(0x58);
    sendData(0x01);
    sendData(0xC0);
    sendCommand(0xE3);
    sendData(0xAA);
    EPD::delayMs(100);
    sendCommand(0x50);
    sendData(0x37);

    return EPD::Error::SUCCESS;
}

/**
 * @brief Sends data (parameters) to ePaper screen
 *
 * @param[data] The data to send
 */
void EPD_565c::sendData(uint8_t data) const
{
    EPD::digitalWrite(dc_pin, HIGH);
    EPD::transfer(data);
}


/**
 * @brief Put ePaper into deep sleep mode
 * Requires hardware reset to wake up. (EPD_565C::reset())
 */
void EPD_565c::sleep(void) const
{
    EPD::delayMs(100);
    sendCommand(0x07);
    sendData(0xA5);
    EPD::delayMs(100);
    EPD::digitalWrite(reset_pin, 0); // Reset
}

/**
 * @brief Clear the screen with a certain color.
 *
 * @param[color] The screen to clear the color with
 */
void EPD_565c::clear(EPD_565c::Color color) const
{
    start();
    for (unsigned int i = 0; i < width / 2; i++)
    {
        for (unsigned int j = 0; j < height; j++)
            sendData(((color & 0xF) << 4) | (color & 0xF)) ;
    }
    stop();
}

/**
 * @brief Reset the ePaper hardware
 * Use this to wake from sleep
 */
void EPD_565c::reset(void) const
{
    EPD::digitalWrite(reset_pin, LOW); // Force a hardware power down
    EPD::delayMs(1);
    EPD::digitalWrite(reset_pin, HIGH);
    EPD::delayMs(200);
}

/**
 * @brief Start a screen content transaction.
 * @warning This function must be called before sending any colors, then must be
 * followed up by stop()
 */
void EPD_565c::start(void) const
{
    sendCommand(0x61); // Full resolution
    sendData(0x02);
    sendData(0x58);
    sendData(0x01);
    sendData(0xC0);
    sendCommand(0x10);
}

/**
 * @brief Step the screen content transaction.
 * @warning This function must be called after sending colors. Must be preceded
 * by start(), then color data.
 */
void EPD_565c::stop(void) const
{
    sendCommand(0x04); //0x04
    busyHigh();
    sendCommand(0x12); //0x12
    busyHigh();
    sendCommand(0x02); //0x02
    busyLow();
    EPD::delayMs(200);
}

/**
 * @brief Display screen demo of all 8 colors.
 */
void EPD_565c::demo(void) const
{
    start();

    for (unsigned i = 0; i < 224; i++) {
        for (unsigned k = 0; k < 4; k++) {
            for (unsigned j = 0; j < 75; j++) {
                sendData((k << 4) | k);
            }
        }
    }
    for (unsigned i = 0; i < 224; i++) {
        for (unsigned k = 4; k < 8; k++) {
            for (unsigned j = 0; j < 75; j++) {
                sendData((k << 4) | k);
            }
        }
    }

    stop();
}

/*******************************************************************************
*                                   PRIVATE                                   *
*******************************************************************************/

/**
 * @brief Send a controll command to the ePaper screen
 *
 * @param[command] The command to send
 */
void EPD_565c::sendCommand(uint8_t command) const
{
    EPD::digitalWrite(dc_pin, LOW);
    EPD::transfer(command);
}

/**
 * @brief Wait for busy line to return high
 */
void EPD_565c::busyHigh(void) const
{
    while (!(EPD::digitalRead(busy_pin)));
}

/**
 * @brief Wait for busy line to return low
 */
void EPD_565c::busyLow(void) const
{
    while (EPD::digitalRead(busy_pin));
}
