#ifdef USE_BATTERY

#include "BatteryUtils.h"

uint16_t BatteryMonitor::readVoltage(uint8_t pin) {
    uint32_t battery_voltage = analogReadMilliVolts(pin);
    battery_voltage *= 2;
    return battery_voltage;
}

void BatteryMonitor::printBatteryInfo(uint8_t batteryPin, uint8_t solarPin) {
    Serial.println("- - - - - - - - - - - - - - - -");
    Serial.println("BatteryMonitor:");
    Serial.printf("\tBattery: %u mV\n", readVoltage(batteryPin));
    Serial.printf("\tSolar: %u mV\n", readVoltage(solarPin));
    Serial.println("- - - - - - - - - - - - - - - -");
}

#endif
