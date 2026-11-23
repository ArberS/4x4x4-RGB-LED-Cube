// ==========================================================
// 4x4x4 RGB LED CUBE - COMPLETE SHOWCASE ENGINE (CALIBRATED)
// ==========================================================

// Hardware Pin Assignments
const int LATCH_PIN = 8;   // ST_CP (PORTB Bit 0)
const int CLOCK_PIN = 9;   // SH_CP (PORTB Bit 1)
const int DATA_PIN  = 12;  // DS    (PORTB Bit 4)

uint8_t cube[4][6]; // 4 Layers, 6 Shift Registers per layer

// Calibrated lookup tables mapping index -> physical bit (0..15)
const uint8_t blueMap[16] = {
  11, 10,  9,  8,   // y = 0 (x = 0, 1, 2, 3)
  15, 14, 13, 12,   // y = 1 (x = 0, 1, 2, 3)
   3,  2,  1,  0,   // y = 2 (x = 0, 1, 2, 3)
   7,  6,  5,  4    // y = 3 (x = 0, 1, 2, 3)
};

const uint8_t greenRedMap[16] = {
  11, 10,  9,  8,   // y = 0 (x = 0, 1, 2, 3)
  15, 14, 13, 12,   // y = 1 (x = 0, 1, 2, 3)
   3,  2,  1,  0,   // y = 2 (x = 0, 1, 2, 3)
   7,  6,  5,  4    // y = 3 (x = 0, 1, 2, 3)
};

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);

  randomSeed(analogRead(A0));
}

void loop() {
  // 1. Normal Axis Grid Sweep (10 Seconds)
  unsigned long start = millis();
  while (millis() - start < 10000) {
    runMatrixSweepStep();
  }

  // 2. 3D Diagonal Volume Sweep (10 Seconds)
  start = millis();
  while (millis() - start < 10000) {
    runDiagonalVolumeSweep();
  }

  // 3. Expanding & Shrinking Cube (12 Seconds)
  start = millis();
  while (millis() - start < 12000) {
    runExpandingCubeStep();
  }

  // 4. Solid Outer Cube Edges / Wireframe (9 Seconds)
  runOuterEdgesEffect(9000);

  // 5. 3D Rainstorm with Splashes (10 Seconds)
  start = millis();
  while (millis() - start < 10000) {
    effectRainstormStep();
  }

  // 6. Relaxed Multi-Color Showcase (12 Seconds)
  start = millis();
  while (millis() - start < 12000) {
    effectCrazySparks(5);
    if (millis() - start >= 12000) break;
    effectCandyCaneSpiral(1);
    if (millis() - start >= 12000) break;
    effectFlashingBoxes(2);
    if (millis() - start >= 12000) break;
    effectChaoticWaves(2);
  }
}

// ==========================================================
// ULTRA-FAST MULTIPLEX ENGINE (STRICT <= 4-PIN MASK)
// ==========================================================
inline void fastShiftOut(uint8_t val) {
  for (uint8_t i = 0; i < 8; i++)  {
    if (val & (1 << (7 - i))) {     
      PORTB |= (1 << PB4);          // Pin 12 (Data) HIGH
    } else {
      PORTB &= ~(1 << PB4);         // Pin 12 (Data) LOW
    }
    PORTB |= (1 << PB1);            // Pin 9 (Clock) HIGH
    PORTB &= ~(1 << PB1);           // Pin 9 (Clock) LOW
  }
}

void renderFrameFast(int passes) {
  for (int p = 0; p < passes; p++) {
    for (uint8_t layer = 0; layer < 4; layer++) {
      for (uint8_t phase = 0; phase < 2; phase++) {
        
        PORTB &= ~(1 << PB0); // Pin 8 (Latch) LOW

        // Phase mask guarantees <= 4 active output pins per 74HC595 register
        uint8_t maskRed   = (phase == 0) ? 0x0F : 0xF0;
        uint8_t maskGreen = (phase == 0) ? 0xF0 : 0x0F; 
        uint8_t maskBlue  = (phase == 0) ? 0xF0 : 0x0F; 

        // Shift out Red first so it lands in Reg 5 & 4
        fastShiftOut(cube[layer][1] & maskRed);
        fastShiftOut(cube[layer][0] & maskRed);

        // Shift out Green second so it lands in Reg 3 & 2
        fastShiftOut(cube[layer][3] & maskGreen);
        fastShiftOut(cube[layer][2] & maskGreen);

        // Shift out Blue last so it lands in Reg 1 & 0
        fastShiftOut(cube[layer][5] & maskBlue);
        fastShiftOut(cube[layer][4] & maskBlue);

        // Shift out the 7th register (layer sink)
        fastShiftOut(1 << layer);

        PORTB |= (1 << PB0); // Pin 8 (Latch) HIGH
        delayMicroseconds(100); 
      }
    }
  }
}

