/*
 * Print size, modify date/time, and name for all files in root.
 */
#include "SdFat.h"

// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 0

// Try max SPI clock for an SD. Reduce SPI_CLOCK if errors occur.
#define SPI_CLOCK SD_SCK_MHZ(50)

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif  ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(config::board::sdcard::CS, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(config::board::sdcard::CS, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

namespace sdcard {

#if SD_FAT_TYPE == 0
    typedef SdFat SD_t;
    typedef File File_t;
#elif SD_FAT_TYPE == 1
    typedef SdFat32 SD_2;
    typedef File32 File_t;
#elif SD_FAT_TYPE == 2
    typedef SdExFat SD_t;
    typedef ExFile File_t;
#elif SD_FAT_TYPE == 3
    typedef SdFs SD_t;
    typedef FsFile File_t;
#else  // SD_FAT_TYPE
#error invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

SD_t sd;

/** Error type class */
enum class error:int{
    SUCCESS = 0,
    NO_INITIALIZATION,
    NO_ROOT_DIRECTORY,
    NO_DIR,
    NO_FILE
};

error setup()
{
    File_t dir;

    // Initialize the SD.
    if (!sd.begin(SD_CONFIG)) {
        return error::NO_INITIALIZATION;
    }

    // Open root directory
    if (!dir.open("/")) {
        return error::NO_ROOT_DIRECTORY;
    }
    
    // Digital pin out for card detect
    // If no card, return error to reset device

    return error::SUCCESS;
}

error randomFile(File_t& file)
{
    File_t dir; // Temporary dir
    long fileCount = 0;
    long randFile = 0;

    /*****************
    *  COUNT FILES  *
    *****************/
    if (!dir.open("/")) {
        return error::NO_ROOT_DIRECTORY;
    }

    while (file.openNext(&dir, O_RDONLY)) {
        fileCount++;
        file.close();
    }

    dir.close();

    if (fileCount == 0) {
        return error::NO_FILE;
    }

    /*******************
    *  CHOOSE RANDOM  *
    *******************/
    randFile = random(0, fileCount) + 1;

    /***************
    *  OPEN FILE  *
    ***************/
    if (!dir.open("/")) {
        return error::NO_ROOT_DIRECTORY;
    }

    while (randFile-- && file.openNext(&dir, O_RDONLY)) {
        if (randFile > 0)
            file.close();
        else
            file.printName(&Serial);
    }

    /*********************
    *  GET ERROR/RETURN  *
    *********************/
    if (dir.getError()) {
        return error::NO_DIR;
    }

    return error::SUCCESS;
}

error close(File_t& file)
{
    if (file.close())
        return error::NO_FILE;
    return error::SUCCESS;
}

void loop(void)
{}

};
