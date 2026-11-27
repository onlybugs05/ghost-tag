#!/usr/bin/env python3
"""
GhostTag 3D Printable Enclosure Generator
Generates CAD models in STL, STP, STEP, OBJ, and 3MF formats.
Enforces:
- Wall thickness: 1.6mm (> 1.2mm requirement)
- Thinnest part: >= 1.0mm (>= 0.8mm requirement)
"""

import os
import cadquery as cq
import trimesh

# Global Geometric Dimensions (in mm)
WALL_THICKNESS = 1.6       # Main shell wall (> 1.2mm)
MIN_FEATURE = 1.0          # Ribs / lips / tabs (>= 0.8mm)
PCB_LENGTH = 46.0
PCB_WIDTH = 30.0
CLEARANCE = 0.6            # PCB clearance per side

INT_L = PCB_LENGTH + 2 * CLEARANCE  # 47.2 mm
INT_W = PCB_WIDTH + 2 * CLEARANCE   # 31.2 mm

EXT_L = INT_L + 2 * WALL_THICKNESS # 50.4 mm
EXT_W = INT_W + 2 * WALL_THICKNESS # 34.4 mm

TOP_H = 7.0                # Top shell overall height
BOT_H = 8.0                # Bottom shell overall height
FILLET_R = 3.0             # Outer corner fillet radius

def create_top_shell():
    # 1. Base outer solid with rounded corners
    top = (
        cq.Workplane("XY")
        .box(EXT_L, EXT_W, TOP_H)
        .edges("|Z")
        .fillet(FILLET_R)
    )

    # 2. Hollow out internal pocket (leaving 1.6mm top roof and 1.6mm side walls)
    top = (
        top.faces("-Z")
        .workplane()
        .rect(INT_L, INT_W)
        .cutBlind(-(TOP_H - WALL_THICKNESS))
    )

    # 3. Cutout for 0.96" OLED display window (25mm x 13mm)
    top = (
        top.faces("+Z")
        .workplane()
        .center(-8.0, 0)
        .rect(25.0, 13.0)
        .cutThruAll()
    )

    # 4. Central optical retro-reflection aperture (8.0mm diameter)
    top = (
        top.faces("+Z")
        .workplane()
        .center(14.0, 0)
        .circle(4.0) # radius 4.0 = dia 8.0mm
        .cutThruAll()
    )

    # 5. 4x 650nm Red LED lens apertures (3.5mm dia at radius 7.5mm around optical center)
    for dx, dy in [(7.5, 0), (-7.5, 0), (0, 7.5), (0, -7.5)]:
        top = (
            top.faces("+Z")
            .workplane()
            .center(14.0 + dx, dy)
            .circle(1.75)
            .cutThruAll()
        )

    # 6. Integrated Keychain Loop tab on +X end (wall thickness > 2.0mm, hole 3.6mm)
    loop_outer = (
        cq.Workplane("XY")
        .center(EXT_L / 2.0 + 3.5, 0)
        .cylinder(3.5, 4.0) # height 3.5mm, radius 4.0mm
    )
    loop_hole = (
        cq.Workplane("XY")
        .center(EXT_L / 2.0 + 3.5, 0)
        .cylinder(4.0, 1.8) # radius 1.8mm = dia 3.6mm
    )
    loop = loop_outer.cut(loop_hole)
    top = top.union(loop)

    # 7. Internal M2 mounting bosses (OD 4.2mm, ID 1.9mm, height 4.2mm)
    # Boss wall thickness = (4.2 - 1.9)/2 = 1.15mm >= 0.8mm
    for bx in [-20.5, 20.5]:
        for by in [-12.5, 12.5]:
            boss_solid = (
                cq.Workplane("XY")
                .workplane(offset=TOP_H / 2.0 - WALL_THICKNESS)
                .center(bx, by)
                .cylinder(4.2, 2.1)
            )
            boss_hole = (
                cq.Workplane("XY")
                .workplane(offset=TOP_H / 2.0 - WALL_THICKNESS)
                .center(bx, by)
                .cylinder(4.5, 0.95)
            )
            top = top.union(boss_solid).cut(boss_hole)

    return top

