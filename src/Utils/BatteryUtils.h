#pragma once

#if defined(USE_BATTERY)

#include <Arduino.h>
#include <esp_adc_cal.h>

#ifndef BOARD_BAT_ADC_PIN
// Battery & Solar
#define BOARD_BAT_ADC_PIN 35
#endif

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

#endif
