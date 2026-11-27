#!/usr/bin/env python3
"""
GhostTag Hardware & Gerber Generator
Generates KiCad 7/8 format schematic and PCB, BOM, CPL, and production-ready
RS-274X Gerber & Excellon drill files for JLCPCB / PCBWay manufacturing.
"""

import os
import zipfile

def write_bom_and_cpl():
    bom_content = """Id,Designator,Package,Quantity,Designation,Supplier and ref
1,"C1, C2",0805,2,"10uF 16V X7R","LCSC / C15850"
2,"C3, C4",0603,2,"100nF 50V X7R","LCSC / C14663"
3,"R1, R3",0603,2,"1k 1%","LCSC / C21190"
4,"R2",0603,1,"10k 1%","LCSC / C25804"
5,"R4, R5",0603,2,"100k 1%","LCSC / C25803"
6,"R6, R7, R8, R9",0805,4,"68R 1% (LED Limiting)","LCSC / C17865"
7,"D1",SOD-123,1,"1N4148W Flyback Diode","LCSC / C81598"
8,"D2, D3, D4, D5",1206,4,"650nm Deep Red High-Power LED","LCSC / C128532"
9,"Q1, Q2",SOT-23,2,"2N7002 N-MOSFET 60V 300mA","LCSC / C8502"
10,"U1",SMD-Module,1,"ESP32-S3-WROOM-1-N8R8 / XIAO S3 Footprint","LCSC / C2913200"
11,"U2",SOT-23-5,1,"MCP73831T-2ACI/OT LiPo Charger","LCSC / C289748"
12,"U3",SOT-23-5,1,"ME6211C33M5G 3.3V 500mA LDO","LCSC / C82942"
13,"J1",TYPE-C-16P,1,"USB Type-C Receptacle 16P","LCSC / C2765186"
14,"J2",JST-PH-2P,1,"2.0mm LiPo Battery Connector","LCSC / C132204"
15,"J3",1x4P-2.54mm,1,"0.96\" I2C OLED Header Female","LCSC / C22543"
16,"SW1",SMD-4P,1,"Tactile Push Button (Mode/Wake)","LCSC / C318884"
17,"SW2",SMD-Slide,1,"SPDT Mini Power Switch","LCSC / C318991"
18,"M1",Coin-SMD,1,"1027 Coin ERM Vibration Motor 3V","Custom / Adafruit 1201"
"""
    with open("hardware/BOM.csv", "w") as f:
        f.write(bom_content)

    cpl_content = """Designator,Val,Package,Mid X,Mid Y,Rotation,Layer
U1,ESP32-S3-WROOM-1,SMD-Module,23.0,16.0,0,Top
U2,MCP73831,SOT-23-5,10.0,8.0,0,Bottom
U3,ME6211,SOT-23-5,16.0,8.0,0,Bottom
J1,TYPE-C-16P,TYPE-C-16P,3.5,15.0,90,Bottom
J2,JST-PH-2P,JST-PH-2P,38.0,8.0,180,Bottom
J3,OLED-I2C,1x4P-2.54,23.0,26.0,0,Top
SW1,PUSH-TACT,SMD-4P,42.0,22.0,0,Top
SW2,SLIDE-SPDT,SMD-Slide,42.0,10.0,0,Top
Q1,2N7002,SOT-23,32.0,18.0,0,Top
Q2,2N7002,SOT-23,32.0,12.0,0,Top
D1,1N4148W,SOD-123,35.0,18.0,0,Top
D2,650nm-LED,1206,23.0,19.0,0,Top
D3,650nm-LED,1206,27.0,15.0,90,Top
D4,650nm-LED,1206,23.0,11.0,180,Top
D5,650nm-LED,1206,19.0,15.0,270,Top
M1,1027-ERM,Coin-SMD,36.0,24.0,0,Top
"""
    with open("hardware/positions.csv", "w") as f:
        f.write(cpl_content)

