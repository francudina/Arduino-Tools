#pragma once

#include <Arduino.h>

#include "../../src/config.h"

class MqttConsumer {
public:
    static void mqttGenericCallback(char* topic, byte* payload, unsigned int length);

private:
    static void handleTopic(byte* payload, unsigned int length);
};
