# GhostTag Architecture Specification

## 1. Overview

GhostTag runs as a standalone embedded sentinel on the ESP32-S3 dual-core microcontroller. The system is designed around non-blocking event loops, passive radio sniffing, and local heuristic correlation.

## 2. Core Subsystems

### 2.1 BLE Sniffer (`ble_sniffer.cpp`)
- Configured for passive scanning across advertising channels 37, 38, and 39.
- Decodes Manufacturer Specific Data fields for:
  - Apple Inc. (`0x004C`) with type `0x12` (offline finding).
  - Google LLC (`0x00E0`) and 16-bit Service UUIDs (`0xFEED`, `0xFE2C`).
  - Samsung Electronics (`0x0075`).
  - Tile Inc. (`0x00D0` / `0xFEED`).

### 2.2 Temporal Correlation Engine (`heuristics.cpp`)
- Tracks detected device identifiers in a fixed-size memory table.
- Evaluates temporal persistence: devices must be seen across multiple observation windows with average RSSI exceeding `-78 dBm`.
- Eliminates false alarms caused by pedestrian traffic or stationary devices.

### 2.3 802.11 Promiscuous Sniffer (`wifi_sniffer.cpp`)
- Enters 802.11 promiscuous mode via the ESP32 Wi-Fi driver.
- Extracts transmitter MAC addresses from management and data frames.
- Compares OUIs against known camera vendors (Tuya, Xiongmai, Hikvision, Dahua).
- Computes packet rates to identify continuous video streaming.

### 2.4 Power & Notification Manager (`power_mgmt.cpp`)
- Reads battery voltage using a 12-bit ADC and calibrated resistive divider.
- Generates distinct haptic pulses via a low-side N-FET.
- Controls low-power light/deep sleep cycles for extended battery operation.

### 2.5 User Interface (`ui_display.cpp`)
- Renders system state to a 128x64 SSD1306 OLED via I2C at 400 kHz.
- Displays RSSI signal meters, vendor badges, battery indicators, and alert notifications.
