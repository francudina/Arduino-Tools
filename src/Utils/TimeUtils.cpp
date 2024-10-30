#ifdef USE_TIME

#include "TimeUtils.h"

// Function to get current system time
time_t getCurrentTime() {
    return now();
}

// Function to convert time_t to string format
String timeToString(time_t t) {
    struct tm *timeinfo;
    char formattedTime[20];
    timeinfo = localtime(&t);
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", timeinfo);
    return String(formattedTime);
}

// Function to calculate duration between two time points
unsigned long calculateDuration(time_t start, time_t end) {
    return (unsigned long)(end - start);
}

void deviceTimeSync(Client& client) {
    // request time sync from other service
    HttpRequestHandler timeSyncClient(client, timeServer, timeServerPort);
    const DynamicJsonDocument doc = timeSyncClient.performGetRequestAndGetData(timeServerTimeSyncResource, 20);
    timeSyncClient.stop();

    if (isJsonEmpty(doc)) { return; }

    String millisValue = doc["millis"];
    time_t epochTime = millisValue.toInt();
    setTime(epochTime);
    
    Serial.println("TimeUtils: Device Time Synchronized: " + timeToString(getCurrentTime()));
}

#endif