// ==========================================================
// GEOMETRY & DRAWING FUNCTIONS
// ==========================================================
void clearCube() {
  for (uint8_t l = 0; l < 4; l++) {
    for (uint8_t r = 0; r < 6; r++) {
      cube[l][r] = 0x00;
    }
  }
}

void setLED(uint8_t x, uint8_t y, uint8_t z, bool r, bool g, bool b) {
  if (x > 3 || y > 3 || z > 3) return;

  // Red & Green use inverted (x, y) to match PCB routing
  uint8_t grIdx = (3 - y) * 4 + (3 - x);

  // Blue uses non-inverted (x, y) so all 3 colors align at (0, 0, 0)
  uint8_t bIdx  = y * 4 + x;

  if (r) {
    uint8_t physBit = greenRedMap[grIdx];
    cube[z][0 + (physBit >> 3)] |= (1 << (physBit & 0x07));
  }
  if (g) {
    uint8_t physBit = greenRedMap[grIdx];
    cube[z][2 + (physBit >> 3)] |= (1 << (physBit & 0x07));
  }
  if (b) {
    uint8_t physBit = blueMap[bIdx];
    cube[z][4 + (physBit >> 3)] |= (1 << (physBit & 0x07));
  }
}

void drawWireframeBox(uint8_t x0, uint8_t y0, uint8_t z0,
                      uint8_t x1, uint8_t y1, uint8_t z1,
                      bool r, bool g, bool b) {
  for (uint8_t x = x0; x <= x1; x++) {
    for (uint8_t y = y0; y <= y1; y++) {
      for (uint8_t z = z0; z <= z1; z++) {
        uint8_t boundaryCount = (x == x0 || x == x1) + (y == y0 || y == y1) + (z == z0 || z == z1);
        if (boundaryCount >= 2) {
          setLED(x, y, z, r, g, b);
        }
      }
    }
  }
}

void setPlane(char axis, uint8_t index, bool r, bool g, bool b) {
  for (uint8_t a = 0; a < 4; a++) {
    for (uint8_t c = 0; c < 4; c++) {
      if (axis == 'Z') setLED(a, c, index, r, g, b);
      else if (axis == 'Y') setLED(a, index, c, r, g, b);
      else if (axis == 'X') setLED(index, a, c, r, g, b);
    }
  }
}

void drawOuterEdges(bool r, bool g, bool b) {
  clearCube();
  drawWireframeBox(0, 0, 0, 3, 3, 3, r, g, b);
}

// ==========================================================
// ANIMATION MODULES
// ==========================================================

// 1. Normal Axis Grid Sweep
void runMatrixSweepStep() {
  static int colorIdx = 0;
  static int axisIdx = 0;
  static int pos = 0;
  static int dir = 1;
  static unsigned long lastCall = 0;

  if (millis() - lastCall > 500) {
    colorIdx = 0;
    axisIdx = 0;
    pos = 0;
    dir = 1;
  }

  const bool colors[4][3] = {
    {true,  false, false}, // Red
    {false, true,  false}, // Green
    {false, false, true }, // Blue
    {true,  true,  true }  // White
  };
  const char axes[3] = {'Z', 'Y', 'X'};

  clearCube();
  setPlane(axes[axisIdx], pos, colors[colorIdx][0], colors[colorIdx][1], colors[colorIdx][2]);
  
  renderFrameFast(75);
  lastCall = millis();

  pos += dir;
  if (pos > 3) {
    pos = 2;
    dir = -1;
  } else if (pos < 0) {
    pos = 0;
    dir = 1;
    axisIdx++;
    if (axisIdx >= 3) {
      axisIdx = 0;
      colorIdx = (colorIdx + 1) % 4;
    }
  }
}

