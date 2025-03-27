#pragma once

#ifdef USE_HOMEASSISTANT

#include <ArduinoJson.h>
#include "Preferences.h"

#include "../Utils/JsonUtils.h"
#include "../Client/MqttClient.h"
#include "../Utils/Variables.h"
#include "../Utils/NetworkUtils.h"
#include "../Utils/StorageUtils.h"
#ifdef USE_BATTERY
#include "../Utils/BatteryUtils.h"
#endif
#ifdef USE_WEATHER
#include "WeatherUtils.h"
#endif

// Device details
// - chip
#ifndef DEVICE_DETAILS
#define device_manufacturer     "LilyGo"
#define device_model            "TTGO T-A7670G R2"
#define device_hw_version       "v1.3"
#endif

#ifndef HOME_ASSISTANT_CONFIG
// - Homeassistant Config
// - Homeassistant Config / Groups
#define ha_areaGroup                    "PakostaneSensors"
#define ha_hubNode                      "Pakostane_HubNode"
// - Homeassistant Config / Sensors (object ids / unique ids)
#define ha_hubNode_status                   "HN_Status"
#define ha_hubNode_swVersion                "HN_SwVersion"
#define ha_hubNode_mac                      "HN_Mac"
#define ha_hubNode_batteryVoltage           "HN_BatteryV"
#define ha_hubNode_freeFlash                "HN_FreeFlash"
#define ha_hubNode_freeRAM                  "HN_FreeRAM"
#define ha_hubNode_signalQuality            "HN_SigQuality"
#define ha_hubNode_cpuTemperature           "HN_CPUTemp"
#define ha_hubNode_location_temperature     "HN_LocTemp"
#define ha_hubNode_bootCount                "HN_BootCount"
#define ha_hubNode_zigbeeHub_bootCount      "HN_ZH_BootCount"
// - Homeassistant Config / Sensors (object ids) / Config Topics
#define ha_hubNode_status_config                "homeassistant/sensor/Pakostane_HubNode_Status/config"
#define ha_hubNode_swVersion_config             "homeassistant/sensor/Pakostane_HubNode_SwVersion/config"
#define ha_hubNode_mac_config                   "homeassistant/sensor/Pakostane_HubNode_Mac/config"
#define ha_hubNode_batteryVoltage_config        "homeassistant/sensor/Pakostane_HubNode_BatteryVoltage/config"
#define ha_hubNode_freeFlash_config             "homeassistant/sensor/Pakostane_HubNode_FreeFlash/config"
#define ha_hubNode_freeRam_config               "homeassistant/sensor/Pakostane_HubNode_FreeRAM/config"
#define ha_hubNode_signalQuality_config         "homeassistant/sensor/Pakostane_HubNode_SignalQuality/config"
#define ha_hubNode_cpuTemperature_config        "homeassistant/sensor/Pakostane_HubNode_CPUTemperature/config"
#define ha_hubNode_location_temperature_config  "homeassistant/sensor/Pakostane_HubNode_LocationTemp/config"
#define ha_hubNode_bootCount_config             "homeassistant/sensor/Pakostane_HubNode_BootCount/config"
#define ha_hubNode_zigbeeHub_bootCount_config   "homeassistant/sensor/Pakostane_ZigbeeHub_BootCount/config"
// - Homeassistant Config / Sensors (object ids) / State Topics
#define ha_hubNode_status_state                 "homeassistant/sensor/Pakostane_HubNode_Status/state"
#define ha_hubNode_swVersion_state              "homeassistant/sensor/Pakostane_HubNode_SwVersion/state"
#define ha_hubNode_mac_state                    "homeassistant/sensor/Pakostane_HubNode_Mac/state"
#define ha_hubNode_batteryVoltage_state         "homeassistant/sensor/Pakostane_HubNode_BatteryVoltage/state"
#define ha_hubNode_freeFlash_state              "homeassistant/sensor/Pakostane_HubNode_FreeFlash/state"
#define ha_hubNode_freeRam_state                "homeassistant/sensor/Pakostane_HubNode_FreeRAM/state"
#define ha_hubNode_signalQuality_state          "homeassistant/sensor/Pakostane_HubNode_SignalQuality/state"
#define ha_hubNode_cpuTemperature_state         "homeassistant/sensor/Pakostane_HubNode_CPUTemperature/state"
#define ha_hubNode_location_temperature_state   "homeassistant/sensor/Pakostane_HubNode_LocationTemp/state"
#define ha_hubNode_bootCount_state              "homeassistant/sensor/Pakostane_HubNode_BootCount/state"
#define ha_hubNode_zigbeeHub_bootCount_state    "homeassistant/sensor/Pakostane_ZigbeeHub_BootCount/state"
// - Homeassistant / Subscriptions
#define ha_hubNode_data                     "homeassistant/Pakostane_HubNode/data"
// - Homeassistant / Preferences config
#define prefHomeAssistant                   "HomeAssistant"
#endif

