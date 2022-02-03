#pragma once

#include <Arduino.h>

class EPD {
public:
    enum Error : int {
        SUCCESS = 0
    };

protected:

    EPD(int RST, int DC, int CS, int BUSY);
    ~EPD(void);

    Error setup(void) const;

    void  digitalWrite(int pin, int value) const; // TODO: inline
    int   digitalRead(int pin) const;             // TODO: inline
    void  delayMs(unsigned int delaytime) const;  // TODO: inline
    void  transfer(unsigned char data) const;

    const int reset_pin;
    const int dc_pin;
    const int cs_pin;
    const int busy_pin;
};