// 2. 3D Diagonal Volume Sweep
void runDiagonalVolumeSweep() {
  static int diagMode = 0;
  static int step = 0;
  static int dir = 1;
  static int colorIdx = 0;
  static unsigned long lastCall = 0;

  if (millis() - lastCall > 500) {
    diagMode = 0;
    step = 0;
    dir = 1;
    colorIdx = 0;
  }

  const bool colors[4][3] = {
    {false, true,  true }, // Cyan
    {true,  false, false}, // Red
    {false, true,  false}, // Green
    {true,  true,  true }  // White
  };

  clearCube();

  for (uint8_t x = 0; x < 4; x++) {
    for (uint8_t y = 0; y < 4; y++) {
      for (uint8_t z = 0; z < 4; z++) {
        bool onPlane = false;

        if (diagMode == 0) {
          onPlane = ((y + (3 - z)) == step);
        } else if (diagMode == 1) {
          onPlane = ((y + z) == step);
        } else {
          onPlane = ((x + y) == step);
        }

        if (onPlane) {
          setLED(x, y, z, colors[colorIdx][0], colors[colorIdx][1], colors[colorIdx][2]);
        }
      }
    }
  }

  renderFrameFast(70);
  lastCall = millis();

  step += dir;
  if (step > 6) {
    step = 5;
    dir = -1;
  } else if (step < 0) {
    step = 0;
    dir = 1;
    diagMode = (diagMode + 1) % 3;
    if (diagMode == 0) {
      colorIdx = (colorIdx + 1) % 4;
    }
  }
}

// 3. Expanding & Shrinking Cube (Centered Pulse + Corner-to-Corner 1x1..4x4 Growth)
void runExpandingCubeStep() {
  static uint8_t mode = 0;
  static uint8_t subCycle = 0;
  static int sizeStep = 0;
  static int dir = 1;
  static uint8_t cornerIdx = 0;
  static uint8_t colorIdx = 0;
  static unsigned long lastCall = 0;

  if (millis() - lastCall > 500) {
    mode = 0;
    subCycle = 0;
    sizeStep = 0;
    dir = 1;
    cornerIdx = 0;
  }

  const bool colors[5][3] = {
    {false, true,  true }, // Cyan
    {true,  false, true }, // Magenta
    {true,  true,  false}, // Yellow
    {false, true,  false}, // Green
    {true,  true,  true }  // White
  };

  bool r = colors[colorIdx][0];
  bool g = colors[colorIdx][1];
  bool b = colors[colorIdx][2];

  clearCube();

  if (mode == 0) {
    if (sizeStep == 0) {
      drawWireframeBox(1, 1, 1, 2, 2, 2, r, g, b);
    } else {
      drawWireframeBox(0, 0, 0, 3, 3, 3, r, g, b);
    }

    renderFrameFast(140);
    lastCall = millis();

    sizeStep ^= 1;
    if (sizeStep == 0) {
      colorIdx = (colorIdx + 1) % 5;
      subCycle++;
      if (subCycle >= 4) {
        subCycle = 0;
        mode = 1;
        sizeStep = 0;
        dir = 1;
      }
    }
  } else {
    uint8_t anchorX = (cornerIdx & 1) ? 3 : 0;
    uint8_t anchorY = (cornerIdx & 2) ? 3 : 0;
    uint8_t anchorZ = (cornerIdx & 4) ? 3 : 0;

    uint8_t x0 = (anchorX == 0) ? 0 : (3 - sizeStep);
    uint8_t x1 = (anchorX == 0) ? sizeStep : 3;
    uint8_t y0 = (anchorY == 0) ? 0 : (3 - sizeStep);
    uint8_t y1 = (anchorY == 0) ? sizeStep : 3;
    uint8_t z0 = (anchorZ == 0) ? 0 : (3 - sizeStep);
    uint8_t z1 = (anchorZ == 0) ? sizeStep : 3;

    drawWireframeBox(x0, y0, z0, x1, y1, z1, r, g, b);

    renderFrameFast(85);
    lastCall = millis();

    sizeStep += dir;
    if (sizeStep > 3) {
      sizeStep = 2;
      dir = -1;
    } else if (sizeStep < 0) {
      sizeStep = 0;
      dir = 1;
      colorIdx = (colorIdx + 1) % 5;
      cornerIdx = (cornerIdx + 3) % 8;
      subCycle++;
      if (subCycle >= 3) {
        subCycle = 0;
        mode = 0;
      }
    }
  }
}

