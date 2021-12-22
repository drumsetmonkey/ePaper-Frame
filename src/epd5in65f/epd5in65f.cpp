/*****************************************************************************
* | File      	:   EPD_5in65f.c
* | Author      :   Waveshare team
* | Function    :   5.65inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-08
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#include <stdlib.h>
#include "epd5in65f.h"

EPD_565c::~EPD_565c()
{};

EPD_565c::EPD_565c(int RST, int DC, int CS, int BUSY)
    :EPD_Base(RST,DC,CS,BUSY)
{};

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
int EPD_565c::setup(void)
{
    if (BaseInit() != 0)
    {
        return -1;
    }
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
    DelayMs(100);
    sendCommand(0x50);
    sendData(0x37);

    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void EPD_565c::sendCommand(unsigned char command)
{
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void EPD_565c::sendData(unsigned char data)
{
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

void EPD_565c::busyHigh(void) // If BUSYN=0 then waiting
{
    while (!(DigitalRead(busy_pin)));
}

void EPD_565c::busyLow(void) // If BUSYN=1 then waiting
{
    while (DigitalRead(busy_pin));
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see EPD_565c::Sleep();
 */
void EPD_565c::reset(void)
{
    DigitalWrite(reset_pin, LOW); //module reset
    DelayMs(1);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
}

/******************************************************************************
function :	show 7 kind of color block
parameter:
******************************************************************************/
void EPD_565c::demo(void)
{
    unsigned long i, j, k;
    unsigned char const Color_seven[8] =
        {EPD_5IN65F_BLACK, EPD_5IN65F_BLUE, EPD_5IN65F_GREEN, EPD_5IN65F_ORANGE,
         EPD_5IN65F_RED, EPD_5IN65F_YELLOW, EPD_5IN65F_WHITE, EPD_5IN65F_CLEAN};
    sendCommand(0x61); //Set Resolution setting
    sendData(0x02);
    sendData(0x58);
    sendData(0x01);
    sendData(0xC0);
    sendCommand(0x10);

    for (i = 0; i < 224; i++)
    {
        for (k = 0; k < 4; k++)
        {
            for (j = 0; j < 75; j++)
            {
                sendData((Color_seven[k] << 4) | Color_seven[k]);
            }
        }
    }
    for (i = 0; i < 224; i++)
    {
        for (k = 4; k < 8; k++)
        {
            for (j = 0; j < 75; j++)
            {
                sendData((Color_seven[k] << 4) | Color_seven[k]);
            }
        }
    }
    sendCommand(0x04); //0x04
    busyHigh();
    sendCommand(0x12); //0x12
    busyHigh();
    sendCommand(0x02); //0x02
    busyLow();
    DelayMs(200);
}

/******************************************************************************
function : 
      Clear screen
******************************************************************************/
void EPD_565c::clear(uint8_t color)
{
    sendCommand(0x61); //Set Resolution setting
    sendData(0x02);
    sendData(0x58);
    sendData(0x01);
    sendData(0xC0);
    sendCommand(0x10);
    for (unsigned int i = 0; i < width / 2; i++)
    {
        for (unsigned int j = 0; j < height; j++)
            sendData((color << 4) | color);
    }
    sendCommand(0x04); //0x04
    busyHigh();
    sendCommand(0x12); //0x12
    busyHigh();
    sendCommand(0x02); //0x02
    busyLow();
    DelayMs(500);
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void EPD_565c::sleep(void)
{
    DelayMs(100);
    sendCommand(0x07);
    sendData(0xA5);
    DelayMs(100);
    DigitalWrite(reset_pin, 0); // Reset
}

/* Transaction start */
void EPD_565c::start(void)
{
    sendCommand(0x61); //Set Resolution setting
    sendData(0x02);
    sendData(0x58);
    sendData(0x01);
    sendData(0xC0);
    sendCommand(0x10);
}

/* Transaction stop */
void EPD_565c::stop(void)
{
    sendCommand(0x04); //0x04
    busyHigh();
    sendCommand(0x12); //0x12
    busyHigh();
    sendCommand(0x02); //0x02
    busyLow();
    delay(200);
}

/* END OF FILE */