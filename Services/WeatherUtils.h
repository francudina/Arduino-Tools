#pragma once

#include <Arduino.h>
#include "ArduinoJson.h"
#include "HttpRequestHandler.h"

#include "HttpRequestHandler.h"
#include "JsonUtils.h"
#include "../../src/config.h"

class WeatherUtils {
public:
    static float fetchCurrentTemperature(Client& client);
};
