#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "heuristics.h"
#include "wifi_sniffer.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDR 0x3C

enum OperatingMode {
    MODE_SENTINEL_BLE,
    MODE_ROOM_SWEEP_WIFI,
    MODE_OPTICAL_SEARCH
};

class UiDisplay {
public:
    UiDisplay(uint8_t sdaPin, uint8_t sclPin);
    bool init();
    void showSplash();
    void showSentinelStatus(const std::vector<TrackedTarget>& alerts, size_t totalTracked, float battVoltage);
    void showWifiSweepStatus(const std::vector<WifiCameraAlert>& alerts, uint8_t channel, float battVoltage);
    void showOpticalSearchStatus(bool ledsActive);
    void clear();

private:
    Adafruit_SSD1306 m_display;
    uint8_t m_sdaPin;
    uint8_t m_sclPin;

    void drawSignalMeter(int16_t x, int16_t y, int rssi);
    void drawBatteryGauge(int16_t x, int16_t y, float voltage);
};

#endif // UI_DISPLAY_H