// 4. Solid Outer Cube Edges (Wireframe)
void runOuterEdgesEffect(unsigned long durationMs) {
  const bool palette[6][3] = {
    {true,  true,  true }, // White
    {false, true,  true }, // Cyan
    {true,  false, false}, // Red
    {false, true,  false}, // Green
    {false, false, true }, // Blue
    {true,  false, true }  // Magenta
  };

  unsigned long start = millis();
  unsigned long lastSwitch = 0;
  uint8_t colorIdx = 0;

  drawOuterEdges(palette[0][0], palette[0][1], palette[0][2]);
  lastSwitch = millis();

  while (millis() - start < durationMs) {
    renderFrameFast(1);

    if (millis() - lastSwitch >= 1500) {
      lastSwitch = millis();
      colorIdx = (colorIdx + 1) % 6;
      drawOuterEdges(palette[colorIdx][0], palette[colorIdx][1], palette[colorIdx][2]);
    }
  }
}

// 5. 3D Rainstorm with Splashes
void effectRainstormStep() {
  struct Raindrop { uint8_t x; uint8_t y; int8_t z; bool active; };
  static Raindrop drops[4] = {{0,0,-1,false}, {0,0,-1,false}, {0,0,-1,false}, {0,0,-1,false}};

  clearCube();

  if (random(0, 3) == 0) {
    for (int i = 0; i < 4; i++) {
      if (!drops[i].active) {
        drops[i].x = random(0, 4);
        drops[i].y = random(0, 4);
        drops[i].z = 3; 
        drops[i].active = true;
        break;
      }
    }
  }

  for (int i = 0; i < 4; i++) {
    if (drops[i].active) {
      if (drops[i].z >= 0) {
        setLED(drops[i].x, drops[i].y, drops[i].z, false, true, true);
      } else if (drops[i].z == -1) {
        setLED(drops[i].x, drops[i].y, 0, false, true, false);
      } else {
        drops[i].active = false;
      }
      drops[i].z--; 
    }
  }
  renderFrameFast(26);
}

// 6. Relaxed Multi-Color Effects (Slowed down ~3x from original)
void effectCrazySparks(int steps) {
  for (int step = 0; step < steps; step++) {
    clearCube();
    int numSparks = random(10, 15);
    for (int i = 0; i < numSparks; i++) {
      uint8_t x = random(0, 4);
      uint8_t y = random(0, 4);
      uint8_t z = random(0, 4);
      bool r = random(0, 2);
      bool g = random(0, 2);
      bool b = random(0, 2);
      if (!r && !g && !b) r = true;
      setLED(x, y, z, r, g, b);
    }
    renderFrameFast(85); // Slowed from 28
  }
}

void effectCandyCaneSpiral(int iterations) {
  const uint8_t orbit[12][2] = {
    {0,0}, {1,0}, {2,0}, {3,0},
    {3,1}, {3,2}, {3,3},
    {2,3}, {1,3}, {0,3},
    {0,2}, {0,1}
  };
  for (int it = 0; it < iterations; it++) {
    for (int step = 0; step < 12; step++) {
      clearCube();
      for (int z = 0; z < 4; z++) {
        uint8_t pos1 = (step + (z * 3)) % 12;
        uint8_t pos2 = (step + (z * 3) + 6) % 12;
        setLED(orbit[pos1][0], orbit[pos1][1], z, true, false, false);
        setLED(orbit[pos2][0], orbit[pos2][1], z, false, true, false);
        setLED(1, 1, z, true, true, false);
        setLED(2, 2, z, true, true, false);
      }
      renderFrameFast(75); // Slowed from 24
    }
  }
}

void effectFlashingBoxes(int cycles) {
  for (int c = 0; c < cycles; c++) {
    clearCube();
    bool rColor = (c % 2 == 0);
    for (int z = 1; z <= 2; z++)
      for (int x = 1; x <= 2; x++)
        for (int y = 1; y <= 2; y++)
          setLED(x, y, z, rColor, !rColor, true); 
    renderFrameFast(110); // Slowed from 35

    clearCube();
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        setLED(i, j, 0, true, true, false);
        setLED(i, j, 3, true, false, true);
        setLED(i, 0, j, false, true, true);
        setLED(i, 3, j, true, true, true);
      }
    }
    renderFrameFast(110); // Slowed from 35
  }
}

void effectChaoticWaves(int waves) {
  for (int w = 0; w < waves; w++) {
    for (int layer = 0; layer < 4; layer++) {
      clearCube();
      for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
          if ((x + y) % 2 == 0) setLED(x, y, layer, true, true, false);
          else                  setLED(x, y, layer, false, true, true);
        }
      }
      renderFrameFast(85); // Slowed from 28
    }
  }
}