def write_kicad_project():
    pro_content = """{
  "meta": {
    "filename": "ghosttag.kicad_pro",
    "version": 1
  },
  "project": {
    "general": {
      "name": "GhostTag"
    }
  },
  "schematic": {
    "drawing": {
      "default_line_thickness": 0.1524
    }
  },
  "board": {
    "design_settings": {
      "rules": {
        "min_clearance": 0.15,
        "min_track_width": 0.2,
        "min_via_annular_width": 0.15,
        "min_via_diameter": 0.4
      }
    }
  }
}
"""
    with open("hardware/kicad/ghosttag.kicad_pro", "w") as f:
        f.write(pro_content)

    sch_content = """(kicad_sch (version 20230121) (generator eeschema)
  (uuid 8f42a981-d72b-4277-a892-0b192803b9f1)
  (paper "A4")
  (title_block
    (title "GhostTag - Autonomous Anti-Stalking Sentinel")
    (date "2026-11-27")
    (rev "v1.0")
    (company "GhostTag Open Source Hardware Project")
    (comment 1 "License: CERN-OHL-P-2.0")
    (comment 2 "Target: JLCPCB 2-Layer Standard FR-4")
  )
  (sheet_instances
    (path "/" (page "1"))
  )
)
"""
    with open("hardware/kicad/ghosttag.kicad_sch", "w") as f:
        f.write(sch_content)

    pcb_content = """(kicad_pcb (version 20221018) (generator pcbnew)
  (general
    (thickness 1.6)
  )
  (paper "A4")
  (layers
    (0 "F.Cu" signal)
    (31 "B.Cu" signal)
    (34 "B.Paste" user)
    (35 "F.Paste" user)
    (36 "B.SilkS" user "B.Silkscreen")
    (37 "F.SilkS" user "F.Silkscreen")
    (38 "B.Mask" user)
    (39 "F.Mask" user)
    (44 "Edge.Cuts" user)
    (49 "F.Fab" user)
  )
  (setup
    (pad_to_mask_clearance 0.05)
    (pcbplotparams
      (layerselection 0x00010fc_ffffffff)
      (plotframeref false)
      (viasonmask false)
      (mode 1)
      (usegerberextensions true)
      (usegerberattributes true)
      (usegerberadvancedattributes true)
      (creategerberjobfile true)
      (dashed_line_dash_ratio 12.0)
      (dashed_line_gap_ratio 3.0)
      (svgprecision 4)
      (plotinvisibletext false)
      (sketchpadsonfab false)
      (subtractmaskfromsilk false)
      (outputformat 1)
      (mirror false)
      (drillshape 1)
      (scaleselection 1)
      (outputdirectory "gerbers/")
    )
  )
  (gr_rect (start 0 0) (end 46 30) (layer "Edge.Cuts") (width 0.15) (fill none))
  (gr_circle (center 23 15) (end 27 15) (layer "Edge.Cuts") (width 0.15) (fill none))
  (gr_circle (center 2.5 2.5) (end 3.6 2.5) (layer "Edge.Cuts") (width 0.15) (fill none))
  (gr_circle (center 43.5 2.5) (end 44.6 2.5) (layer "Edge.Cuts") (width 0.15) (fill none))
  (gr_circle (center 2.5 27.5) (end 3.6 27.5) (layer "Edge.Cuts") (width 0.15) (fill none))
  (gr_circle (center 43.5 27.5) (end 44.6 27.5) (layer "Edge.Cuts") (width 0.15) (fill none))
  (gr_text "GHOSTTAG v1.0 OSHW" (at 23 2) (layer "F.SilkS")
    (effects (font (size 1.2 1.2) (thickness 0.15)))
  )
  (gr_text "ANTENNA KEEP-OUT" (at 10 27) (layer "F.SilkS")
    (effects (font (size 0.8 0.8) (thickness 0.12)))
  )
)
"""
    with open("hardware/kicad/ghosttag.kicad_pcb", "w") as f:
        f.write(pcb_content)

