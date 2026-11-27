# GhostTag Hardware & Assembly Guide

This guide details the physical hardware architecture, custom 2-layer PCB specifications, and component assembly for GhostTag.

---

## 1. Electrical Architecture & Chip Selection

| Functional Block | Primary IC / Part | Package | Circuit Function & Rationale |
| :--- | :--- | :--- | :--- |
| **System on Chip (SoC)** | **ESP32-S3-WROOM-1-N8** | SMD Module | Xtensa 32-bit dual-core @ 240MHz. Provides dual 2.4GHz RF transceivers for BLE 5.0 beacon hunting and Wi-Fi 802.11 promiscuous frame analysis. |
| **Charge Management (PMIC)**| **MCP73831T-2ACI/OT** | SOT-23-5 | Linear Li-Ion/LiPo charge controller with constant current/constant voltage profiling, automatic preconditioning, and charge status output. |
| **Voltage Regulator (LDO)** | **ME6211C33M5G** | SOT-23-5 | High-accuracy LDO outputting 3.3V @ up to 500mA. Ultra-low dropout (100mV @ 100mA) and 15µA quiescent current for long standby operation. |
| **Haptic Motor Driver** | **2N7002 / AO3400A** | SOT-23 | Low-side N-channel MOSFET gate switch driven by GPIO 4. Includes a 1N4148W flyback diode to suppress inductive motor EMF spikes. |
| **Optical Lens Array Driver**| **AO3400A / 2N7002** | SOT-23 | High-efficiency N-channel switch driving 4x 650nm deep red LEDs at 80mA pulsed current for camera lens reflection detection. |
| **Display Interface** | **0.96" SSD1306 OLED** | 4-Pin 2.54mm | 128x64 monochrome graphic display communicating over hardware I2C (SDA: GPIO 5, SCL: GPIO 6) at 400kHz. |
| **USB Interface** | **16-Pin USB Type-C** | Hybrid SMD/THT| Provides 5V VBUS power and serial flashing/debugging. Features dual 5.1kΩ CC pulldowns for compatibility with USB-C PD power sources. |
| **Battery Interface** | **JST-PH 2.0mm 2-Pin** | Right Angle | Plugs into a 3.7V 500mAh LiPo cell fitted inside the dedicated battery recess of the bottom shell. |

---

## 2. Mechanical Datum & 3D Enclosure Alignment

The PCB ($46.0 \text{ mm} \times 30.0 \text{ mm}$) and the 3D enclosure are built from a single synchronized parameter model:

```
  Y (Width: 30mm)
  ▲
  │   [Mount Hole 3]                                  [Mount Hole 4]
  │   (3.0, 27.0)                                     (43.0, 27.0)
  │        ┌───────────────────────────────────────────────┐
  │        │  [OLED 128x64 Display]    (Mode Button: 34.0) │
  │        │   Center: (14.0, 15.0)           ┌─────┐      │
  │[USB-C] │                                  │ (o) │ ◄────┼── [8mm Optical Hole & 4x LEDs]
  │ (0.0,  │                                  └─────┘      │   Center: (34.0, 15.0)
  │  15.0) │                                               │   LED Radius: 7.0mm
  │        │                       (Slide Switch: 23.0)    │
  │        └───────────────────────────────────────────────┘
  │   [Mount Hole 1]                                  [Mount Hole 2]
  │   (3.0, 3.0)                                      (43.0, 3.0)
  └─────────────────────────────────────────────────────────────────► X (Length: 46mm)
```

### Alignment Points:
1. **M2 Fastener Bosses:** 4 holes at `(3.0, 3.0)`, `(43.0, 3.0)`, `(3.0, 27.0)`, `(43.0, 27.0)` mm line up with the 3D printed standoffs in both top and bottom shells.
2. **USB-C Cutout:** Left end cutout ($9.4 \times 3.6$ mm) matches the PCB USB-C receptacle center at $Y=15.0$ mm.
3. **Power Switch Slot:** Bottom wall cutout ($6.0 \times 3.2$ mm) matches the SPDT slide switch at $X=23.0$ mm.
4. **Mode Button Plunger:** Top wall circular hole ($\varnothing 3.5$ mm) aligns with the tactile switch at $X=34.0$ mm.
5. **Optical Inspection Viewport:** $\varnothing 8.0$ mm circular viewing window and 4 surrounding LED ports align with the optical sensor array at $X=34.0, Y=15.0$ mm.
6. **Display Frame:** Top shell window ($24.5 \times 13.0$ mm) frames the active pixel matrix of the 0.96" OLED display.

---

## 3. PCB Fabrication Specifications

- **Layers:** 2-Layer FR-4 Standard
- **Dimensions:** 46.00 mm x 30.00 mm
- **Board Thickness:** 1.60 mm
- **Copper Weight:** 1 oz (35 µm) outer layers
- **Surface Finish:** Lead-Free HASL or ENIG (Electroless Nickel Immersion Gold)
- **Minimum Trace/Space:** 0.20 mm / 0.20 mm (8 mil)
- **Minimum Drill:** 0.30 mm
- **Solder Mask:** Matte Black or Green
- **Silkscreen:** High-contrast White
- **Gerber Package:** `hardware/GhostTag_Production_Gerbers.zip`