class HomeAssistant {
public:
    static void config_check(MqttClient &mqttClient);

    // configs
    static bool config_statusSensor(MqttClient &mqttClient);
    static bool config_swVersionSensor(MqttClient &mqttClient);
    static bool config_macSensor(MqttClient &mqttClient);
#ifdef USE_BATTERY
    static bool config_batteryVoltageSensor(MqttClient &mqttClient);
#endif
    static bool config_freeFlashSensor(MqttClient &mqttClient);
    static bool config_freeRAMSensor(MqttClient &mqttClient);
    static bool config_signalQualitySensor(MqttClient &mqttClient);
#ifdef HAS_BUILTIN_TEMP_SENSOR
    static bool config_cpuTemperatureSensor(MqttClient &mqttClient);
#endif
#ifdef USE_WEATHER
    static bool config_temperatureSensor(MqttClient &mqttClient);
#endif
    static bool config_bootCountSensor(MqttClient &mqttClient);
#ifdef USE_ZIGBEE_HUB
    static bool config_zigbeeHubBootCountSensor(MqttClient &mqttClient);
#endif

    // states
    static bool state_statusSensor(const char *status, MqttClient &mqttClient);
    static bool state_swVersionSensor(MqttClient &mqttClient);
    static bool state_macSensor(MqttClient &mqttClient);
#ifdef USE_BATTERY
    static bool state_batteryVoltageSensor(uint8_t pin, MqttClient &mqttClient);
#endif
    static bool state_freeFlashSensor(MqttClient &mqttClient);
    static bool state_freeRAMSensor(MqttClient &mqttClient);
    static bool state_signalQualitySensor(const int sigQuality, MqttClient &mqttClient);
#ifdef HAS_BUILTIN_TEMP_SENSOR
    static bool state_cpuTemperatureSensor(MqttClient &mqttClient);
#endif
#ifdef USE_WEATHER
    static bool state_temperatureSensor(float latitude, float longitude, MqttClient &mqttClient, Client& client);
#endif
    static bool state_bootCountSensor(uint32_t bootCount, MqttClient &mqttClient);
#ifdef USE_ZIGBEE_HUB
    static bool state_zigbeeHubBootCountSensor(uint32_t bootCount, MqttClient &mqttClient);
#endif

    // config / generic 
    static JsonDocument config_sensorBasicConfig(
        const char* name, 
        const char* device_class, 
        const char* unique_id,
        const char* value_template,
        const char* unit_of_measurement,
        const char* state_topic,
        const char* dev_name = ha_hubNode,
        const char* dev_manufacturer = device_manufacturer,
        const char* dev_model = device_model,
        const char* dev_hw_version = device_hw_version,
        const char* dev_sw_version = DeviceSoftware::getVersion().c_str()
    );

    static bool config_sensorRegistration(
        const char* configTopic, 
        const JsonDocument &payload, 
        MqttClient &mqttClient
    );

    static bool config_checkAndCreate(const char *unique_id, Preferences &pref);

    static String createPayload(const char *key, const char *value);
    static String createPayload(const char *key, const float value);
    static String createPayload(const char *key, const int value);
    static String createPayload(const char *key, const uint32_t value);
};

#endif
