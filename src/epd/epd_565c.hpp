#pragma once

#include "epd.hpp"

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
