# ePaper-Frame
ePaper Picture Frame firmware

This firmware runs on an Adafruit 32u4 Adalogger, although other boards are
implemented for testing, such as ESP32.

The firwmare pulls image files from an attached SD card, to write the images to
the attached ePaper display.

The image is updated randomly a few times a day without user intervention,
although there is a bypass button on the frame to allow the user to change the
button at their will.

## Image Format
Images must be placed onto an internal SD card. The images are a proprietary
format, **BUT GOOD NEWS** there is an included converter to convert just about
any format into the proprietary format.

### Image Size
The ePaper panel used for this frame has a resolution of 600x448 pixels. All
images placed onto the panel must be exactly these dimensions. If you'd like
the frame to display portrait photos instead, just create 448x600 portrait
photos and rotate them 90 degrees to 600x448.

### Color Palette
Since ePaper screens only have a limited number of colors, in this case 8, there
needs to be a color palette used to generate Floyd Steinburg dithering for the
images. The color palette used is shown below:

| Color     | Hex Value | Index |
|-----------|-----------|-------|
| Black     | 0x060606  |   0   |
| White     | 0xF0F0F0  |   1   |
| Green     | 0x4C6E55  |   2   |
| Blue      | 0x39476C  |   3   |
| Red       | 0xA6534F  |   4   |
| Yellow    | 0xDDCD5E  |   5   |
| Orange    | 0xC16653  |   6   |
| Tan/Clean | 0xD2A691  |   7   |

This color palette should be imported into photo editor like GIMP, and used to
convert a standard image into an indexed image with the colors above. This could
be done with Floyd-Steinburg dithering, positional dithering, or just closest
color rounding. The method used is purely personal preference as long as the
size of the image is *exactly* what's listed above and the only colors in the
image are those in the table above (with the exact color codes).

### Converting Images
To convert an image, first follow the previous two sections to create an image
with the proper size and colors palette. Next, the included conversion program
can be used to create the `.raw` image file that is understood by the frame.

The conversion program is within `scripts/converter/`. It can be built with the
included `Makefile`, and all libraries required are included. The conversion
program can be executed like so:

```
./converter input.png output.raw
```

with `input.png` being any input image and `output.raw` being any output image.

## Image Storage
Images are stored on a standard micro SD card inside the frame. The SD card must
be formatted using the correct FAT formatting method. The formatting method can
be seen below:

| SD Card Size | Fat Format |
|--------------|------------|
|     < 2GB    |   FAT16    |
|     < 4GB    |   FAT32    |
|     < 2TB    |   ExFAT    |

### Formatting
For best compatability and speed with Arduino-like boards, the official SD
standard format should be used to format the cards. This can be done directly
on the Arduino using [this example code](https://github.com/greiman/SdFat/blob/master/examples/SdFormatter/SdFormatter.ino)
from the SdFat repository.

## Building & Running
This firmware uses PlatformIO for programming and firmware management

## TODO 
* Create error text/icons for screen
  + These can possibly be stored in ROM for an ESP32 based system
* Custom PCB
* Better RNG
* Clean up code
  + Everything is in power code
  + Better multi-board configuration system
* Gyro to determine orientation for loading images
* Config file on SD card
  + Could be cached in flash for ESP32
