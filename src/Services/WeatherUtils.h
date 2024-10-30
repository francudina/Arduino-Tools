#pragma once

#ifdef USE_WEATHER

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../Client/HttpRequestHandler.h"
#include "../Utils/JsonUtils.h"

#ifndef WEATHER_SERVER_CONFIG
// - Open
// - Open / Weather
#define weatherServer     "api.open-meteo.com"
#define weatherServerPort   80
#define weatherResource   "/v1/forecast?latitude52.52&longitude13.41&currenttemperature_2m"
#endif

class WeatherUtils {
public:
    static float fetchCurrentTemperature(Client& client);
};

#endif
