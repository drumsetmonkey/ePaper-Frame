#pragma once

#include "epd.hpp"

class EPD_565c : EPD {
public:
    EPD_565c(int RST, int DC, int CS, int BUSY);
    ~EPD_565c();

    enum Color : uint8_t {
        Black  = 0x0,
        White  = 0x1,
        Green  = 0x2,
        Blue   = 0x3,
        Red    = 0x4,
        Yellow = 0x5,
        Orange = 0x6,
        Clean  = 0x7
    };

    const static unsigned long width = 600;
    const static unsigned long height = 448;

    EPD::Error setup(void) const;

    void sendData(uint8_t data) const;

    void sleep(void) const;
    void clear(Color color) const;
    void reset(void) const;

    void start(void) const;
    void stop(void) const;

	void demo(void) const;

private:
    void sendCommand(uint8_t command) const;
	void busyHigh(void) const;
	void busyLow(void) const;
};
