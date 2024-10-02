#pragma once

#ifdef USE_HOME_ASSISTANT

#include <ArduinoJson.h>
#include "Preferences.h"

#include "JsonUtils.h"
#include "MqttClient.h"
#include "Versions.h"
#include "NetworkUtils.h"
#include "StorageUtils.h"
#include "BatteryUtils.h"
#include "WeatherUtils.h"
#include "../../src/config.h"

class HomeAssistant {
public:
    static void config_check(MqttClient &mqttClient);

    static bool config_statusSensor(MqttClient &mqttClient);
    static bool config_swVersionSensor(MqttClient &mqttClient);
    static bool config_macSensor(MqttClient &mqttClient);
    static bool config_batteryVoltageSensor(MqttClient &mqttClient);
    static bool config_freeFlashSensor(MqttClient &mqttClient);
    static bool config_freeRAMSensor(MqttClient &mqttClient);
    static bool config_temperatureSensor(MqttClient &mqttClient);

    static bool state_statusSensor(const char *status, MqttClient &mqttClient);
    static bool state_swVersionSensor(MqttClient &mqttClient);
    static bool state_macSensor(MqttClient &mqttClient);
    static bool state_batteryVoltageSensor(MqttClient &mqttClient);
    static bool state_freeFlashSensor(MqttClient &mqttClient);
    static bool state_freeRAMSensor(MqttClient &mqttClient);
    static bool state_temperatureSensor(MqttClient &mqttClient, Client& client);

private:
    static bool config_checkAndCreate(const char *unique_id, Preferences &pref);

    static String createPayload(const char *key, const char *value);
    static String createPayload(const char *key, const float value);
    static String createPayload(const char *key, const int value);
    static String createPayload(const char *key, const uint32_t value);

    static DynamicJsonDocument config_sensorBasicConfig(
        const char* name,
        const char* device_class, 
        const char* unique_id, 
        const char* value_template,
        const char* unit_of_measurement,
        const char* state_topic
    );
    static bool config_sensorRegistration(
        const char* configTopic, 
        const DynamicJsonDocument &payload, 
        MqttClient &mqttClient
    );
};

#endif
