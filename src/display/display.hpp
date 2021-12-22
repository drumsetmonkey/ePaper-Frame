#pragma once

#include <GxEPD2.h>
#include <GxEPD2_7C.h>

//#include <demo_image.hpp>

#define GxEPD2_DISPLAY_CLASS GxEPD2_7C
#define GxEPD2_DRIVER_CLASS GxEPD2_565c

// somehow there should be an easier way to do this
#define GxEPD2_BW_IS_GxEPD2_BW false
#define GxEPD2_3C_IS_GxEPD2_3C false
#define GxEPD2_7C_IS_GxEPD2_7C true
#define GxEPD2_1248_IS_GxEPD2_1248 false

#define IS_GxEPD(c, x) (c##x)
#define IS_GxEPD2_BW(x) IS_GxEPD(GxEPD2_BW_IS_, x)
#define IS_GxEPD2_3C(x) IS_GxEPD(GxEPD2_3C_IS_, x)
#define IS_GxEPD2_7C(x) IS_GxEPD(GxEPD2_7C_IS_, x)
#define IS_GxEPD2_1248(x) IS_GxEPD(GxEPD2_1248_IS_, x)

#if defined(ESP32)
    #define MAX_DISPLAY_BUFFER_SIZE 65536ul // e.g.
#else
    #define MAX_DISPLAY_BUFFER_SIZE 448
#endif

#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
    #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS)
    #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
    #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif

