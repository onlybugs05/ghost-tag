# GhostTag

GhostTag is an open-source, phone-independent firmware sentinel for the ESP32-S3 microcontroller. It passively monitors 2.4 GHz RF space to identify persistent Bluetooth Low Energy (BLE) tracking tags and local Wi-Fi video streaming devices.

## Features

- **Multi-Protocol BLE Beacon Detection:** Detects Apple Find My, Google Find My Device Network (FMDN), Samsung SmartTag, and Tile advertisement packets.
- **Temporal Correlation Engine:** Filters transient passerby devices using rolling time-window tracking and RSSI proximity thresholds.
- **Promiscuous Wi-Fi Stream Analysis:** Captures 802.11 frames to detect surveillance camera vendor OUIs and sustained streaming throughput.
- **Silent Haptic Alerts:** Triggers discrete vibration sequences using a coin ERM motor.
- **Visual Display:** Real-time OLED dashboard rendering RSSI meters, device badges, and battery voltage.

## Repository Structure

```
├── docs/
│   ├── ARCHITECTURE.md     # Detailed software architecture and data pipelines
│   ├── HARDWARE_GUIDE.md   # Pinout, breadboard schematic, and component list
│   └── THREAT_MODEL.md     # Threat model and beacon protocol characteristics
├── firmware/
│   ├── platformio.ini      # PlatformIO configuration for ESP32-S3
│   └── src/
│       ├── ble_sniffer.cpp # BLE scanning and packet decoding
│       ├── heuristics.cpp  # Temporal correlation filter
│       ├── main.cpp        # Master control loop and state transitions
│       ├── power_mgmt.cpp  # Battery telemetry and haptic driver
│       ├── ui_display.cpp  # SSD1306 OLED interface
│       └── wifi_sniffer.cpp# 802.11 promiscuous frame classifier
└── LICENSE                 # MIT License
```

## Hardware Setup & Pinout

| Subsystem | Microcontroller Pin | Target Peripheral |
| :--- | :--- | :--- |
| **I2C SDA** | GPIO 5 | SSD1306 OLED Display (128x64) |
| **I2C SCL** | GPIO 6 | SSD1306 OLED Display (128x64) |
| **Haptic Gate** | GPIO 4 | 2N7002 N-FET driving 1027 ERM Motor |
| **Mode Button** | GPIO 0 | Active-low tactical button |
| **Optical LEDs** | GPIO 7 | 650nm Red LED pulse array |
| **Battery ADC** | GPIO 1 | 1/2 resistive divider to V_BAT |

## Getting Started

### Prerequisites

- An ESP32-S3 development board (e.g., Seeed Studio XIAO ESP32-S3 or ESP32-S3-DevKitC-1).
- [PlatformIO Core or IDE](https://platformio.org/).

### Building and Flashing

1. Clone the repository:
   ```bash
   git clone https://github.com/onlybugs05/ghost-tag.git
   cd ghost-tag/firmware
   ```
2. Build the firmware:
   ```bash
   pio run
   ```
3. Connect your ESP32-S3 via USB-C and flash:
   ```bash
   pio run -t upload
   ```
4. Monitor serial diagnostics at 115200 baud:
   ```bash
   pio device monitor -b 115200
   ```

## Operating Modes

1. **BLE Sentinel Mode:** Continuously scans BLE advertising channels, updates the persistent tracker table, and alerts on correlated tracking signatures.
2. **Wi-Fi Sweep Mode:** Cycles across 2.4 GHz channels 1, 6, and 11 in promiscuous mode to flag covert camera OUIs and anomalous frame rates.
3. **Optical Search Mode:** Pulses 650 nm red LEDs at 4 Hz to facilitate manual retro-reflection inspection of hidden camera pinholes.

## License

This software is released under the MIT License. See [LICENSE](LICENSE) for details.
