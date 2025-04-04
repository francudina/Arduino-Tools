#ifdef USE_HOMEASSISTANT

#include "HomeAssistantUtils.h"

void HomeAssistant::config_check(MqttClient &mqttClient) {
    Preferences pref;
    pref.begin(prefHomeAssistant, false);
#ifdef HOMEASSISTANT_REGISTER_SENSORS
    pref.clear();
#endif

    // check if sensor needs registration/config
    if (!config_checkAndCreate(ha_hubNode_status, pref)) config_statusSensor(mqttClient);
    if (!config_checkAndCreate(ha_hubNode_swVersion, pref)) config_swVersionSensor(mqttClient);
    if (!config_checkAndCreate(ha_hubNode_mac, pref)) config_macSensor(mqttClient);
#ifdef USE_BATTERY
    if (!config_checkAndCreate(ha_hubNode_batteryVoltage, pref)) config_batteryVoltageSensor(mqttClient);
#endif
    if (!config_checkAndCreate(ha_hubNode_freeFlash, pref)) config_freeFlashSensor(mqttClient);
    if (!config_checkAndCreate(ha_hubNode_freeRAM, pref)) config_freeRAMSensor(mqttClient);
    if (!config_checkAndCreate(ha_hubNode_signalQuality, pref)) config_signalQualitySensor(mqttClient);
#ifdef HAS_BUILTIN_TEMP_SENSOR
    if (!config_checkAndCreate(ha_hubNode_cpuTemperature, pref)) config_cpuTemperatureSensor(mqttClient);
#endif
#ifdef USE_WEATHER
    if (!config_checkAndCreate(ha_hubNode_location_temperature, pref)) config_temperatureSensor(mqttClient);
#endif
    if (!config_checkAndCreate(ha_hubNode_bootCount, pref)) config_bootCountSensor(mqttClient);
#ifdef USE_ZIGBEE_HUB
    if (!config_checkAndCreate(ha_hubNode_zigbeeHub_bootCount, pref)) config_zigbeeHubBootCountSensor(mqttClient);
#endif

    pref.end();
}

bool HomeAssistant::config_statusSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_status,
        "", 
        ha_hubNode_status,
        "{{ value_json.status }}",
        "",
        ha_hubNode_status_state
    );
    return config_sensorRegistration(ha_hubNode_status_config, config, mqttClient);
}

bool HomeAssistant::config_swVersionSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_swVersion,
        "", 
        ha_hubNode_swVersion,
        "{{ value_json.sw_version }}",
        "",
        ha_hubNode_swVersion_state
    );
    return config_sensorRegistration(ha_hubNode_swVersion_config, config, mqttClient);
}

bool HomeAssistant::config_macSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_mac,
        "", 
        ha_hubNode_mac,
        "{{ value_json.mac_address }}",
        "",
        ha_hubNode_mac_state
    );
    return config_sensorRegistration(ha_hubNode_mac_config, config, mqttClient);
}

#ifdef USE_BATTERY
bool HomeAssistant::config_batteryVoltageSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_batteryVoltage,
        "", 
        ha_hubNode_batteryVoltage,
        "{{ value_json.battery_voltage }}",
        "mV",
        ha_hubNode_batteryVoltage_state
    );
    return config_sensorRegistration(ha_hubNode_batteryVoltage_config, config, mqttClient);
}
#endif

bool HomeAssistant::config_freeFlashSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_freeFlash,
        "", 
        ha_hubNode_freeFlash,
        "{{ value_json.free_flash }}",
        "KB",
        ha_hubNode_freeFlash_state
    );
    return config_sensorRegistration(ha_hubNode_freeFlash_config, config, mqttClient);
}

bool HomeAssistant::config_freeRAMSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_freeRAM,
        "", 
        ha_hubNode_freeRAM,
        "{{ value_json.free_ram }}",
        "KB",
        ha_hubNode_freeRam_state
    );
    return config_sensorRegistration(ha_hubNode_freeRam_config, config, mqttClient);
}

