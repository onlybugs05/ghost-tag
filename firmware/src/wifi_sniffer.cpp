#include "wifi_sniffer.h"
#include <map>

struct PacketStats {
    uint8_t mac[6];
    int rssi;
    uint32_t packetCount;
    uint32_t bytesCount;
};

static std::map<String, PacketStats> s_trafficMap;

// Promiscuous packet callback
static void wifiPromiscuousCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_DATA && type != WIFI_PKT_MGMT) return;

    const wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
    const uint8_t* payload = pkt->payload;
    uint16_t len = pkt->rx_ctrl.sig_len;

    if (len < 24) return; // 802.11 MAC header min size

    // Extract transmitter address (Addr2, bytes 10-15)
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             payload[10], payload[11], payload[12], payload[13], payload[14], payload[15]);

    String macKey(macStr);
    auto it = s_trafficMap.find(macKey);
    if (it == s_trafficMap.end()) {
        PacketStats stats;
        memcpy(stats.mac, &payload[10], 6);
        stats.rssi = pkt->rx_ctrl.rssi;
        stats.packetCount = 1;
        stats.bytesCount = len;
        s_trafficMap[macKey] = stats;
    } else {
        it->second.packetCount++;
        it->second.bytesCount += len;
        it->second.rssi = (it->second.rssi + pkt->rx_ctrl.rssi) / 2;
    }
}

WifiCameraSniffer::WifiCameraSniffer() : m_currentChannel(1), m_isRunning(false) {}

bool WifiCameraSniffer::init() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) return false;

    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_NULL);
    esp_wifi_start();
    return true;
}

void WifiCameraSniffer::startPromiscuous(uint8_t channel) {
    m_currentChannel = channel;
    s_trafficMap.clear();

    wifi_promiscuous_filter_t filter = {
        .filter_mask = WIFI_PROMIS_FILTER_MASK_DATA | WIFI_PROMIS_FILTER_MASK_MGMT
    };

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_filter(&filter);
    esp_wifi_set_promiscuous_rx_cb(&wifiPromiscuousCallback);
    esp_wifi_set_channel(m_currentChannel, WIFI_SECOND_CHAN_NONE);
    m_isRunning = true;
}

void WifiCameraSniffer::stopPromiscuous() {
    if (m_isRunning) {
        esp_wifi_set_promiscuous(false);
        m_isRunning = false;
    }
}

void WifiCameraSniffer::setChannel(uint8_t channel) {
    m_currentChannel = channel;
    esp_wifi_set_channel(m_currentChannel, WIFI_SECOND_CHAN_NONE);
}

String WifiCameraSniffer::lookupOUI(const uint8_t* mac) {
    // Known IoT / Spy Camera OUI prefixes
    if (mac[0] == 0x00 && mac[1] == 0x12 && mac[2] == 0x17) return "Tuya Smart";
    if (mac[0] == 0x70 && mac[1] == 0x8E && mac[2] == 0xDC) return "Tuya Smart";
    if (mac[0] == 0xA0 && mac[1] == 0x92 && mac[2] == 0x08) return "Tuya Smart";
    if (mac[0] == 0x00 && mac[1] == 0x1A && mac[2] == 0xFE) return "Hangzhou Xiongmai";
    if (mac[0] == 0x00 && mac[1] == 0x0C && mac[2] == 0x43) return "Ralink/Cam";
    if (mac[0] == 0x3C && mac[1] == 0x33 && mac[2] == 0x00) return "Hikvision";
    if (mac[0] == 0x44 && mac[1] == 0x19 && mac[2] == 0xB6) return "Hikvision";
    if (mac[0] == 0xBC && mac[1] == 0xBA && mac[2] == 0xE1) return "Dahua Tech";
    if (mac[0] == 0x24 && mac[1] == 0x0A && mac[2] == 0xC4) return "Espressif/DIY Cam";
    if (mac[0] == 0x18 && mac[1] == 0xFE && mac[2] == 0x34) return "Espressif/DIY Cam";
    if (mac[0] == 0x00 && mac[1] == 0x24 && mac[2] == 0xE4) return "Withings/IPCam";
    return "Generic WiFi";
}

std::vector<WifiCameraAlert> WifiCameraSniffer::analyze(uint32_t sampleTimeMs) {
    std::vector<WifiCameraAlert> alerts;
    delay(sampleTimeMs);

    for (const auto& pair : s_trafficMap) {
        const PacketStats& st = pair.second;
        String vendor = lookupOUI(st.mac);

        // Streaming signature: high packet density (typically > 50 packets/sec) or recognized camera vendor
        bool highThroughput = (st.packetCount > (sampleTimeMs / 20));
        bool isKnownVendor = (vendor != "Generic WiFi");

        if (isKnownVendor || highThroughput) {
            WifiCameraAlert alert;
            alert.bssid = pair.first;
            alert.vendor = vendor;
            alert.rssi = st.rssi;
            alert.packetRate = (st.packetCount * 1000) / sampleTimeMs;
            alert.isStreaming = highThroughput;
            alerts.push_back(alert);
        }
    }

    return alerts;
}
