#pragma once

#if defined(USE_TIME)

#include <Arduino.h>
#include <TimeLib.h>
#include <ArduinoHttpClient.h>

#include "../Client/HttpRequestHandler.h"

#ifndef TIME_SERVER_CONFIG
#define timeServer                      "smart-home-pi.duckdns.org"
#define timeServerPort                  443
#define timeServerTimeSyncResource      "/v1/currentTime?typemillis"
#endif

// Function to get current system time
time_t getCurrentTime();

// Function to convert time_t to string format
String timeToString(time_t t);

// Function to calculate duration between two time points
unsigned long calculateDuration(time_t start, time_t end);

// Sync time
void deviceTimeSync(Client& client);

#endif
