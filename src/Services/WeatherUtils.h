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
#define weatherResource   "/v1/forecast"
#define weatherResource_param_temperature_2m "temperature_2m"
#endif

class WeatherUtils {
public:
    static float fetchCurrentTemperature(const char *param, float latitude, float longitude, Client& client);
};

#endif
