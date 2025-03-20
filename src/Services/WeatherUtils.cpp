#ifdef USE_WEATHER

#include "WeatherUtils.h"

float WeatherUtils::fetchCurrentTemperature(const char *param, float latitude, float longitude, Client& client) {
#ifdef DEVICE_LOGGING
    Serial.println("Weather: Fetching current temperature...");
#endif
    String url = weatherResource 
        + "?latitude=" + String(latitude, 4) 
        + "&longitude=" + String(longitude, 4) 
        + "&current=" + String(param);

    HttpRequestHandler weatherClient(client, weatherServer, weatherServerPort);
    const JsonDocument doc = weatherClient.performGetRequestAndGetData(url.c_str());
    
    // Extract the current temperature
    float temperature = doc["current"][param];
    // Serial.printf("Weather: Current temperature: %.2f\n", temperature);
    weatherClient.stop();
    
    return temperature;
}

#endif
