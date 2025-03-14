// #if defined(USE_WIREGUARD)
// #include "WiFiManager.h"

// #if !defined(USE_WIFI)
// #define USE_WIFI
// #endif

// dependency: "ciniml/WireGuard-ESP32": "*"

// bool VpnManager::initVPN(
//         const char* localIP, 
//         const char* privateKey, 
//         const char* remotePeerAddress, 
//         const char* remotePeerPublicKey, 
//         uint16_t remotePeerPort
// ) {
//     // check connection
//     if (!WiFiManager::isWiFiConnected()) {
//         Serial.print("VpnManager: WiFi isn't Connected");
//         return false;
//     }
//     // configure time before
//     WiFiManager::initTime();
//     // init vpn client
//     return wg.begin(
//         IpUtils::ipFromString(localIP),     // IP address of the local interface
//         privateKey,                         // Private key of the local interface
//         remotePeerAddress,                  // Address of the endpoint peer.
//         remotePeerPublicKey,                // Public key of the endpoint peer.
//         remotePeerPort);                    // Port pf the endpoint peer.
// }

// #endif
