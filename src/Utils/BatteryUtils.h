#pragma once

#ifdef USE_BATTERY

#include <Arduino.h>
#include <esp32-hal-adc.h>

class BatteryMonitor {
public:
    static uint16_t readVoltage(uint8_t pin);
    static void printBatteryInfo(uint8_t batteryPin, uint8_t solarPin);
};

#endif
