#pragma once

#ifdef USE_HTTP_CLIENT

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

class MacUtils {
public:
    static uint8_t* getDeviceMacAddress();
    static void printMacAddress(uint8_t* macAddress);
    static String getMacAddressString(const uint8_t* macAddress);
    static uint8_t* macStrToArray(const char* macAddress);
};

class IpUtils {
public:
    static IPAddress ipFromString(const char* ip);
};

#endif