namespace display
{

static GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> 
    display(GxEPD2_DRIVER_CLASS(config::board::epaper::CS,
                                config::board::epaper::DC,
                                config::board::epaper::RST,
                                config::board::epaper::BUSY));

void setup(void)
{
    display.init();
    config::board::spi::reset();

    display.setRotation(0);
    display.firstPage();
}

void drawPixel(uint16_t color, uint32_t x, uint32_t y)
{
    display.drawPixel(x, y, color);
    //display.fillRect(x, y, 1, 1, color);
    //display.writeImagePart(color, x, y, 0, 0, 1, 1, false, false, false); // No invert
    //display.writeImage(color, x, y, 1, 1, true, false, false); // Invert
}

void drawNative(uint8_t* native, uint32_t x, uint32_t y, uint32_t w)
{
    display.writeNative(native, 0, x, y, w, 1, false, false, false);
}

bool nextPage(void)
{
    return display.nextPage();
}

void clear(void)
{
    display.clearScreen();
}

void refresh(void)
{
    display.refresh(false);
}

uint32_t pageHeight(void)
{
    return display.pageHeight();
}

uint32_t pages(void)
{
    return display.pages();
}

uint32_t width(void)
{
    return display.width();
}

uint8_t color7(uint16_t color)
{
    static uint16_t _prev_color = GxEPD_BLACK;
    static uint8_t _prev_color7 = 0x00; // black
    if (color == _prev_color) return _prev_color7;
    uint8_t cv7 = 0x00;
    switch (color)
    {
    case GxEPD_BLACK: cv7 = 0x00; break;
    case GxEPD_WHITE: cv7 = 0x01; break;
    case GxEPD_GREEN: cv7 = 0x02; break;
    case GxEPD_BLUE:  cv7 = 0x03; break;
    case GxEPD_RED:   cv7 = 0x04; break;
    case GxEPD_YELLOW: cv7 = 0x05; break;
    case GxEPD_ORANGE: cv7 = 0x06; break;
    default:
        {
        uint16_t red = color & 0xF800;
        uint16_t green = (color & 0x07E0) << 5;
        uint16_t blue = (color & 0x001F) << 11;
        if ((red < 0x8000) && (green < 0x8000) && (blue < 0x8000)) cv7 = 0x00; // black
        else if ((red >= 0x8000) && (green >= 0x8000) && (blue >= 0x8000)) cv7 = 0x01; // white
        else if ((red >= 0x8000) && (blue >= 0x8000)) cv7 = red > blue ? 0x04 : 0x03; // red, blue
        else if ((green >= 0x8000) && (blue >= 0x8000)) cv7 = green > blue ? 0x02 : 0x03; // green, blue
        else if ((red >= 0x8000) && (green >= 0x8000))
        {
            static const uint16_t y2o_lim = ((GxEPD_YELLOW - GxEPD_ORANGE) / 2 + (GxEPD_ORANGE & 0x07E0)) << 5;
            cv7 = green > y2o_lim ? 0x05 : 0x06; // yellow, orange
        }
        else if (red >= 0x8000) cv7 = 0x04; // red
        else if (green >= 0x8000) cv7 = 0x02; // green
        else cv7 = 0x03; // blue
        }
    }
    _prev_color = color;
    _prev_color7 = cv7;
    return cv7;
}

void demo(void)
{
    display.setRotation(0);
    display.firstPage();
    int32_t page = 0;
    do {
        for (int y = 0; y < display.height(); y++) {
            for (int x = 0; x < display.width(); x++) {
                if ((x & 1) && (y & 1))
                    display.drawPixel(x, y, GxEPD_BLACK);
                else
                    display.drawPixel(x, y, GxEPD_WHITE);
            }
        }
    } while (display.nextPage());
}

static const uint16_t input_buffer_pixels = 200; // may affect performance

static const uint16_t max_row_width = 600; // for up to 7.5" display
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t output_row_color_buffer[max_row_width / 8]; // buffer for at least one row of color bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w

uint16_t read16(SdFile& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(SdFile& f)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void drawBitmapFromSD(const char *filename, int16_t x, int16_t y, bool with_color)
{
  SdFile file;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  //Serial.println(); Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.println(filename);
  if ((x >= int16_t(display.width())) || (y >= int16_t(display.height()))) return;
  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
#if defined(ESP32)
  file = SD.open(String("/") + filename, FILE_READ);
  if (!file)
  {
    Serial.print(F("File not found"));
    return;
  }
#else
  if (!file.open(filename, FILE_READ))
  {
    Serial.print(F("File not found"));
    return;
  }
#endif
  // Parse BMP header
  if (read16(file) == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(file);
    uint32_t creatorBytes = read32(file);
    uint32_t imageOffset = read32(file); // Start of image data
    uint32_t headerSize = read32(file);
    uint32_t width  = read32(file);
    uint32_t height = read32(file);
    uint16_t planes = read16(file);
    uint16_t depth = read16(file); // bits per pixel
    uint32_t format = read32(file);
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print(F("File size: ")); Serial.println(fileSize);
      Serial.print(F("Image Offset: ")); Serial.println(imageOffset);
      Serial.print(F("Header size: ")); Serial.println(headerSize);
      Serial.print(F("Bit Depth: ")); Serial.println(depth);
      Serial.print(F("Image size: "));
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= int16_t(display.width()))  w = int16_t(display.width())  - x;
      if ((y + h - 1) >= int16_t(display.height())) h = int16_t(display.height()) - y;
      if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish, colored;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //file.seekSet(54); //palette is always @ 54
          file.seekSet(imageOffset - (4 << depth)); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = file.read();
            green = file.read();
            red   = file.read();
            file.read();
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
          }
        }
        display.clearScreen();
        display.firstPage();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          Serial.println(row);
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint8_t out_color_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          file.seekSet(rowPosition);
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              in_bytes = file.read(input_buffer, in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain);
              in_remain -= in_bytes;
              in_idx = 0;
            }
            switch (depth)
            {
              case 24:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                break;
              case 16:
                {
                  uint8_t lsb = input_buffer[in_idx++];
                  uint8_t msb = input_buffer[in_idx++];
                  if (format == 0) // 555
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                    red   = (msb & 0x7C) << 1;
                  }
                  else // 565
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                    red   = (msb & 0xF8);
                  }
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                }
                break;
              case 1:
              case 4:
              case 8:
                {
                  if (0 == in_bits)
                  {
                    in_byte = input_buffer[in_idx++];
                    in_bits = 8;
                  }
                  uint16_t pn = (in_byte >> bitshift) & bitmask;
                  whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  in_byte <<= depth;
                  in_bits -= depth;
                }
                break;
            }
            if (whitish)
            {
              //out_byte |= 0x80 >> col % 8; // not black
              //out_color_byte |= 0x80 >> col % 8; // not colored
              // keep white
            }
            else if (colored && with_color)
            {
              //out_byte |= 0x80 >> col % 8; // not black
              out_color_byte &= ~(0x80 >> col % 8); // colored
            }
            else
            {
              //out_color_byte |= 0x80 >> col % 8; // not colored
              out_byte &= ~(0x80 >> col % 8); // black
            }
            if ((7 == col % 8) || (col == w - 1)) // write that last byte! (for w%8!=0 border)
            {
              output_row_color_buffer[out_idx] = out_color_byte;
              output_row_mono_buffer[out_idx++] = out_byte;
              out_byte = 0xFF; // white (for w%8!=0 border)
              out_color_byte = 0xFF; // white (for w%8!=0 border)
            }
          } // end pixel
          uint16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_mono_buffer, output_row_color_buffer, x, yrow, w, 1);
          display.nextPage();
        } // end line
        Serial.print(F("loaded in ")); Serial.print(millis() - startTime); Serial.println(F(" ms"));
        display.refresh();
      }
    }
  }
  //Serial.print(F("end curPosition  ")); Serial.println(file.curPosition());
  file.close();
  if (!valid)
  {
    Serial.println(F("bitmap format not handled."));
  }
}

}; // namespace display
