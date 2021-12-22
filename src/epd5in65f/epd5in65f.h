/*****************************************************************************
* | File      	:   EPD_5in65f.h
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

#ifndef __EPD_5IN65F_H__
#define __EPD_5IN65F_H__

#include "epdif.h"

/**********************************
Color Index
**********************************/
#define EPD_5IN65F_BLACK   0x0	/// 000
#define EPD_5IN65F_WHITE   0x1	///	001
#define EPD_5IN65F_GREEN   0x2	///	010
#define EPD_5IN65F_BLUE    0x3	///	011
#define EPD_5IN65F_RED     0x4	///	100
#define EPD_5IN65F_YELLOW  0x5	///	101
#define EPD_5IN65F_ORANGE  0x6	///	110
#define EPD_5IN65F_CLEAN   0x7	///	111   unavailable  Afterimage

class EPD_565c : EPD_Base {
public:
    EPD_565c(int RST, int DC, int CS, int BUSY);
    ~EPD_565c();
    int  setup(void);
	void busyHigh(void);
	void busyLow(void);
    void reset(void);
	void demo(void);
    void sendCommand(unsigned char command);
    void sendData(unsigned char data);
    void sleep(void);
    void clear(uint8_t color);

    void start(void);
    void stop(void);

private:
    const unsigned long width = 600;
    const unsigned long height = 448;
};

#endif /* EPD5IN83B_HD_H */

/* END OF FILE */
