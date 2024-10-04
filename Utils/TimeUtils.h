#pragma once

#include <Arduino.h>
#include <TimeLib.h>
#include <ArduinoHttpClient.h>

#include "HttpRequestHandler.h"
#include "../../src/config.h"

// Function to get current system time
time_t getCurrentTime();

// Function to convert time_t to string format
String timeToString(time_t t);

// Function to calculate duration between two time points
unsigned long calculateDuration(time_t start, time_t end);

// Sync time
void deviceTimeSync(Client& client);
