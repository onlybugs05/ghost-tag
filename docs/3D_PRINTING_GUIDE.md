# GhostTag Enclosure 3D Printing & Fabrication Guide

This guide details the specifications, design constraints, and step-by-step upload instructions for ordering or 3D printing the GhostTag pocket keychain enclosure.

---

## 1. File Formats & Geometry Compliance

All models in `hardware/3d-models/` have been generated and validated to strictly meet manufacturing thickness limits:

| Design Constraint | Required Specification | GhostTag Implemented Value | Compliance Status |
| :--- | :--- | :--- | :---: |
| **Wall Thickness** | **> 1.20 mm** | **1.60 mm** (Nominal outer shell) | **PASS** |
| **Thinnest Feature** | **≥ 0.80 mm** | **1.00 mm – 1.20 mm** (Ribs, bosses, lip) | **PASS** |
| **M2 Screw Boss Wall**| ≥ 0.80 mm | **1.15 mm** (OD 4.2mm, ID 1.9mm) | **PASS** |
| **Keyring Eyelet Wall**| ≥ 1.50 mm | **2.20 mm** (OD 8.0mm, ID 3.6mm) | **PASS** |

### Available Model Files (`hardware/3d-models/`)

Every model is exported in all 5 major CAD and slicer formats:

1. **Top Shell (`ghosttag_top_shell.*`)**:
   - Houses the 0.96" OLED display window (25x13mm), 8mm optical aperture, 4x 650nm LED lens holes, M2 screw standoffs, and reinforced keychain loop.
   - Formats: `.stl`, `.stp`, `.step`, `.obj`, `.3mf`

2. **Bottom Shell (`ghosttag_bottom_shell.*`)**:
   - Houses the internal 500mAh LiPo battery pocket, USB-C side port cutout (9.4x3.6mm), power switch slot, and matching screw posts.
   - Formats: `.stl`, `.stp`, `.step`, `.obj`, `.3mf`

3. **Assembled Model (`ghosttag_enclosure_assembly.*`)**:
   - Complete reference assembly for fit verification in CAD software (FreeCAD, Fusion 360, SolidWorks, Blender).
   - Formats: `.stl`, `.stp`, `.step`, `.obj`, `.3mf`

---

## 2. Option A: On-Demand Commercial Order (JLCPCB / PCBWay)

*Recommended for grant prototypes & end users who do not own a 3D printer. Cost is approx \$1.50–\$2.50 per set and can be shipped in the same box as the PCBs.*

### JLCPCB 3D Printing Upload Instructions

1. Go to [JLCPCB 3D Printing](https://jlcpcb.com/3d-printing).
2. Click **Upload 3D Files** and drag in either:
   - `hardware/3d-models/ghosttag_top_shell.step` (or `.stl`)
   - `hardware/3d-models/ghosttag_bottom_shell.step` (or `.stl`)
   *(You can upload both simultaneously).*
3. **Select Technology & Material:**
   - **Technology:** **SLA (Stereolithography)**
   - **Material:** **LEDO 6060 Resin** (White/Smooth) or **Black Resin (8228 / 8001)** for a sleek, covert matte black finish.
   - *(Alternative for extreme durability: **SLS Nylon PA12**).*
4. **Surface Finish:** Standard Sanding / Cleaning.
5. **Tolerance Check:** The automated JLCPCB portal will confirm wall thickness > 1.2mm and green-light the order instantly.
6. Add to cart alongside your `GhostTag_Gerbers_v1.0.zip` PCB order to consolidate shipping!

---

## 3. Option B: Self-Printing via FDM / SLA Desktop Slicers

### Recommended Slicer Settings (Bambu Studio, PrusaSlicer, OrcaSlicer, Cura)

| Parameter | Recommended Setting | Reason |
| :--- | :--- | :--- |
| **Material** | PETG, PLA+, or ABS/ASA | PETG offers superior impact resistance for everyday pocket carry |
| **Layer Height** | **0.12 mm – 0.16 mm** | Ensures clean layer adhesion on M2 screw bosses and smooth display bevels |
| **Perimeters / Walls** | **4 to 5 walls** | Guarantees solid perimeter walls without internal voids |
| **Infill** | **25% – 40% Gyroid** | High torsional rigidity with minimal weight |
| **Top/Bottom Layers** | 5 Top / 5 Bottom | Eliminates pinholes and ensures waterproof pocket resistance |
| **Supports** | **Tree / Organic Supports** (Only for USB-C and power switch side bridges) | Minimal cleanup required |

### Print Bed Orientation

- **Top Shell:** Place with the flat top face down on the build plate (textured PEI plate gives a premium matte look).
- **Bottom Shell:** Place with the flat bottom face down on the build plate.
- No brim required if bed adhesion is properly calibrated.

---

## 4. Hardware Assembly Bill of Fasteners

To securely fasten the printed enclosure:

| Item | Specification | Quantity | Purpose |
| :--- | :--- | :---: | :--- |
| **Case Screws** | M2 x 6mm or M2 x 8mm socket head cap / pan head screws | 4 | Fastens top and bottom shells through PCB corner holes |
| **Keyring** | 20mm – 25mm Flat Split Ring (Stainless Steel) | 1 | Attaches to external eyelet loop |
| **Acrylic Lens (Optional)** | 27 x 15 x 0.8mm clear acrylic sheet | 1 | Glues into internal display rebate to protect OLED |
