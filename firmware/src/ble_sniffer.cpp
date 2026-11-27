#include "ble_sniffer.h"

// Static container for callbacks
static std::vector<BeaconDetection> s_currentScanResults;

class SnifferCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        BeaconDetection detection;
        detection.type = TRACKER_NONE;
        detection.typeName = "Unknown";
        detection.macAddress = advertisedDevice.getAddress().toString().c_str();
        detection.rssi = advertisedDevice.getRSSI();
        detection.lastSeenMs = millis();
        detection.hits = 1;
        detection.isPersistent = false;

        // 1. Check Manufacturer Specific Data
        if (advertisedDevice.haveManufacturerData()) {
            std::string mfgData = advertisedDevice.getManufacturerData();
            const uint8_t* data = (const uint8_t*)mfgData.data();
            size_t len = mfgData.length();

            if (len >= 2) {
                uint16_t companyId = (data[1] << 8) | data[0];

                // Apple Inc. (0x004C)
                if (companyId == 0x004C) {
                    // Type 0x12: Offline Finding (Find My / AirTag)
                    if (len >= 4 && data[2] == 0x12) {
                        detection.type = TRACKER_APPLE_FINDMY;
                        detection.typeName = "Apple FindMy";
                    }
                }
                // Samsung Electronics (0x0075)
                else if (companyId == 0x0075) {
                    if (len >= 4 && (data[2] == 0x01 || data[2] == 0x42)) {
                        detection.type = TRACKER_SAMSUNG_SMARTTAG;
                        detection.typeName = "Samsung SmartTag";
                    }
                }
                // Google LLC (0x00E0)
                else if (companyId == 0x00E0) {
                    detection.type = TRACKER_GOOGLE_FMDN;
                    detection.typeName = "Google FMDN";
                }
                // Tile Inc. (0x00D0)
                else if (companyId == 0x00D0) {
                    detection.type = TRACKER_TILE;
                    detection.typeName = "Tile Beacon";
                }
            }
        }

        // 2. Check Service UUIDs (Google Fast Pair / FMDN & Tile)
        if (detection.type == TRACKER_NONE && advertisedDevice.haveServiceUUID()) {
            BLEUUID serviceUuid = advertisedDevice.getServiceUUID();
            // Google Fast Pair / FMDN: 0xFE2C, 0xFEED, 0xFEAA
            if (serviceUuid.equals(BLEUUID((uint16_t)0xFE2C)) || 
                serviceUuid.equals(BLEUUID((uint16_t)0xFEED))) {
                detection.type = TRACKER_GOOGLE_FMDN;
                detection.typeName = "Google FMDN";
            }
            // Tile 16-bit Service UUID: 0xFEED
            else if (serviceUuid.equals(BLEUUID((uint16_t)0xFEED))) {
                detection.type = TRACKER_TILE;
                detection.typeName = "Tile Beacon";
            }
        }

        if (detection.type != TRACKER_NONE) {
            // Generate a simple payload hash for rotating MAC correlation
            char hashBuf[16];
            snprintf(hashBuf, sizeof(hashBuf), "%04X%02X", 
                     (uint16_t)(detection.type * 31 + detection.macAddress.length()),
                     (uint8_t)(abs(detection.rssi)));
            detection.payloadHash = String(hashBuf);
            s_currentScanResults.push_back(detection);
        }
    }
};

BleSniffer::BleSniffer() : m_pBLEScan(nullptr) {}

bool BleSniffer::init() {
    BLEDevice::init("GhostTag-Scanner");
    m_pBLEScan = BLEDevice::getScan();
    if (!m_pBLEScan) return false;

    m_pBLEScan->setAdvertisedDeviceCallbacks(new SnifferCallbacks(), true);
    m_pBLEScan->setActiveScan(true);
    m_pBLEScan->setInterval(100);
    m_pBLEScan->setWindow(99);
    return true;
}

std::vector<BeaconDetection> BleSniffer::scan(uint32_t durationSeconds) {
    s_currentScanResults.clear();
    if (m_pBLEScan) {
        m_pBLEScan->start(durationSeconds, false);
        m_pBLEScan->clearResults();
    }
    return s_currentScanResults;
}