bool HomeAssistant::config_signalQualitySensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_signalQuality,
        "", 
        ha_hubNode_signalQuality,
        "{{ value_json.signal_quality }}",
        "dBm",
        ha_hubNode_signalQuality_state
    );
    return config_sensorRegistration(ha_hubNode_signalQuality_config, config, mqttClient);
}

#ifdef HAS_BUILTIN_TEMP_SENSOR
bool HomeAssistant::config_cpuTemperatureSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_cpuTemperature,
        "temperature", 
        ha_hubNode_cpuTemperature,
        "{{ value_json.cpu_temperature }}",
        "°C",
        ha_hubNode_cpuTemperature_state
    );
    return config_sensorRegistration(ha_hubNode_cpuTemperature_config, config, mqttClient);
}
#endif

#ifdef USE_WEATHER
bool HomeAssistant::config_temperatureSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_location_temperature,
        "temperature", 
        ha_hubNode_location_temperature,
        "{{ value_json.current_temperature }}",
        "°C",
        ha_hubNode_location_temperature_state
    );
    return config_sensorRegistration(ha_hubNode_location_temperature_config, config, mqttClient);
}
#endif

bool HomeAssistant::config_bootCountSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_bootCount,
        "", 
        ha_hubNode_bootCount,
        "{{ value_json.boot_count }}",
        "No.",
        ha_hubNode_bootCount_state
    );
    return config_sensorRegistration(ha_hubNode_bootCount_config, config, mqttClient);
}

#ifdef USE_ZIGBEE_HUB
bool HomeAssistant::config_zigbeeHubBootCountSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_zigbeeHub_bootCount,
        "", 
        ha_hubNode_zigbeeHub_bootCount,
        "{{ value_json.zigbee_hub_boot_count }}",
        "No.",
        ha_hubNode_zigbeeHub_bootCount_state
    );
    return config_sensorRegistration(ha_hubNode_zigbeeHub_bootCount_config, config, mqttClient);
}
#endif

bool HomeAssistant::state_statusSensor(const char *status, MqttClient &mqttClient) {
    String payload = createPayload("status", status);
    return mqttClient.publish(ha_hubNode_status_state, payload); 
}

bool HomeAssistant::state_swVersionSensor(MqttClient &mqttClient) {
    String payload = createPayload("sw_version", DeviceSoftware::getVersion().c_str());
    return mqttClient.publish(ha_hubNode_swVersion_state, payload); 
}

bool HomeAssistant::state_macSensor(MqttClient &mqttClient) {
    uint8_t* macAddress = MacUtils::getDeviceMacAddress();
    String payload = createPayload("mac_address", MacUtils::getMacAddressString(macAddress).c_str());
    return mqttClient.publish(ha_hubNode_mac_state, payload); 
}

#ifdef USE_BATTERY
bool HomeAssistant::state_batteryVoltageSensor(uint8_t pin, MqttClient &mqttClient) {
    uint16_t batteryVoltage = BatteryMonitor::readVoltage(pin);
    String payload = createPayload("battery_voltage", batteryVoltage);
    return mqttClient.publish(ha_hubNode_batteryVoltage_state, payload); 
}
#endif

bool HomeAssistant::state_freeFlashSensor(MqttClient &mqttClient) {
    uint32_t freeFlash = Storage::getFreeSketchSpace() / 1024;
    String payload = createPayload("free_flash", freeFlash);
    return mqttClient.publish(ha_hubNode_freeFlash_state, payload); 
}

bool HomeAssistant::state_freeRAMSensor(MqttClient &mqttClient) {
    uint32_t freeRAM = Storage::getFreeHeap() / 1024;
    String payload = createPayload("free_ram", freeRAM);
    return mqttClient.publish(ha_hubNode_freeRam_state, payload); 
}

bool HomeAssistant::state_signalQualitySensor(const int sigQuality, MqttClient &mqttClient) {
    String payload = createPayload("signal_quality", sigQuality);
    return mqttClient.publish(ha_hubNode_signalQuality_state, payload); 
}

