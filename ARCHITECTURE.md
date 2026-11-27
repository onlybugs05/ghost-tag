# GhostTag Technical Architecture

This document describes the hardware design, RF detection pipeline, and power management architecture for GhostTag.

---

## 1. System Overview

GhostTag is an autonomous physical device engineered to detect persistent tracking beacons and local surveillance streams without depending on external network infrastructure or cloud servers.

```
+------------------------------------------------------------------------+
|                               GHOSTTAG                                 |
|                                                                        |
|  [ Antenna ]                                                           |
|       │                                                                |
|       ▼                                                                |
|  +--------------------+      +---------------------------------------+ |
|  |     ESP32-S3       |      |       Detection Engine                | |
|  |  Dual-Core LX7     | ---> |  • Bluetooth LE Advertisement Filter  | |
|  |  2.4 GHz RF Trans. |      |  • Wi-Fi Frame Classifier             | |
|  +--------------------+      +---------------------------------------+ |
|            │                                     │                     |
|            │                                     ▼                     |
|            │                          +---------------------+          |
|            │                          |  Correlation Table  |          |
|            │                          |  (LittleFS Buffer)  |          |
|            │                          +---------------------+          |
|            ▼                                     │                     |
|  +-----------------------------------------------+                     |
|  │                                                                     |
|  ▼                                               ▼                     |
|  [ 0.96" I2C OLED ]                     [ Haptic Motor Driver ]        |
|  SSD1306 Display                        ERM Vibration (Silent Alert)   |
+------------------------------------------------------------------------+
```

---

## 2. Hardware Subsystems

### 2.1 Microcontroller Unit (MCU)
* **Processor:** ESP32-S3 (Xtensa Dual-Core 32-bit LX7, up to 240 MHz).
* **Wireless:** 2.4 GHz Wi-Fi (802.11 b/g/n) and Bluetooth Low Energy 5.0.
* **Internal SRAM:** 512 KB SRAM + 384 KB ROM.
* **Flash Memory:** 4 MB to 8 MB SPI Flash (holds firmware, device database, and circular event log).

### 2.2 Alert and Display Subsystem
* **Primary Notification (Silent):** 3V 1027 coin-type Eccentric Rotating Mass (ERM) haptic motor driven via an N-channel MOSFET switch (2N7002 or similar) with flyback diode protection.
* **Visual Screen:** 0.96" Monochrome OLED (SSD1306) communicating over I2C at 400 kHz (SDA: GPIO 5, SCL: GPIO 6).
* **Optical Retro-Reflection Array:** Four 650 nm high-intensity red LEDs positioned around a central viewing hole for physical camera lens reflection inspection.

### 2.3 Power Management
* **Battery:** 3.7 V single-cell Lithium Polymer (LiPo), 400 mAh to 500 mAh.
* **Charger Circuit:** TP4056 or MCP73831 USB Type-C charge controller with constant-current/constant-voltage profiling.
* **Voltage Regulation:** Ultra-low quiescent current Low-Dropout Regulator (LDO) outputting a steady 3.3 V (e.g., ME6211 or RT9013).

---

## 3. Detection Engine & State Logic

### 3.1 Bluetooth LE Hunter Pipeline

Commercial trackers broadcast periodic Bluetooth Low Energy advertisement packets. While trackers randomize their MAC addresses over intervals (typically every 15 minutes), their broadcast payload formats retain identifiable characteristics:

1. **Passive Radio Sniffing:** The radio periodically scans standard BLE advertising channels (37, 38, 39).
2. **Payload Extraction:**
   * Extracts Manufacturer Specific Data fields.
   * Identifies payload structures and public key prefixes.
3. **Temporal Tracking Table:**
   * Detected payloads and signal strengths (RSSI) are logged to a rolling in-memory table.
   * If a target device signature maintains close proximity across consecutive measurement intervals, the correlation engine increments a suspicion counter.
4. **Trigger Threshold:** When the suspicion score exceeds threshold criteria (e.g., strong RSSI maintained across three observation windows), the silent haptic motor activates.

### 3.2 Wi-Fi Surveillance Stream Detection

When configured in Room Sweep mode:
* The Wi-Fi radio enters promiscuous mode to capture raw 802.11 MAC frames.
* **OUI Lookup:** Inspects MAC address Organizational Unique Identifiers against known vendors of wireless camera modules.
* **Packet Burst Classification:** Flags anomalous high-throughput continuous UDP or RTSP streaming patterns characteristic of active video feeds.

---

## 4. Power & Duty-Cycle Budget

To achieve multi-day operation on a compact 500 mAh battery:
* **Active BLE Scan Phase:** 65 mA for 3 seconds every 60 seconds.
* **Deep Sleep Phase:** ~15 µA during inactive periods with RTC timer wake-up.
* **Average Current Consumption:** Estimated at ~3.5 mA under typical duty cycling.
* **Theoretical Run Time:** ~100 to 120 hours of continuous protection per full charge.
