# GhostTag Hardware & Wiring Guide

This guide details the components and connections required to wire a working breadboard MVP.

---

## 1. Components Required for MVP

| Component | Recommended Model | Notes |
| :--- | :--- | :--- |
| **Microcontroller** | Seeed XIAO ESP32-S3 or ESP32-S3 DevKit | Dual-core, onboard antenna, USB-C |
| **Display** | 0.96" Monochrome I2C OLED (SSD1306) | 128x64 resolution, 4-pin interface |
| **Haptic Motor** | 1027 Coin-Type ERM Vibration Motor (3V) | Silent pocket notification |
| **Switching Transistor** | 2N7002 / 2N2222 NPN or N-FET | Drives motor from 3.3V GPIO |
| **Flyback Diode** | 1N4148 or similar | Protects MCU from inductive voltage spikes |
| **Resistors** | 1k Ohm (gate/base) + 10k Ohm (pull-down) | Motor switching network |
| **Power Supply** | 3.7V 500mAh LiPo Cell + TP4056 USB-C board | Or direct USB-C power for bench testing |

---

## 2. Pinout Connections

### 2.1 OLED Display (I2C)
| OLED Pin | ESP32-S3 Pin | Function |
| :--- | :--- | :--- |
| **VCC** | 3V3 | Power (3.3V) |
| **GND** | GND | Ground |
| **SCL** | GPIO 6 (or default SCL) | I2C Clock |
| **SDA** | GPIO 5 (or default SDA) | I2C Data |

### 2.2 Haptic Vibration Motor Circuit
Because the ESP32-S3 GPIO pins supply a maximum of ~20mA to 40mA, do not connect the vibration motor directly to a GPIO pin. Use a simple transistor switch:

```
                  +3.3V (or V_BAT)
                      │
                      ├─── [ + ] Vibration Motor
                      │            │
             [ 1N4148 ] (Cathode)  ├── [ - ] Vibration Motor
             Diode    │            │
                      └─── [ Anode ]
                                   │
                                   ├── Drain / Collector (2N7002 / 2N2222)
                                   │
   ESP32-S3 GPIO 4 ── [ 1k ] ──────┤ Gate / Base
                                   │
                                  [ 10k ] Pull-down
                                   │
                                  GND
```

* **Vibration Control Pin:** ESP32-S3 GPIO 4
* When GPIO 4 is driven `HIGH`, the transistor conducts to ground, activating the motor.

---

## 3. Enclosure Fabrication (No 3D Printer Required)

You do not need to own a 3D printer:
1. **On-Demand Industrial SLA Resin:** Upload the enclosure `.stl` files directly to the JLCPCB / PCBWay 3D printing portal when placing your PCB order. Select **Black Resin** (SLA) or **PA12 Nylon** (SLS).
2. **Cost:** Approximately \$2.00 to \$3.00 per shell.
3. **Consolidated Shipping:** The printed cases ship inside the same carton as your circuit boards, adding zero extra freight fees.
