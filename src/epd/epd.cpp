#include "epd.hpp"
#include <SPI.h>

EPD::EPD(int RST, int DC, int CS, int BUSY)
    :reset_pin(RST), dc_pin(DC), cs_pin(CS), busy_pin(BUSY)
{};

EPD::~EPD() 
{};

/* @brief: Setup the hardware.
 * This will set pin directions for control pins and start SPI.
 *
 * @return[EPD::Error] EPD:Error::SUCCESS if successful.
 */
EPD::Error EPD::setup(void) const
{
    pinMode(cs_pin, OUTPUT);
    pinMode(reset_pin, OUTPUT);
    pinMode(dc_pin, OUTPUT);
    pinMode(busy_pin, INPUT); 

    SPI.begin(); // TODO: make this dynamic with hardware pins
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));    

    return Error::SUCCESS; // TODO: detect errors
}

/* @brief Writes to pin. Just a wrapper for future error handling
 *
 * @param[pin] Pin to update
 * @param[value] Value to write to pin
 */
void EPD::digitalWrite(int pin, int value) const
{
    digitalWrite(pin, value);
}

/* @brief Reads from pin.
 *
 * @param[pin] Pin to read
 * @return[int] Value of `pin`
 */
int EPD::digitalRead(int pin) const
{
    return digitalRead(pin);
}

/* @brief Delays for `delaytime` milliseconds
 * This is a basic wrapper, but it can be used to wrap platform sleep
 * @param[delaytime] How many milliseconds to delay
 */
void EPD::delayMs(unsigned int delaytime) const
{
    delay(delaytime);
}

/* @brief Transfers data over SPI to ePaper
 * 
 * @param[data] Data to send
 */
void EPD::transfer(unsigned char data) const
{
    digitalWrite(cs_pin, LOW);
    SPI.transfer(data);
    digitalWrite(cs_pin, HIGH);
}
