#ifndef BLE_SNIFFER_H
#define BLE_SNIFFER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>

enum TrackerType {
    TRACKER_NONE = 0,
    TRACKER_APPLE_FINDMY,
    TRACKER_GOOGLE_FMDN,
    TRACKER_SAMSUNG_SMARTTAG,
    TRACKER_TILE,
    TRACKER_UNKNOWN_BEACON
};

struct BeaconDetection {
    TrackerType type;
    String typeName;
    String macAddress;
    int rssi;
    uint32_t firstSeenMs;
    uint32_t lastSeenMs;
    uint16_t hits;
    String payloadHash;
    bool isPersistent;
};

class BleSniffer {
public:
    BleSniffer();
    bool init();
    std::vector<BeaconDetection> scan(uint32_t durationSeconds);
    static TrackerType identifyPayload(const uint8_t* payload, size_t length, String& outName);
    
private:
    BLEScan* m_pBLEScan;
};

#endif // BLE_SNIFFER_H
