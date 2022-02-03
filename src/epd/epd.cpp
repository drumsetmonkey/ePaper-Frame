#include "epd.hpp"
#include <SPI.h>

EPD_Base::EPD_Base(int RST, int DC, int CS, int BUSY)
    :reset_pin(RST), dc_pin(DC), cs_pin(CS), busy_pin(BUSY)
{};

EPD_Base::~EPD_Base() 
{};

void EPD_Base::digitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}

int EPD_Base::digitalRead(int pin) {
    return digitalRead(pin);
}

void EPD_Base::delayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EPD_Base::sendCommand(unsigned char data) {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(data);
    digitalWrite(cs_pin, HIGH);
}

EPD_Base::error EPD_Base::setup(void) {
    pinMode(cs_pin, OUTPUT);
    pinMode(reset_pin, OUTPUT);
    pinMode(dc_pin, OUTPUT);
    pinMode(busy_pin, INPUT); 
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));    

    return error::SUCCESS;
}

