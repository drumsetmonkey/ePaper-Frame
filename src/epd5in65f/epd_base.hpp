#pragma once

#include <Arduino.h>

class EPD_Base {
public:
    enum error : int {
        SUCCESS = 0
    };

    EPD_Base(int RST, int DC, int CS, int BUSY);
    ~EPD_Base(void);

    error BaseInit(void);
    void  DigitalWrite(int pin, int value); 
    int   DigitalRead(int pin);
    void  DelayMs(unsigned int delaytime);
    void  SpiTransfer(unsigned char data);

protected:
    const int reset_pin;
    const int dc_pin;
    const int cs_pin;
    const int busy_pin;
};
