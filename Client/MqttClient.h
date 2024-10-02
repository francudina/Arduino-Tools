#pragma once

#ifdef USE_MQTT_CLIENT

#include <set>
#include <PubSubClient.h>

#include "../../src/config.h"

class MqttClient {
public:
    MqttClient(Client& client, const char* broker, int port, const char* clientId, const char* username, const char* password);
    bool waitForConnection(bool debug = true, const int retry = 10);
    void disconnect();
    bool isConnected();

    bool subscribe(const char* topic, const uint8_t qos = 0);
    bool unsubscribe(const char* topic);
    void subscribeAll(const uint8_t qos = 0);

    void setCallback(MQTT_CALLBACK_SIGNATURE);
    bool publish(const char* topic, const String payload);
    
    void loop();
    void waitForMessages(const int waitMillis = 1000);

private:
    PubSubClient mqttClient;
    const char* broker;
    int port;
    const char* clientId;
    const char* username;
    const char* password;
    std::set<String> topicsToSubscribe;

    String connectionString();
    void addTopic(const char* topic);
    void removeTopic(const char* topic);
};

#endif