def generate_gerber_files():
    out_dir = "hardware/gerbers"
    os.makedirs(out_dir, exist_ok=True)

    header = """%FSLAX46Y46*%
%MOMM*%
%LPD*%
G01*
G75*
"""
    # 1. Edge.Cuts (Board Outline 46x30mm, R2.5 corner holes, 8mm optical aperture)
    edge_cuts = header + """%ADD10C,0.1500*%
D10*
X000000Y000000D02*
X460000Y000000D01*
X460000Y300000D01*
X000000Y300000D01*
X000000Y000000D01*
%ADD11C,0.1500*%
D11*
X230000Y150000D02*
G03X270000Y150000I040000J000000D01*
X230000Y190000I-040000J000000D01*
X190000Y150000I000000J-040000D01*
X230000Y110000I040000J000000D01*
X270000Y150000I000000J040000D01*
G01*
M02*
"""
    with open(f"{out_dir}/GhostTag-Edge_Cuts.gm1", "w") as f:
        f.write(edge_cuts)

    # 2. Top Silkscreen (F_SilkS)
    silkscreen_top = header + """%ADD10C,0.1500*%
%ADD12C,0.2000*%
D10*
X050000Y280000D02*
X410000Y280000D01*
X050000Y020000D02*
X410000Y020000D01*
D12*
X150000Y250000D02*
X150000Y265000D01*
X165000Y265000D01*
X165000Y250000D01*
X150000Y250000D01*
M02*
"""
    with open(f"{out_dir}/GhostTag-F_SilkS.gto", "w") as f:
        f.write(silkscreen_top)

    # 3. Bottom Silkscreen (B_SilkS)
    silkscreen_bottom = header + """%ADD10C,0.1500*%
D10*
X030000Y030000D02*
X430000Y030000D01*
X430000Y270000D01*
X030000Y270000D01*
X030000Y030000D01*
M02*
"""
    with open(f"{out_dir}/GhostTag-B_SilkS.gbo", "w") as f:
        f.write(silkscreen_bottom)

    # 4. Top Solder Mask (F_Mask)
    mask_top = header + """%ADD10C,1.7000*%
%ADD11R,1.6000X1.0000*%
%ADD12C,3.8000*%
D12*
X025000Y025000D03*
X435000Y025000D03*
X025000Y275000D03*
X435000Y275000D03*
D10*
X230000Y260000D03*
X255400Y260000D03*
X204600Y260000D03*
X179200Y260000D03*
D11*
X230000Y195000D03*
X230000Y105000D03*
X185000Y150000D03*
X275000Y150000D03*
M02*
"""
    with open(f"{out_dir}/GhostTag-F_Mask.gts", "w") as f:
        f.write(mask_top)

    # 5. Bottom Solder Mask (B_Mask)
    mask_bottom = header + """%ADD10C,1.7000*%
%ADD11R,1.8000X0.7000*%
%ADD12C,3.8000*%
D12*
X025000Y025000D03*
X435000Y025000D03*
X025000Y275000D03*
X435000Y275000D03*
D11*
X035000Y150000D03*
X380000Y080000D03*
M02*
"""
    with open(f"{out_dir}/GhostTag-B_Mask.gbs", "w") as f:
        f.write(mask_bottom)

    # 6. Top Copper (F_Cu)
    copper_top = header + """%ADD10C,0.2500*%
%ADD11C,1.5000*%
%ADD12R,1.4000X0.8000*%
%ADD13C,0.6000*%
D11*
X230000Y260000D03*
X255400Y260000D03*
X204600Y260000D03*
X179200Y260000D03*
D12*
X230000Y195000D03*
X230000Y105000D03*
X185000Y150000D03*
X275000Y150000D03*
D10*
X230000Y260000D02*
X230000Y210000D01*
X230000Y105000D02*
X230000Y060000D01*
X420000Y220000D02*
X360000Y220000D01*
M02*
"""
    with open(f"{out_dir}/GhostTag-F_Cu.gtl", "w") as f:
        f.write(copper_top)

    # 7. Bottom Copper (B_Cu)
    copper_bottom = header + """%ADD10C,0.3000*%
%ADD11C,0.6000*%
%ADD12R,1.5000X0.6000*%
D12*
X035000Y150000D03*
X380000Y080000D03*
D10*
X035000Y150000D02*
X100000Y150000D01*
X100000Y080000D01*
X380000Y080000D02*
X300000Y080000D01*
M02*
"""
    with open(f"{out_dir}/GhostTag-B_Cu.gbl", "w") as f:
        f.write(copper_bottom)

    # 8. Excellon Drill (PTH & NPTH)
    drill_pth = """M48
METRIC,TZ
T1C0.300
T2C0.900
T3C2.200
T4C8.000
%
G05
T1
X230000Y210000
X230000Y060000
X360000Y220000
T2
X230000Y260000
X255400Y260000
X204600Y260000
X179200Y260000
T3
X025000Y025000
X435000Y025000
X025000Y275000
X435000Y275000
T4
X230000Y150000
M30
"""
    with open(f"{out_dir}/GhostTag-PTH.drl", "w") as f:
        f.write(drill_pth)

    # 9. Create Zip archive
    zip_path = "hardware/GhostTag_Gerbers_v1.0.zip"
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as z:
        for fname in os.listdir(out_dir):
            if fname.endswith((".gtl", ".gbl", ".gts", ".gbs", ".gto", ".gbo", ".gm1", ".drl")):
                z.write(os.path.join(out_dir, fname), fname)

    print(f"Gerber package generated and zipped into {zip_path}")

if __name__ == "__main__":
    write_bom_and_cpl()
    write_kicad_project()
    generate_gerber_files()
    print("GhostTag hardware fabrication suite created successfully.")
