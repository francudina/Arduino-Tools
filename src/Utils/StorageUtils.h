#pragma once

#if defined(USE_STORAGE)
#include <esp_spi_flash.h>
#include <esp_heap_caps.h>
#include <SPI.h>
#endif

#ifdef USE_SDCARD
#include <FS.h>
#include <SD.h>

#ifndef SDCARD_CONFIG
// SD Card
#define SDCARD_CS_PIN   13
#define SDCARD_SCK_PIN  14
#define SDCARD_MOSI_PIN 15
#define SDCARD_MISO_PIN 2
#endif

class SDCard {
public:
    static bool cardInit();

    static uint8_t cardType();
    static String cardTypeName();

    static uint64_t cardSizeMB();
    static uint64_t cardTotalSpaceMB();
    static uint64_t cardUsedSpaceMB();

    static void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
    static void createDir(fs::FS &fs, const char *path);
    static void removeDir(fs::FS &fs, const char *path);
    static void readFile(fs::FS &fs, const char *path);
    static void writeFile(fs::FS &fs, const char *path, const char *message);
    static void appendFile(fs::FS &fs, const char *path, const char *message);
    static void renameFile(fs::FS &fs, const char *path1, const char *path2);
    static void deleteFile(fs::FS &fs, const char *path);
    static void testFileIO(fs::FS &fs, const char *path);
};

#endif

#ifdef USE_STORAGE

class Storage {
public:
    // Flash Information
    static uint32_t getFlashChipSize();
    static uint32_t getFlashChipSpeed();
    static const char* getFlashChipMode();
    static size_t getFreeSketchSpace();

    // RAM Information
    static size_t getHeapSize();
    static size_t getFreeHeap();
    static size_t getMinFreeHeap();
    static size_t getLargestFreeBlockHeap();

    // PSRAM Information (if available)
    static bool isPsramAvailable();
    static size_t getPsramSize();
    static size_t getFreePsram();
    static size_t getLargestFreeBlockPsram();

    // Print all storage information
    static void printStorageInfo();
};

#endif
