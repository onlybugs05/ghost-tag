#include "ui_display.h"
#include <Wire.h>

UiDisplay::UiDisplay(uint8_t sdaPin, uint8_t sclPin)
    : m_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      m_sdaPin(sdaPin), m_sclPin(sclPin) {}

bool UiDisplay::init() {
    Wire.begin(m_sdaPin, m_sclPin);
    if (!m_display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        return false;
    }
    m_display.clearDisplay();
    m_display.display();
    return true;
}

void UiDisplay::showSplash() {
    m_display.clearDisplay();
    m_display.setTextSize(2);
    m_display.setTextColor(SSD1306_WHITE);
    m_display.setCursor(14, 10);
    m_display.println("GHOSTTAG");
    
    m_display.setTextSize(1);
    m_display.setCursor(18, 34);
    m_display.println("Pocket Sentinel");
    m_display.setCursor(14, 48);
    m_display.println("OSHW v1.0 • OSHW");
    m_display.display();
    delay(1500);
}

void UiDisplay::drawBatteryGauge(int16_t x, int16_t y, float voltage) {
    // Battery icon outline: 16x8
    m_display.drawRect(x, y, 16, 8, SSD1306_WHITE);
    m_display.fillRect(x + 16, y + 2, 2, 4, SSD1306_WHITE); // Battery nipple

    int pct = (int)((voltage - 3.3f) / (4.2f - 3.3f) * 12.0f);
    if (pct < 0) pct = 0;
    if (pct > 12) pct = 12;

    if (pct > 0) {
        m_display.fillRect(x + 2, y + 2, pct, 4, SSD1306_WHITE);
    }
}

void UiDisplay::drawSignalMeter(int16_t x, int16_t y, int rssi) {
    int bars = 1;
    if (rssi > -60) bars = 4;
    else if (rssi > -70) bars = 3;
    else if (rssi > -80) bars = 2;

    for (int i = 0; i < 4; i++) {
        int h = (i + 1) * 2;
        if (i < bars) {
            m_display.fillRect(x + (i * 3), y + (8 - h), 2, h, SSD1306_WHITE);
        } else {
            m_display.drawPixel(x + (i * 3), y + 7, SSD1306_WHITE);
        }
    }
}

void UiDisplay::showSentinelStatus(const std::vector<TrackedTarget>& alerts, size_t totalTracked, float battVoltage) {
    m_display.clearDisplay();

    // Header bar
    m_display.setTextSize(1);
    m_display.setTextColor(SSD1306_WHITE);
    m_display.setCursor(0, 0);
    m_display.print("GHOSTTAG: BLE");

    drawBatteryGauge(108, 0, battVoltage);

    m_display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    if (!alerts.empty()) {
        const auto& alert = alerts[0];
        // Inverted alert banner
        m_display.fillRect(0, 13, 128, 11, SSD1306_WHITE);
        m_display.setTextColor(SSD1306_BLACK);
        m_display.setCursor(2, 15);
        m_display.printf("! PURSUIT ALERT !");

        m_display.setTextColor(SSD1306_WHITE);
        m_display.setCursor(0, 27);
        m_display.printf("Type: %s", alert.typeName.c_str());

        m_display.setCursor(0, 39);
        m_display.printf("MAC: %s", alert.identifier.substring(0, 14).c_str());

        m_display.setCursor(0, 51);
        m_display.printf("RSSI: %d dBm", alert.avgRssi);
        drawSignalMeter(90, 51, alert.avgRssi);
    } else {
        m_display.setCursor(0, 16);
        m_display.println("STATUS: Safe / Clear");
        m_display.setCursor(0, 28);
        m_display.printf("Tracking Buffer: %u", (unsigned int)totalTracked);
        m_display.setCursor(0, 40);
        m_display.println("Scanning Channels 37-39");
        m_display.setCursor(0, 52);
        m_display.println("Silent Haptic Armed");
    }

    m_display.display();
}

void UiDisplay::showWifiSweepStatus(const std::vector<WifiCameraAlert>& alerts, uint8_t channel, float battVoltage) {
    m_display.clearDisplay();

    // Header bar
    m_display.setTextSize(1);
    m_display.setTextColor(SSD1306_WHITE);
    m_display.setCursor(0, 0);
    m_display.printf("SWEEP: CH %d", channel);

    drawBatteryGauge(108, 0, battVoltage);
    m_display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    if (!alerts.empty()) {
        const auto& cam = alerts[0];
        m_display.fillRect(0, 13, 128, 11, SSD1306_WHITE);
        m_display.setTextColor(SSD1306_BLACK);
        m_display.setCursor(2, 15);
        m_display.println("! CAM DETECTED !");

        m_display.setTextColor(SSD1306_WHITE);
        m_display.setCursor(0, 27);
        m_display.printf("OUI: %s", cam.vendor.c_str());

        m_display.setCursor(0, 39);
        m_display.printf("Rate: %u pkt/s", (unsigned int)cam.packetRate);

        m_display.setCursor(0, 51);
        m_display.printf("RSSI: %d dBm", cam.rssi);
        drawSignalMeter(90, 51, cam.rssi);
    } else {
        m_display.setCursor(0, 18);
        m_display.println("Sniffing 2.4GHz RTSP...");
        m_display.setCursor(0, 32);
        m_display.println("Analyzing 802.11 frames");
        m_display.setCursor(0, 46);
        m_display.println("No covert streams.");
    }

    m_display.display();
}

void UiDisplay::showOpticalSearchStatus(bool ledsActive) {
    m_display.clearDisplay();
    m_display.setTextSize(1);
    m_display.setTextColor(SSD1306_WHITE);
    m_display.setCursor(0, 0);
    m_display.println("OPTICAL LENS FINDER");
    m_display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    m_display.setCursor(0, 18);
    m_display.printf("650nm LEDs: %s\n", ledsActive ? "PULSING [ON]" : "STANDBY");
    m_display.setCursor(0, 32);
    m_display.println("Look through center");
    m_display.setCursor(0, 44);
    m_display.println("viewing aperture for");
    m_display.setCursor(0, 54);
    m_display.println("retro-reflections.");
    m_display.display();
}

void UiDisplay::clear() {
    m_display.clearDisplay();
    m_display.display();
}
