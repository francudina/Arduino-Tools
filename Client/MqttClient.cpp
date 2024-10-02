#include "MqttClient.h"

MqttClient::MqttClient(Client& client, const char* broker, int port, const char* clientId, const char* username, const char* password)
    : mqttClient(client), broker(broker), port(port), clientId(clientId), username(username), password(password) {
        mqttClient.setServer(broker, port);
        mqttClient.setBufferSize(MQTT_MAX_BUFFER_SIZE);
    }

bool MqttClient::waitForConnection(bool debug, const int retry) {
    if (isConnected()) {
#ifdef DEVICE_LOGGING
        Serial.printf("MqttClient: Already Connected to MQTT broker: %s\n", connectionString().c_str());
#endif
        return true;
    }

    if (debug) Serial.printf("MqttClient: Connecting to MQTT broker: %s...\n", connectionString().c_str());
    int i = 0;
    while (!mqttClient.connect(clientId, username, password) && ++i <= retry) {
        if (debug) Serial.printf("MqttClient: Failed Connecting to MQTT broker: %s, retry: %d\n", connectionString().c_str(), i);
        delay(2000);
    }
    bool connected = isConnected();
    if (debug) 
        Serial.printf("MqttClient: Connecting to MQTT broker: %s? %s\n", 
            connectionString().c_str(), 
            connected ? "SUCCESS" : "FAILURE");
    
    return connected;
}

void MqttClient::disconnect() {
    mqttClient.disconnect();
}

bool MqttClient::isConnected() {
    return mqttClient.connected();
}

void MqttClient::addTopic(const char* topic) {
    topicsToSubscribe.insert(String(topic));
}

void MqttClient::removeTopic(const char* topic) {
    topicsToSubscribe.erase(topic);
}

bool MqttClient::subscribe(const char* topic, const uint8_t qos) {
    bool added = mqttClient.subscribe(topic, qos);
#ifdef DEVICE_LOGGING
    Serial.printf("MqttClient: Subscribed to topic: '%s' (QoS: %d)? %s\n", topic, qos, added ? "true" : "false");
#endif 
    if (!added) { return false; }
    addTopic(topic);
    return true;
}

bool MqttClient::unsubscribe(const char* topic) {
    bool removed = mqttClient.unsubscribe(topic);
#ifdef DEVICE_LOGGING
    Serial.printf("MqttClient: Unsubscribed from topic: '%s'? %s\n", topic, removed ? "true" : "false");
#endif 
    if (!removed) { return false; }
    removeTopic(topic);
    return true;
}

void MqttClient::subscribeAll(const uint8_t qos) {
    for (const auto& topic : topicsToSubscribe) {
        bool added = mqttClient.subscribe(topic.c_str(), qos);
#ifdef DEVICE_LOGGING
        Serial.printf("MqttClient: Subscribed to topic: '%s' (QoS: %d)? %s\n", topic, qos, added ? "true" : "false");
#endif  
    }
}

void MqttClient::setCallback(MQTT_CALLBACK_SIGNATURE) {
    mqttClient.setCallback(callback);
}

bool MqttClient::publish(const char* topic, const String payload) {
    // check connection before sending
    if (!waitForConnection(false, 1)) {
        Serial.printf("MqttClient: Connecting to MQTT broker FAILED before PUBLISH: %s\n", connectionString().c_str());
        return false;
    }
    // subscribe again to required topics, if needed! 
    // because connection termination resets connection!
    subscribeAll(mainServerMqttQoS);
    
    // const char* p = payload.c_str();
    // unsigned int length = strlen(p);
    bool published = mqttClient.publish(topic, payload.c_str());
    if (published) {
#ifdef LOGGING_MQTT
        Serial.printf("MqttClient: Topic published: %s\n", topic);
#endif
    } else {
        Serial.printf("MqttClient: Topic publish FAILED: %s\n", topic);
        Serial.printf("MqttClient: Buffer Size: %d, payload size: %d\n", mqttClient.getBufferSize(), payload.length());
    }

    // trigger msg publish
    mqttClient.loop();
    delay(50);
    return published;
}

void MqttClient::loop() {
    mqttClient.loop();
}

void MqttClient::waitForMessages(const int waitMillis) {
    unsigned long start = millis();
    while (millis() - start < waitMillis) {
        mqttClient.loop();
        delay(50);
    }
}

String MqttClient::connectionString() {
    return "Host: " + String(broker) + ":" + String(port) + ", Client ID: " + String(clientId);
}