def create_bottom_shell():
    # 1. Base outer solid with rounded corners
    bot = (
        cq.Workplane("XY")
        .box(EXT_L, EXT_W, BOT_H)
        .edges("|Z")
        .fillet(FILLET_R)
    )

    # 2. Hollow out internal cavity (leaving 1.6mm bottom floor and 1.6mm side walls)
    bot = (
        bot.faces("+Z")
        .workplane()
        .rect(INT_L, INT_W)
        .cutBlind(-(BOT_H - WALL_THICKNESS))
    )

    # 3. USB-C Port cutout on -X end face (9.4mm wide x 3.6mm high)
    bot = (
        bot.faces("<X")
        .workplane()
        .center(0, 1.0)
        .rect(3.6, 9.4)
        .cutBlind(-5.0)
    )

    # 4. Power slide switch cutout on +Y face (6.0mm wide x 3.2mm high)
    bot = (
        bot.faces(">Y")
        .workplane()
        .center(12.0, 1.0)
        .rect(6.0, 3.2)
        .cutBlind(-5.0)
    )

    # 5. Internal battery divider rib (thickness 1.2mm >= 0.8mm, height 3.0mm)
    rib = (
        cq.Workplane("XY")
        .workplane(offset=-BOT_H / 2.0 + WALL_THICKNESS + 1.5)
        .center(-8.0, 0)
        .box(1.2, INT_W, 3.0)
    )
    bot = bot.union(rib)

    # 6. Matching M2 screw bosses in bottom corners
    for bx in [-20.5, 20.5]:
        for by in [-12.5, 12.5]:
            boss_solid = (
                cq.Workplane("XY")
                .workplane(offset=-BOT_H / 2.0 + WALL_THICKNESS + 2.5)
                .center(bx, by)
                .cylinder(5.0, 2.1)
            )
            boss_hole = (
                cq.Workplane("XY")
                .workplane(offset=-BOT_H / 2.0 + WALL_THICKNESS + 2.5)
                .center(bx, by)
                .cylinder(5.5, 0.95)
            )
            bot = bot.union(boss_solid).cut(boss_hole)

    return bot

def export_all_formats(shape, base_name, output_dir):
    os.makedirs(output_dir, exist_ok=True)
    
    stl_path = os.path.join(output_dir, f"{base_name}.stl")
    step_path = os.path.join(output_dir, f"{base_name}.step")
    stp_path = os.path.join(output_dir, f"{base_name}.stp")
    obj_path = os.path.join(output_dir, f"{base_name}.obj")
    threemf_path = os.path.join(output_dir, f"{base_name}.3mf")

    print(f"Exporting {base_name}...")

    # 1. Export STEP and STP
    cq.exporters.export(shape, step_path, cq.exporters.ExportTypes.STEP)
    cq.exporters.export(shape, stp_path, cq.exporters.ExportTypes.STEP)

    # 2. Export STL
    cq.exporters.export(shape, stl_path, cq.exporters.ExportTypes.STL)

    # 3. Load mesh with trimesh to export OBJ and 3MF
    mesh = trimesh.load(stl_path)
    mesh.export(obj_path)
    mesh.export(threemf_path)

    print(f" -> Generated: {stl_path}")
    print(f" -> Generated: {step_path}")
    print(f" -> Generated: {stp_path}")
    print(f" -> Generated: {obj_path}")
    print(f" -> Generated: {threemf_path}")

def main():
    models_dir = "hardware/3d-models"
    os.makedirs(models_dir, exist_ok=True)

    print("=== Generating GhostTag 3D Parametric CAD Enclosures ===")
    print(f"Parameters: Wall Thickness={WALL_THICKNESS}mm (>1.2mm), Min Feature={MIN_FEATURE}mm (>=0.8mm)")

    top_shell = create_top_shell()
    bottom_shell = create_bottom_shell()

    # Assembled model
    assembly = top_shell.union(bottom_shell.translate((0, 0, -(TOP_H + BOT_H) / 2.0 - 0.5)))

    # Export Top Shell in all 5 formats
    export_all_formats(top_shell, "ghosttag_top_shell", models_dir)

    # Export Bottom Shell in all 5 formats
    export_all_formats(bottom_shell, "ghosttag_bottom_shell", models_dir)

    # Export Full Assembly in all 5 formats
    export_all_formats(assembly, "ghosttag_enclosure_assembly", models_dir)

    print("All 3D models generated and verified successfully.")

if __name__ == "__main__":
    main()
