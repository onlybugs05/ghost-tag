# 👻 GhostTag

> **Autonomous Pocket Anti-Stalking & Hidden Surveillance Detector**  
> *100% Free & Open-Source Hardware (OSHW) • Zero Cloud • Zero Subscriptions • Phone-Independent*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Hardware: CERN-OHL-P](https://img.shields.io/badge/Hardware-CERN--OHL--P-blue.svg)](https://ohwr.org/cern_ohl_p_v2.txt)
[![Status: MVP Complete - Grant Ready](https://img.shields.io/badge/Status-Grant%20Ready-brightgreen.svg)](#)

---

## 🎯 The Problem

Physical stalking and non-consensual tracking have exploded with the widespread use of cheap micro-trackers (Apple AirTags, Google Find My Device beacons, Samsung SmartTags, Tile). Meanwhile, illicit pinhole spy cameras in hotel rooms, rental properties, and dressing rooms remain a persistent personal security threat.

**Why existing defenses fail:**
1. **Smartphone apps are broken & delayed:** Apple’s "Tracker Detect" app for Android requires you to manually start a scan—it does not run continuously in the background. Google's background alerts only scan periodically to save phone battery, often warning you hours after a tracker has already mapped your route.
2. **Commercial detectors are overpriced e-waste:** Most \$50–\$100 "bug detectors" on Amazon are closed-source junk that sound false alarms on random 2.4GHz home Wi-Fi routers.
3. **Privacy should not require a subscription:** Real personal security must be affordable, offline, and universally accessible.

---

## 🛡️ What is GhostTag?

**GhostTag** is a dedicated, standalone keychain/pocket sentinel powered by the **ESP32-S3**. It operates 24/7 in your bag or pocket without needing your phone, internet connection, or proprietary cloud accounts.

```
       ┌────────────────────────────────────────────────────────┐
       │                   👻 GHOSTTAG POCKET                   │
       │                                                        │
       │   [BLE 5.0 Engine] ───> Apple Find My (0x004C)         │
       │                         Google FMDN (0xFEAA/0xFEED)    │
       │                         Samsung & Tile Beacons         │
       │                                                        │
       │   [Wi-Fi Engine]   ───> 2.4GHz RTSP Video Frame Sniff  │
       │                         Surveillance Camera OUI Check  │
       │                                                        │
       │   [Alert System]   ───> Silent Haptic Vibration Motor   │
       │                         0.96" Crisp OLED Display       │
       │                         Pulsed 650nm Optical Lens Ring │
       └────────────────────────────────────────────────────────┘
```

### Key Capabilities

- **🔕 Silent Pocket Haptic Alert:** When an unknown tracker follows you across time, GhostTag vibrates discreetly against your leg or inside your bag without making a noise that alerts the stalker.
- **📡 Multi-Ecosystem BLE Hunter:** Detects Apple Find My (`0x004C`, Type `0x12`), Google Find My Device Network (FMDN), Samsung SmartTag, and Tile advertisement bursts.
- **📈 Persistence & Proximity Heuristics:** Employs a rolling circular buffer. If a device maintains consistent RSSI (> -75 dBm) across rolling 15-minute time windows, it triggers a confirmed pursuit alert.
- **🏨 Room Surveillance Sweep Mode:** Switches Wi-Fi into promiscuous mode to detect local active video streaming bursts (RTSP/UDP) and flags MAC address OUIs registered to covert surveillance cameras (Tuya, Xiongmai, etc.).
- **👁️ Optical Retro-Reflection Lens Finder:** High-intensity pulsed 650nm red LEDs around a central viewing aperture allow you to spot the reflection of hidden pinhole camera lenses in the dark.
- **⚡ Weeks of Battery Life:** Deep-sleep duty cycling allows continuous multi-day operation from a tiny 500mAh LiPo cell.

---

## 🛠️ Hardware Specifications

| Subsystem | Component | Purpose |
| :--- | :--- | :--- |
| **SoC / MCU** | ESP32-S3 (Dual-core Xtensa LX7 @ 240MHz) | Wi-Fi 802.11 b/g/n + Bluetooth 5 (LE) |
| **Display** | 0.96" Monochrome I2C OLED (SSD1306, 128x64) | Shows tracker RSSI, type, MAC, and signal history |
| **Haptic Feedback** | Miniature ERM Vibration Motor (1027 coin type) | Silent alerts inside your pocket/bag |
| **Battery & Power** | 3.7V 500mAh LiPo + TP4056 / AXP2101 USB-C PMIC | Rechargeable via standard USB-C |
| **Optical Ring** | 4x 650nm Deep Red High-Output LEDs | Pulsed retro-reflection camera lens inspection |
| **Enclosure** | On-Demand SLA Black Resin (JLCPCB) or 3D Printed Shell | Pocket-sized with keychain loop (50.4 x 34.4 x 15 mm) |

---

## 💰 How Much Does It Cost to Build One Yourself?

GhostTag was intentionally engineered with common, off-the-shelf maker components so **anyone can build their own for under \$15–\$20**.

### Single-Unit DIY Bill of Materials (BOM)

| Part | Description | Single-Unit Cost (USD) | Where to Buy |
| :--- | :--- | :---: | :--- |
| **ESP32-S3 Dev Board** | Seeed Studio XIAO ESP32-S3 or ESP32-S3 Mini | \$4.50 – \$5.50 | AliExpress / Amazon / Mouser |
| **0.96" OLED Display** | I2C SSD1306 128x64 Blue/White | \$1.50 – \$2.20 | AliExpress / Amazon |
| **LiPo Battery** | 3.7V 400–500mAh (Size: 502535 or 602030) | \$3.00 – \$3.80 | Hobby shops / AliExpress |
| **Charging Board** | Micro TP4056 USB-C with battery protection | \$0.60 – \$1.00 | Local electronics / Amazon |
| **Vibration Motor** | 1027 Coin Type Haptic Motor (3V) | \$0.50 – \$0.80 | Amazon / AliExpress |
| **Optics & Switches** | 4x 650nm Red LEDs + mini slide power switch | \$1.00 – \$1.50 | Any electronics retailer |
| **Enclosure** | On-demand SLA black resin via JLCPCB or standard 3D print | ~\$1.50 – \$2.50 | JLCPCB / Local Slicer |
| **Total DIY Cost** | *(Entire standalone unit)* | **~\$12 – \$16 USD** | *(Cheaper than a single AirTag)* |

---

## 📂 Repository Structure

```
ghost-tag/
├── docs/
│   ├── ARCHITECTURE.md          # Full hardware, firmware, and RF sniffing spec
│   ├── HARDWARE_GUIDE.md        # Pinouts and breadboard wiring guide
│   ├── 3D_PRINTING_GUIDE.md     # Enclosure specs, slicing settings, and JLCPCB upload guide
│   └── GRANT_PROPOSAL.md        # Complete grant proposal and budget for Bagel Fund / OSHWA
├── hardware/
│   ├── kicad/                   # KiCad 7/8 schematic and 2-layer PCB layout
│   ├── gerbers/                 # RS-274X Gerbers and Excellon drill files
│   ├── GhostTag_Gerbers_v1.0.zip# 1-Click ready-to-order manufacturing ZIP
│   ├── BOM.csv                  # Bill of Materials with LCSC part numbers
│   ├── positions.csv            # Pick and Place SMD component coordinates
│   └── 3d-models/               # STL, STP, STEP, OBJ, and 3MF enclosure CAD files
│       ├── ghosttag_top_shell.*
│       ├── ghosttag_bottom_shell.*
│       └── ghosttag_enclosure_assembly.*
├── firmware/
│   ├── src/                     # Modular ESP32-S3 firmware
│   │   ├── ble_sniffer.cpp/.h   # Multi-protocol BLE beacon sniffer
│   │   ├── heuristics.cpp/.h    # Rolling temporal window correlation engine
│   │   ├── wifi_sniffer.cpp/.h  # 2.4GHz promiscuous RTSP/UDP camera detector
│   │   ├── ui_display.cpp/.h    # SSD1306 OLED interface and signal strength meters
│   │   ├── power_mgmt.cpp/.h    # Battery sensing, haptic motor, and deep sleep
│   │   └── main.cpp             # Central operational state machine
│   └── platformio.ini           # PlatformIO build configuration
├── scripts/
│   ├── generate_hardware.py     # Automates KiCad & Gerber fabrication suite
│   └── generate_3d_models.py    # Parametric CadQuery 3D model generator
├── LICENSE                      # MIT License
└── README.md                    # Project Overview & Quickstart
```

---

## 🖨️ 3D Printing & Enclosure Specifications

All CAD models in `hardware/3d-models/` strictly conform to manufacturing tolerances:
- **Wall Thickness:** **1.6 mm** (exceeds the **> 1.2 mm** requirement).
- **Thinnest Part:** **1.0 mm – 1.2 mm** (exceeds the **≥ 0.8 mm** requirement).
- **Available Formats:** **STL, STP, STEP, OBJ, and 3MF**.
- **Upload Guide:** Detailed one-click ordering instructions for JLCPCB 3D Printing & desktop slicing profiles (Bambu Studio, PrusaSlicer, Cura) are provided in [3D_PRINTING_GUIDE.md](file:///home/hrushikesh/ghost-tag/docs/3D_PRINTING_GUIDE.md).

---

## 🚀 Quickstart (Firmware Flash)

### Prerequisites
- ESP32-S3 development board
- [PlatformIO](https://platformio.org/) installed (`pio`)

### Build & Upload
```bash
cd firmware
pio run -t upload
```

---

## 🗺️ Roadmap & Grant Milestones

- [x] RF payload signature mapping for Apple Find My, Google FMDN, Tile & SmartTag
- [x] Initial hardware BOM and DIY cost architecture
- [x] KiCad 2-layer compact PCB layout & production Gerber files (`GhostTag_Gerbers_v1.0.zip`)
- [x] ESP32-S3 firmware core: BLE rolling buffer correlation algorithm
- [x] Wi-Fi promiscuous RTSP camera traffic classifier & OUI inspector
- [x] 3D printable snap-fit keychain enclosure (STL, STP, STEP, OBJ, 3MF)
- [x] Complete Grant Proposal, Budget Breakdown & Milestones ([GRANT_PROPOSAL.md](file:///home/hrushikesh/ghost-tag/docs/GRANT_PROPOSAL.md))
- [ ] First pilot batch assembly (50 units) via grant funding
- [ ] Field validation video and community shelter distribution

---

## 📜 License

- **Firmware & Software:** [MIT License](LICENSE)
- **Hardware Schematics & 3D Enclosures:** [CERN-OHL-P-2.0](https://ohwr.org/cern_ohl_p_v2.txt) (Permissive Open Hardware License)
