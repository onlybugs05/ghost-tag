# GhostTag MVP Roadmap & Build Checklist

This document tracks progress from initial concept to a fully realized open-source hardware package ready for grant submission (Bagel Fund / OSHWA / Prototype Fund).

---

## Phase 1: Architecture & Firmware Core (Completed)
- [x] Define core threat model (Apple Find My, Google FMDN, Tile, SmartTag, RTSP spy cams).
- [x] Create project technical architecture (`docs/ARCHITECTURE.md`).
- [x] Formulate zero-printer enclosure strategy (JLCPCB industrial SLA black resin).
- [x] Build modular ESP32-S3 BLE sniffer (`firmware/src/ble_sniffer.cpp`).
- [x] Implement rolling temporal correlation engine (`firmware/src/heuristics.cpp`).
- [x] Implement 2.4GHz promiscuous Wi-Fi camera detector (`firmware/src/wifi_sniffer.cpp`).
- [x] Build SSD1306 OLED UI and signal meter interface (`firmware/src/ui_display.cpp`).
- [x] Implement power management, battery sensing, and deep sleep (`firmware/src/power_mgmt.cpp`).
- [x] Write hardware pinout and breadboard wiring guide (`docs/HARDWARE_GUIDE.md`).

---

## Phase 2: Hardware Engineering & Manufacturing Suite (Completed)
- [x] Finalize KiCad schematic and 2-layer PCB layout (`hardware/kicad/`).
- [x] Generate full set of RS-274X Gerber & Excellon drill files (`hardware/gerbers/`).
- [x] Package ready-to-order manufacturing archive (`hardware/GhostTag_Gerbers_v1.0.zip`).
- [x] Generate Bill of Materials with LCSC part numbers (`hardware/BOM.csv`).
- [x] Generate Pick-and-Place SMD component coordinates (`hardware/positions.csv`).

---

## Phase 3: 3D CAD Enclosure & Slicing Suite (Completed)
- [x] Design parametric 2-piece pocket keychain enclosure (Top and Bottom shells).
- [x] Enforce wall thickness constraint: **1.6 mm** (exceeds requirement > 1.2 mm).
- [x] Enforce thinnest feature constraint: **1.0 mm – 1.2 mm** (exceeds requirement ≥ 0.8 mm).
- [x] Export all CAD models in 5 formats: **STL, STP, STEP, OBJ, and 3MF** (`hardware/3d-models/`).
- [x] Write comprehensive 3D printing & JLCPCB upload instructions (`docs/3D_PRINTING_GUIDE.md`).

---

## Phase 4: Grant Submission & Funding (Current Phase)
- [x] Draft complete Grant Proposal and \$1,500 budget breakdown (`docs/GRANT_PROPOSAL.md`).
- [ ] Submit application to Bagel Fund (`https://bagel.fund`) with repository link.
- [ ] Complete 48-hour interview / confirmation.

---

## Phase 5: Post-Grant Pilot Production & Open Release
- [ ] Place batch order with JLCPCB for 50 custom assembled PCBs and SLA black resin shells.
- [ ] Solder displays and run 72-hour battery endurance tests with deep-sleep cycling.
- [ ] Conduct field validation tests with physical Apple AirTags and Samsung SmartTags.
- [ ] Record 60-second video demo showing silent pocket detection in transit.
- [ ] Distribute pilot sentinel kits to domestic violence shelters and privacy advocates.
