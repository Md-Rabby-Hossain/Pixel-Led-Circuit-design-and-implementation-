#include <FastLED.h>

#define DATA_PIN 10
#define NUM_STRIPS 12
#define LEDS_PER_STRIP 8
#define NUM_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

CRGB leds[NUM_LEDS];

#define SPEED 90

uint8_t mode = 0;
uint8_t posOdd = 0;
uint8_t posEven = 0;
unsigned long lastChange = 0;

// ----------------------------------------------------
// ZIG-ZAG CORRECT MAPPING (IMPORTANT)
// ----------------------------------------------------
int getIndex(int strip, int r) {
  if (strip % 2 == 0) {
    return strip * LEDS_PER_STRIP + r;   // normal direction
  } else {
    return strip * LEDS_PER_STRIP + (LEDS_PER_STRIP - 1 - r); // reversed
  }
}

// ----------------------------------------------------
void fadeAll() {
  fadeToBlackBy(leds, NUM_LEDS, 150);
}

// ----------------------------------------------------
// COLOR MODES (every 5 seconds)
// ----------------------------------------------------
CRGB getOddColor(uint8_t m) {
  switch (m) {
    case 0: return CRGB::White;
    case 1: return CRGB::White;
    case 2: return CRGB(255, 215, 0); // Gold
    case 3: return CRGB::Red;
  }
  return CRGB::White;
}

CRGB getEvenColor(uint8_t m) {
  switch (m) {
    case 0: return CRGB::Blue;
    case 1: return CRGB(255, 105, 180); // Pink
    case 2: return CRGB::Blue;
    case 3: return CRGB::Green;
  }
  return CRGB::Blue;
}

// ----------------------------------------------------
// TRAIN DRAW FUNCTION (4 LED length)
// ----------------------------------------------------
void drawTrain(int strip, int pos, CRGB color, uint8_t brightness) {
  for (int i = 0; i < 4; i++) {
    int r = pos - i;

    if (r >= 0 && r < LEDS_PER_STRIP) {
      CRGB c = color;
      c.nscale8_video(brightness);
      leds[getIndex(strip, r)] = c;   //  FIXED FOR ZIGZAG
    }
  }
}

// ----------------------------------------------------
// SETUP
// ----------------------------------------------------
void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

// ----------------------------------------------------
// LOOP
// ----------------------------------------------------
void loop() {

  // change mode every 5 seconds
  if (millis() - lastChange > 5000) {
    mode++;
    if (mode > 3) mode = 0;

    posOdd = 0;
    posEven = 0;

    FastLED.clear();
    lastChange = millis();
  }

  fadeAll();

  CRGB oddColor = getOddColor(mode);
  CRGB evenColor = getEvenColor(mode);

  // --------------------------------------------------
  //  ODD STRIPS (FULL BRIGHT TRAIN)
  // --------------------------------------------------
  for (int s = 0; s < NUM_STRIPS; s += 2) {
    drawTrain(s, posOdd, oddColor, 255);
  }

  // --------------------------------------------------
  // EVEN STRIPS (DIM + DELAYED START)
  // --------------------------------------------------
  for (int s = 1; s < NUM_STRIPS; s += 2) {

    // delay effect so even starts later
    if (posOdd > 2) {
      drawTrain(s, posEven, evenColor, 120);
    }
  }

  // --------------------------------------------------
  // MOTION CONTROL
  // --------------------------------------------------
  posOdd++;

  if (posOdd >= LEDS_PER_STRIP + 4) {
    posOdd = 0;
    posEven = 0;
  }

  if (posOdd > 2) {
    posEven++;
    if (posEven >= LEDS_PER_STRIP + 4) {
      posEven = 0;
    }
  }

  FastLED.show();
  delay(SPEED);
}