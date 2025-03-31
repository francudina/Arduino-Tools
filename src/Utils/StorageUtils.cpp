#if defined(USE_SDCARD) || defined(USE_STORAGE)
#include "StorageUtils.h"
#endif

#if defined(USE_SDCARD)

// SDCard
bool SDCard::cardInit() {
    SPI.begin(SDCARD_SCK_PIN, SDCARD_MISO_PIN, SDCARD_MOSI_PIN);
    if (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("SDCard: Mount Failed");
        return false;
    }
    return isAvailable();
}

bool SDCard::isAvailable() {
    String type = cardTypeName();
    return type != "UNKNOWN" && type != "CARD_NONE";
}

uint8_t SDCard::cardType() {
    return SD.cardType();
}

String SDCard::cardTypeName() {
    uint8_t type = cardType();
    if (type == CARD_NONE) {
        return "CARD_NONE";
    } else if (type == CARD_MMC) {
        return "CARD_MMC";
    } else if (type == CARD_SD) {
        return "CARD_SD";
    } else if (type == CARD_SDHC) {
        return "CARD_SDHC";
    } else {
        return "UNKNOWN";
    }
}

uint64_t SDCard::cardSizeMB() {
    return SD.cardSize() / (1024 * 1024); // MB
}

uint64_t SDCard::cardTotalSpaceMB() {
    return SD.totalBytes() / (1024 * 1024); // MB
}

uint64_t SDCard::cardUsedSpaceMB() {
    return SD.usedBytes() / (1024 * 1024); // MB
}

void SDCard::listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    Serial.printf("SDCard: Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("SDCard: Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("SDCard: Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.path(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void SDCard::createDir(fs::FS &fs, const char *path) {
    Serial.printf("SDCard: Creating Dir: %s\n", path);
    if (fs.mkdir(path)) {
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void SDCard::removeDir(fs::FS &fs, const char *path) {
    Serial.printf("SDCard: Removing Dir: %s\n", path);
    if (fs.rmdir(path)) {
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void SDCard::readFile(fs::FS &fs, const char *path) {
#ifdef SD_CARD_DEBUG
    Serial.printf("SDCard: Reading file: %s\n", path);
#endif

    File file = fs.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }
#ifdef SD_CARD_DEBUG
    Serial.print("SDCard: Read from file: ");
#endif
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void SDCard::writeFile(fs::FS &fs, const char *path, const char *message) {
#ifdef SD_CARD_DEBUG
    Serial.printf("SDCard: Writing file: %s\n", path);
#endif

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
#ifdef SD_CARD_DEBUG
        Serial.println("File written");
#endif
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void SDCard::appendFile(fs::FS &fs, const char *path, const char *message) {
#ifdef SD_CARD_DEBUG
    Serial.printf("SDCard: Appending to file: %s\n", path);
#endif

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message)) {
#ifdef SD_CARD_DEBUG
        Serial.println("Message appended");
#endif
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void SDCard::renameFile(fs::FS &fs, const char *path1, const char *path2) {
    Serial.printf("SDCard: Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void SDCard::deleteFile(fs::FS &fs, const char *path) {
    Serial.printf("SDCard: Deleting file: %s\n", path);
    if (fs.remove(path)) {
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void SDCard::testFileIO(fs::FS &fs, const char *path) {
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if (toRead > 512) {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("SDCard: %u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("SDCard: Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("SDCard: Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++) {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("SDCard: %u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

#endif

#ifdef USE_STORAGE

// Storage
uint32_t Storage::getFlashChipSize() {
    return ESP.getFlashChipSize();
}

uint32_t Storage::getFlashChipSpeed() {
    return ESP.getFlashChipSpeed();
}

const char* Storage::getFlashChipMode() {
    return ESP.getFlashChipMode() == FM_QIO ? "QIO" : "QOUT";
}

size_t Storage::getFreeSketchSpace() {
    return ESP.getFreeSketchSpace();
}

size_t Storage::getHeapSize() {
    return ESP.getHeapSize();
}

size_t Storage::getFreeHeap() {
    return ESP.getFreeHeap();
}

size_t Storage::getMinFreeHeap() {
    return ESP.getMinFreeHeap();
}

size_t Storage::getLargestFreeBlockHeap() {
    return heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
}

bool Storage::isPsramAvailable() {
    return psramFound();
}

size_t Storage::getPsramSize() {
    return ESP.getPsramSize();
}

size_t Storage::getFreePsram() {
    return ESP.getFreePsram();
}

size_t Storage::getLargestFreeBlockPsram() {
    return heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
}

void Storage::printStorageInfo() {
    // Flash Information
    Serial.println("- - - - - - - - - - - - - - - - - - - - - - -");
    Serial.println("Flash Information:");
    Serial.printf("\tFlash chip size: %u KB\n", getFlashChipSize() / 1024);
    Serial.printf("\tFlash chip speed: %u Hz\n", getFlashChipSpeed());
    Serial.printf("\tFlash chip mode: %s\n", getFlashChipMode());
    Serial.printf("\tFree sketch space: %u KB\n", getFreeSketchSpace() / 1024);

    // RAM Information
    Serial.println("RAM Information:");
    Serial.printf("\tTotal heap size: %u KB\n", getHeapSize() / 1024);
    Serial.printf("\tFree heap size: %u KB\n", getFreeHeap() / 1024);
    Serial.printf("\tMinimum free heap size since boot: %u KB\n", getMinFreeHeap() / 1024);
    // Serial.printf("\tLargest free block in heap: %u KB\n", getLargestFreeBlockHeap() / 1024);

    // PSRAM Information (if available)
    if (isPsramAvailable()) {
        Serial.println("PSRAM Information:");
        Serial.printf("\tTotal PSRAM size: %u KB\n", getPsramSize() / 1024);
        Serial.printf("\tFree PSRAM size: %u KB\n", getFreePsram() / 1024);
        // Serial.printf("\tLargest free block in PSRAM: %u KB\n", getLargestFreeBlockPsram() / 1024);
    } else {
        Serial.println("PSRAM not available");
    }
    Serial.println("- - - - - - - - - - - - - - - - - - - - - - -");
}

#endif
