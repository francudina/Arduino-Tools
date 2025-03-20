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
    if (!config_checkAndCreate(ha_hubNode_temperature, pref)) config_temperatureSensor(mqttClient);

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

bool HomeAssistant::config_temperatureSensor(MqttClient &mqttClient) {
    JsonDocument config = config_sensorBasicConfig(
        ha_hubNode_temperature,
        "temperature", 
        ha_hubNode_temperature,
        "{{ value_json.current_temperature }}",
        "°C",
        ha_hubNode_temperature_state
    );
    return config_sensorRegistration(ha_hubNode_temperature_config, config, mqttClient);
}

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
bool HomeAssistant::state_batteryVoltageSensor(MqttClient &mqttClient) {
    uint16_t batteryVoltage = BatteryMonitor::readBatteryVoltage();
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

bool HomeAssistant::state_temperatureSensor(MqttClient &mqttClient, Client& client) {
    float temperature = WeatherUtils::fetchCurrentTemperature(client);
    String payload = createPayload("current_temperature", temperature);
    return mqttClient.publish(ha_hubNode_temperature_state, payload); 
}

bool HomeAssistant::config_checkAndCreate(const char *unique_id, Preferences &pref) {
    bool exists = pref.getBool(unique_id, false);
    Serial.printf("HomeAssistant: Device uid: '%s' already configured? %s\n", unique_id, exists ? "true" : "false");
    if (exists) { return true; }

    pref.putBool(unique_id, true);
    return false;
}

String HomeAssistant::createPayload(const char *key, const char *value) {
    JsonDocument payload = createJsonDocument(5);
    payload[key] = value;
    return getStringFromJson(payload);
}

String HomeAssistant::createPayload(const char *key, const float value) {
    JsonDocument payload = createJsonDocument(5);
    payload[key] = value;
    return getStringFromJson(payload);
}

String HomeAssistant::createPayload(const char *key, const int value) {
    JsonDocument payload = createJsonDocument(5);
    payload[key] = value;
    return getStringFromJson(payload);
}

String HomeAssistant::createPayload(const char *key, const uint32_t value) {
    JsonDocument payload = createJsonDocument(5);
    payload[key] = value;
    return getStringFromJson(payload);
}

JsonDocument HomeAssistant::config_sensorBasicConfig(
    const char* name, 
    const char* device_class, 
    const char* unique_id,
    const char* value_template,
    const char* unit_of_measurement,
    const char* state_topic
) {
    // create config object for sensor registration
    JsonDocument sensorDoc;
    sensorDoc["name"] = name;
    // if (strlen(device_class) != 0) sensorDoc["device_class"] = device_class;
    sensorDoc["unique_id"] = unique_id;
    sensorDoc["state_topic"] = state_topic;
    
    if (strlen(unit_of_measurement) != 0) sensorDoc["unit_of_measurement"] = unit_of_measurement;
    sensorDoc["value_template"] = value_template;

    JsonObject device = sensorDoc.createNestedObject("device");
    device["name"] = ha_hubNode;
    device["manufacturer"] = device_manufacturer;
    device["model"] = device_model;
    device["hw_version"] = device_hw_version;
    device["sw_version"] = DeviceSoftware::getVersion();

    JsonArray identifiers = device.createNestedArray("identifiers");
    identifiers.add(ha_hubNode);

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
