#pragma once

#include <Preferences.h>
#include <cstring>

// Preferences config
// - general info
#ifndef PREF_GENERAL_INFO
#define prefMetadata                        "metadata"
#define prefMetadata_bootCount              "boot_count"
#define prefMetadata_loopCount              "loop_count"
#define prefMetadata_macAddress             "mac_address"
#define prefMetadata_softwareVersion        "sw_version"
#endif

// - software
#ifndef DEVICE_DETAILS
const char device_swVersion_major[] = "1";
const char device_swVersion_minor[] = "0";
const char device_swVersion_patch[] = "0";
#endif

class DeviceSoftware {
public:
    static String getVersion();
    static String getVersion(Preferences &pref);

    static String setVersion();

    static uint32_t getBootNumber();
    static uint32_t setBootNumber();

private:
    static String versionFormat(const char *major, const char *minor, const char *patch);
};