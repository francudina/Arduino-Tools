#include "BatteryUtils.h"

esp_adc_cal_characteristics_t BatteryMonitor::adc_chars;

void BatteryMonitor::begin() {
    // Characterize ADC
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, &adc_chars);
}

uint16_t BatteryMonitor::readBatteryVoltage() {
    return readVoltage(BOARD_BAT_ADC_PIN);
}

uint16_t BatteryMonitor::readSolarVoltage() {
#ifdef BOARD_SOLAR_ADC_PIN
    return readVoltage(BOARD_SOLAR_ADC_PIN);
#else
    return 0;
#endif
}

void BatteryMonitor::printBatteryInfo() {
    Serial.println("- - - - - - - - - - - - - - - -");
    Serial.println("BatteryMonitor:");
    Serial.printf("\tBattery: %u mV\n", readBatteryVoltage());
    Serial.printf("\tSolar: %u mV\n", readSolarVoltage());
    Serial.println("- - - - - - - - - - - - - - - -");
}

uint16_t BatteryMonitor::readVoltage(uint8_t channel) {
    return esp_adc_cal_raw_to_voltage(analogRead(channel), &adc_chars) * 2;
}
