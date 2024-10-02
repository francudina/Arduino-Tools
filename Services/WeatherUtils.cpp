#include "WeatherUtils.h"

float WeatherUtils::fetchCurrentTemperature(Client& client) {
#ifdef DEVICE_LOGGING
    Serial.println("Weather: Fetching current temperature...");
#endif
    HttpRequestHandler weatherClient(client, weatherServer, weatherServerPort);
    const DynamicJsonDocument doc = weatherClient.performGetRequestAndGetData(weatherResource, 200);
    
    // Extract the current temperature
    float temperature = doc["current"]["temperature_2m"];
    // Serial.printf("Weather: Current temperature: %.2f\n", temperature);
    weatherClient.stop();
    
    return temperature;
}
