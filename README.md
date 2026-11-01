# 4x4x4 RGB LED Cube

A 4x4x4 multiplexed common-cathode RGB LED cube driven by an Arduino Nano, cascaded 74HC595 shift registers, and low-side switching MOSFETs. 

---

## Soldering Jig Design & Fabrication

Soldering a 3D LED matrix freehand almost always results in crooked pillars and uneven spacing. Before assembling the electronics, I designed and laser-cut an interlocking alignment jig to hold the LEDs rigidly at fixed intervals while soldering each layer plane.

<img width="2237" height="1208" alt="Jig Model" src="https://github.com/user-attachments/assets/3757588d-2eae-4618-9b58-01a73f7ffcba" />

### Dimensional Constraints & Design Choices

* **LED Pitch (25 mm):** I went with a 2.5 cm center-to-center spacing between LEDs. This leaves plenty of clearance for soldering iron tips and needle-nose pliers while keeping internal animations clearly visible from outside angles.
* **Lead Proximity to Edge:** The LED leads are soldered directly along the wooden edge of the comb. Keeping the distance from the LED cutout to the outer edge minimal forces the vertical buses to run close together, which gives the completed cube clean, parallel lines.
* **Hole Tolerance (0.49 cm for 0.5 cm LEDs):** Standard 5 mm diffused RGB LEDs measure ~0.50 cm. Because laser cutters burn away material (kerf), drawing exact 0.50 cm circles would leave the holes loose. I modeled the cutouts at **0.49 cm** instead. This resulted in a light press-fit where the LEDs snap in securely; for any holes that cut slightly tight, a quick twist of rolled sandpaper opened them up to a snug fit.
* **Slot Fit & Kerf Management:** The base slots were modeled slightly narrower than the nominal 0.40 cm plywood thickness. Sheet plywood always has manufacturing variance, so it is much safer to design the tenons tight and sand them down than to have sloppy joints. Sanding ~0.05 cm off each tab brought them to a precise 0.35 cm friction fit.

---

### CAD to Laser Workflow

I designed the fixture parametrically in **Autodesk Inventor**, but the laser cutting software required vector SVG geometry.

1. **Export:** Exported the 2D sketch faces from Inventor as `.dxf` files.
2. **Conversion:** Converted the DXF vectors into SVG format.
3. **Path Prep in Inkscape:**
   * Verified all physical dimensions 1:1 against the original CAD model.
   * Set the cut lines to pure red (`#FF0000`) with a stroke width of **0.025 mm** (hairline), which the laser driver interprets as vector cut lines rather than raster engraving.

<img width="1856" height="1363" alt="Inkscape Setup" src="https://github.com/user-attachments/assets/93f3f528-df11-4c88-86f7-bc1605bfd452" />

* **Cutter Settings:** Cut on a **Trotec Speedy 400 (120W CO2 Laser)** at **100% Power** and **0.3 Speed**. Because 0.4 cm plywood is relatively dense, slow feed rates ensured clean cut-throughs without excessive charring along the edges.

---

## Jig Assembly

<img width="4000" height="3000" alt="Laser cut pieces" src="https://github.com/user-attachments/assets/b2e49303-73e9-49c0-8bd4-2d776d1d70fc" />

The laser cuts produce the flat base plate and vertical alignment combs.

<img width="4000" height="3000" alt="Tenon thickness check" src="https://github.com/user-attachments/assets/47b0b40e-258c-4694-b3e3-9eea8302ce3b" />

Before gluing, block-sand the mating tabs down to ~0.35 cm until each comb slides firmly into its base slot without splitting the wood.

<img width="3000" height="4000" alt="Sanded comb tabs" src="https://github.com/user-attachments/assets/19efe971-fbe7-4182-8c5a-cf5f3b357aa4" />

Once the tabs seat smoothly, apply wood glue or cyanoacrylate (super glue) and clamp the uprights perpendicular to the base plate. Make sure the combs remain squared at 90° so the LED columns stay parallel across all four rows.

<img width="4000" height="3000" alt="Fully assembled jig" src="https://github.com/user-attachments/assets/da548024-6d75-418f-afa3-d1257ff03603" />

<img width="3000" height="4000" alt="Jig upright profile" src="https://github.com/user-attachments/assets/6acbe197-9f61-4fa5-b296-2ea704df4208" />

<img width="3000" height="1352" alt="Jig top down alignment" src="https://github.com/user-attachments/assets/1a147e04-70ab-4144-8a05-10fd5d0a199f" />

> **Design Note on the 5th Slot:**  
> The cube is 4x4x4, but the jig features 5 LED slots per comb. The 5th slot serves as an anchor point to clip and tension the horizontal bus wire while soldering the common cathode planes.
