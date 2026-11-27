#ifndef WIFI_SNIFFER_H
#define WIFI_SNIFFER_H

#include <Arduino.h>
#include <esp_wifi.h>
#include <vector>

struct WifiCameraAlert {
    String bssid;
    String vendor;
    int rssi;
    uint32_t packetRate;
    bool isStreaming;
};

class WifiCameraSniffer {
public:
    WifiCameraSniffer();
    bool init();
    void startPromiscuous(uint8_t channel = 1);
    void stopPromiscuous();
    void setChannel(uint8_t channel);
    std::vector<WifiCameraAlert> analyze(uint32_t sampleTimeMs = 2000);
    static String lookupOUI(const uint8_t* mac);

private:
    uint8_t m_currentChannel;
    bool m_isRunning;
};

#endif // WIFI_SNIFFER_H
