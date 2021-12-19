#pragma once

#include <stdint.h>

#include <GxEPD2.h>

#include <SD/sdcard.hpp>

enum palette : uint32_t {
    red    = 0xA6534F,
    orange = 0xC16653,
    yellow = 0xDDCD5E,
    green  = 0x4C6E55,
    blue   = 0x39476C,
    white  = 0xE4E4E4,
    black  = 0x060606
};

struct BMP_Header
{
    uint16_t signature;
    uint32_t file_size;
    uint32_t _RESERVED;
    uint32_t data_offset;
}__attribute__((packed));

struct BMP_Information
{
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t panes; // UNSUPPORTED
    uint16_t bpp;
    uint32_t compression; // UNSUPPORTED
    uint32_t image_size; // 0 if uncompressed
    uint32_t ppm_x;
    uint32_t ppm_y;
    uint32_t colors_index; // UNSUPPORTED
    uint32_t important_color; // UNSUPPORTED
}__attribute__((packed));

class BMP
{
private:
    BMP_Header header;
    BMP_Information information;

    sdcard::File_t file;

    bool isValid = false;
    uint32_t pixelOffset;
    uint32_t xOffset;
    uint32_t yOffset;
public:
    BMP(sdcard::File_t _file): file(_file)
    {
        file.seek(0);
        file.read(&header, sizeof(header));

        if (header.signature != 0x4D42) {
            isValid = false;
            return;
        }

        file.read(&information, sizeof(information));

        file.seek(header.data_offset);

        pixelOffset = header.data_offset;
        xOffset = 0;
        yOffset = information.height - 1;

        if (information.bpp != 24) {
            isValid = false;
            return;
        }

        isValid = true;
    }

    void printInfo(void)
    {
        if (!isValid) {
            printf("BMP Image is not valid");
            return;
        }

        Serial.print("File Size: ");
        Serial.println(header.file_size);

        Serial.print("Image size: ");
        Serial.print(information.width);
        Serial.print("x");
        Serial.println(information.height);

        Serial.print("Bits-Per-Pixel: ");
        Serial.println(information.bpp);

        Serial.print("Compression: ");
        Serial.println(information.compression);

        Serial.print("Image Size: ");
        Serial.println(information.image_size);
    }

    bool readTop(void)
    {
        if (!isValid)
            return false;

        file.seek(header.data_offset);

        return true;
    }

    bool readPixel(uint16_t& color, uint32_t& x, uint32_t& y)
    {
        if (!isValid)
            return false;

        if (pixelOffset < header.data_offset)
            return false;

        if (pixelOffset >= header.file_size)
            return false;

        if (yOffset >= information.height)
            return false;

        if (information.bpp == 24) {
            uint32_t wide_pixel;
            uint8_t pixel[3];

            file.read(&pixel, 3);

            wide_pixel  = ((uint32_t)pixel[0] <<  0) & 0x000000FF;
            wide_pixel |= ((uint32_t)pixel[1] <<  8) & 0x0000FF00;
            wide_pixel |= ((uint32_t)pixel[2] << 16) & 0x00FF0000;

            x = xOffset;
            y = information.height - yOffset - 1;

            Serial.print("Color: ");
            Serial.print(wide_pixel, HEX);
            Serial.print(", "); Serial.print(x);
            Serial.print(", "); Serial.println(y);

            switch(wide_pixel) {
                case palette::red:    color = GxEPD_RED;    break;
                case palette::orange: color = GxEPD_ORANGE; break;
                case palette::yellow: color = GxEPD_YELLOW; break;
                case palette::green:  color = GxEPD_GREEN;  break;
                case palette::blue:   color = GxEPD_BLUE;   break;
                case palette::white:  color = GxEPD_WHITE;  break;
                case palette::black:  color = GxEPD_BLACK;  break;
                default:              color = GxEPD_WHITE;
            }

            pixelOffset += 3;
            xOffset += 1;
            if (xOffset >= information.width) {
                xOffset = 0;
                yOffset += 1;
            }

            return true;
        }

        return false;
    }

    bool valid(void) const
    {
        return isValid;
    }
};
