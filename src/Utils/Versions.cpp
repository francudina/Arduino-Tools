#ifdef USE_PREFERENCE

#include "Versions.h"

String DeviceSoftware::getVersion() {
    Preferences pref;
    pref.begin(prefMetadata, true);
    String version = pref.getString(prefMetadata_softwareVersion, "v1.0.0");
    pref.end();
    return version;
}

String DeviceSoftware::getVersion(Preferences &pref) {
    return pref.getString(prefMetadata_softwareVersion, "v1.0.0");
}

String DeviceSoftware::setVersion() {
    Preferences pref;
    pref.begin(prefMetadata, false);

    String current = getVersion(pref);
    String next = versionFormat(device_swVersion_major, device_swVersion_minor, device_swVersion_patch);
    if (current != next) {
        pref.putString(prefMetadata_softwareVersion, next);
    }
    pref.end();
    Serial.printf("Versions: Device Software Version (previous: %s, current: %s)\n", current.c_str(), next.c_str());
    return next;
}

uint32_t DeviceSoftware::getBootNumber() {
    Preferences metadataPreference;
    metadataPreference.begin(prefMetadata, false);
    // booting info
    // number of bootups (can be used as count for deepsleeps)
    uint32_t bootCount = metadataPreference.getULong(prefMetadata_bootCount, 0);
    metadataPreference.end();
    return bootCount;
}

uint32_t DeviceSoftware::setBootNumber() {
    Preferences metadataPreference;
    metadataPreference.begin(prefMetadata, false);
    // booting info
    // number of bootups (can be used as count for deepsleeps)
    uint32_t bootCount = metadataPreference.getULong(prefMetadata_bootCount, 0);
    metadataPreference.putULong(prefMetadata_bootCount, ++bootCount);
    metadataPreference.end();
    return bootCount;
}

String DeviceSoftware::versionFormat(const char *major, const char *minor, const char *patch) {
    return "v" + String(major) + "." + String(minor) + "." + String(patch);
}

#endif
