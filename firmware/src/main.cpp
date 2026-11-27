/**
 * @file main.cpp
 * @brief GhostTag - Autonomous Pocket Anti-Stalking & Hidden Surveillance Detector
 * @version 1.0.0 (Initial Open Source Release)
 * @license CERN-OHL-P-2.0 / MIT
 */

#include <Arduino.h>
#include "ble_sniffer.h"
#include "heuristics.h"
#include "wifi_sniffer.h"
#include "ui_display.h"
#include "power_mgmt.h"

// Hardware Pin Definitions
#define PIN_OLED_SDA 5
#define PIN_OLED_SCL 6
#define BUTTON_DEBOUNCE_MS 250

// Core Subsystems
BleSniffer bleSniffer;
PersistenceEngine persistenceEngine(60000, 3, -78); // 1 min window, 3 hits, -78 dBm threshold
WifiCameraSniffer wifiSniffer;
UiDisplay ui(PIN_OLED_SDA, PIN_OLED_SCL);
PowerManager power;

// System State
OperatingMode currentMode = MODE_SENTINEL_BLE;
uint32_t lastModePressMs = 0;
uint8_t currentWifiChannel = 1;

void checkModeButton() {
    if (digitalRead(PIN_BUTTON_MODE) == LOW) {
        uint32_t now = millis();
        if (now - lastModePressMs > BUTTON_DEBOUNCE_MS) {
            lastModePressMs = now;
            if (currentMode == MODE_SENTINEL_BLE) {
                currentMode = MODE_ROOM_SWEEP_WIFI;
                wifiSniffer.init();
                wifiSniffer.startPromiscuous(currentWifiChannel);
            } else if (currentMode == MODE_ROOM_SWEEP_WIFI) {
                wifiSniffer.stopPromiscuous();
                currentMode = MODE_OPTICAL_SEARCH;
            } else {
                power.setOpticalLeds(false);
                currentMode = MODE_SENTINEL_BLE;
                bleSniffer.init();
            }
            power.triggerHapticAlert(50, 1); // Tactile button confirmation
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("==================================================");
    Serial.println("   👻 GHOSTTAG POCKET SENTINEL - FIRMWARE v1.0   ");
    Serial.println("   Open Source Hardware & Firmware Anti-Stalker   ");
    Serial.println("==================================================");

    power.init();
    
    if (ui.init()) {
        ui.showSplash();
    } else {
        Serial.println("[WARN] OLED display not detected on I2C bus.");
    }

    if (!bleSniffer.init()) {
        Serial.println("[ERROR] BLE Subsystem initialization failed!");
    } else {
        Serial.println("[OK] BLE Sentinel Engine active.");
    }
}

void loop() {
    checkModeButton();
    float vBat = power.readBatteryVoltage();

    switch (currentMode) {
        case MODE_SENTINEL_BLE: {
            // BLE Passive Tracking Scan (3 seconds)
            auto detections = bleSniffer.scan(3);
            persistenceEngine.processScanResults(detections);
            auto alerts = persistenceEngine.getActiveAlerts();

            ui.showSentinelStatus(alerts, persistenceEngine.getTrackedCount(), vBat);

            if (!alerts.empty()) {
                Serial.printf("[ALERT] Persistent Tracker Confirmed: %s (MAC: %s, RSSI: %d dBm)\n",
                              alerts[0].typeName.c_str(), alerts[0].identifier.c_str(), alerts[0].avgRssi);
                // Silent pocket vibration pulse (distinct double-buzz)
                power.triggerHapticAlert(200, 2);
            }
            break;
        }

        case MODE_ROOM_SWEEP_WIFI: {
            // Scan through Wi-Fi channels 1, 6, 11
            wifiSniffer.setChannel(currentWifiChannel);
            auto camAlerts = wifiSniffer.analyze(1500);

            ui.showWifiSweepStatus(camAlerts, currentWifiChannel, vBat);

            if (!camAlerts.empty() && camAlerts[0].isStreaming) {
                Serial.printf("[CAM ALERT] Active stream detected on Ch %d! OUI: %s\n",
                              currentWifiChannel, camAlerts[0].vendor.c_str());
                power.triggerHapticAlert(150, 3);
            }

            // Channel hopping logic
            if (currentWifiChannel == 1) currentWifiChannel = 6;
            else if (currentWifiChannel == 6) currentWifiChannel = 11;
            else currentWifiChannel = 1;
            break;
        }

        case MODE_OPTICAL_SEARCH: {
            ui.showOpticalSearchStatus(true);
            // Flash 650nm LEDs at 4Hz retro-reflection rate
            for (int i = 0; i < 8; i++) {
                checkModeButton();
                if (currentMode != MODE_OPTICAL_SEARCH) break;
                power.pulseOpticalLeds(4);
            }
            break;
        }
    }

    delay(200);
}
