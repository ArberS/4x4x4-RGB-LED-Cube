// ==========================================================
// 4x4x4 RGB LED CUBE - SINGLE-LED COORDINATE CALIBRATION TEST
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
}

void loop() {
  // Sweep each color channel (0 = Red, 1 = Green, 2 = Blue) across all 64 LEDs
  for (uint8_t color = 0; color < 3; color++) {
    bool r = (color == 0);
    bool g = (color == 1);
    bool b = (color == 2);

    for (uint8_t z = 0; z < 4; z++) {
      for (uint8_t y = 0; y < 4; y++) {
        for (uint8_t x = 0; x < 4; x++) {
          clearCube();
          setLED(x, y, z, r, g, b);
          renderFrameFast(60); // Hold each LED briefly for visual inspection
        }
      }
    }
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
// COORDINATE MAPPING
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