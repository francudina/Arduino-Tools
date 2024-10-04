#pragma once

#include <Arduino.h>
#include <esp_adc_cal.h>

#include "../../src/config.h"

class BatteryMonitor {
public:
    static void begin();
    static uint16_t readBatteryVoltage();
    static uint16_t readSolarVoltage();
    static void printBatteryInfo();

private:
    static esp_adc_cal_characteristics_t adc_chars;
    static uint16_t readVoltage(uint8_t channel);
};