#ifdef HAS_BUILTIN_TEMP_SENSOR
bool HomeAssistant::state_cpuTemperatureSensor(MqttClient &mqttClient) {
    float temp_celsius = temperatureRead();
    Serial.print("Temp onBoard ");
    Serial.print(temp_celsius);
    Serial.println("°C");
    String payload = createPayload("cpu_temperature", temp_celsius);
    return mqttClient.publish(ha_hubNode_cpuTemperature_state, payload); 
}
#endif

#ifdef USE_WEATHER
bool HomeAssistant::state_temperatureSensor(float latitude, float longitude, MqttClient &mqttClient, Client& client) {
    float temperature = WeatherUtils::fetchCurrentTemperature(weatherResource_param_temperature_2m, latitude, longitude, client);
    String payload = createPayload("current_temperature", temperature);
    return mqttClient.publish(ha_hubNode_location_temperature_state, payload); 
}
#endif

bool HomeAssistant::state_bootCountSensor(uint32_t bootCount, MqttClient &mqttClient) {
    String payload = createPayload("boot_count", bootCount);
    return mqttClient.publish(ha_hubNode_bootCount_state, payload); 
}

#ifdef USE_ZIGBEE_HUB
bool HomeAssistant::state_zigbeeHubBootCountSensor(uint32_t bootCount, MqttClient &mqttClient) {
    String payload = createPayload("zigbee_hub_boot_count", bootCount);
    return mqttClient.publish(ha_hubNode_zigbeeHub_bootCount_state, payload); 
}
#endif

bool HomeAssistant::config_checkAndCreate(const char *unique_id, Preferences &pref) {
    bool exists = pref.getBool(unique_id, false);
    Serial.printf("HomeAssistant: Device uid: '%s' already configured? %s\n", unique_id, exists ? "true" : "false");
    if (exists) { return true; }

    pref.putBool(unique_id, true);
    return false;
}

String HomeAssistant::createPayload(const char *key, const char *value) {
    JsonDocument payload = createJsonDocument();
    payload[key] = value;
    return getStringFromJson(payload);
}

String HomeAssistant::createPayload(const char *key, const float value) {
    JsonDocument payload = createJsonDocument();
    payload[key] = value;
    return getStringFromJson(payload);
}

String HomeAssistant::createPayload(const char *key, const int value) {
    JsonDocument payload = createJsonDocument();
    payload[key] = value;
    return getStringFromJson(payload);
}

String HomeAssistant::createPayload(const char *key, const uint32_t value) {
    JsonDocument payload = createJsonDocument();
    payload[key] = value;
    return getStringFromJson(payload);
}

JsonDocument HomeAssistant::config_sensorBasicConfig(
    const char* name, 
    const char* device_class, 
    const char* unique_id,
    const char* value_template,
    const char* unit_of_measurement,
    const char* state_topic,
    const char* dev_name,
    const char* dev_manufacturer,
    const char* dev_model,
    const char* dev_hw_version,
    const char* dev_sw_version
) {
    // create config object for sensor registration
    JsonDocument sensorDoc;
    sensorDoc["name"] = name;
    // if (strlen(device_class) != 0) sensorDoc["device_class"] = device_class;
    sensorDoc["unique_id"] = unique_id;
    sensorDoc["state_topic"] = state_topic;
    
    if (strlen(unit_of_measurement) != 0) sensorDoc["unit_of_measurement"] = unit_of_measurement;
    sensorDoc["value_template"] = value_template;

    JsonObject device = sensorDoc["device"].to<JsonObject>();
    device["name"] = dev_name;
    device["manufacturer"] = dev_manufacturer;
    device["model"] = dev_model;
    device["hw_version"] = dev_hw_version;
    device["sw_version"] = dev_sw_version;

    JsonArray identifiers = device["identifiers"].to<JsonArray>();
    identifiers.add(dev_name);

    return sensorDoc;
}

bool HomeAssistant::config_sensorRegistration(
    const char* configTopic, 
    const JsonDocument &payload, 
    MqttClient &mqttClient
) {
    String config = getStringFromJson(payload);
    return mqttClient.publish(configTopic, config);
}

#endif
