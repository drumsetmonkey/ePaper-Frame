#pragma once

#include <stdint.h>

#include <GxEPD2.h>

#include <SD/sdcard.hpp>

class RAW
{
private:
    sdcard::File_t &file;
public:

    enum error : int {
        VALID,
        INVALID_SIGNATURE
    };

    RAW(sdcard::File_t &file): file(file)
    {}

    error validate(uint16_t validsig)
    {
        file.rewind();

        uint16_t signature;
        ((uint8_t *)&signature)[0] = file.read(); // LSB
        ((uint8_t *)&signature)[1] = file.read(); // MSB

        if (signature != validsig) {
            return error::INVALID_SIGNATURE;
        }

        return error::VALID;
    }

    void seekData(void)
    {
        file.seek(2);
    }

    uint8_t readRaw(void)
    {
        return file.read();
    }
};
