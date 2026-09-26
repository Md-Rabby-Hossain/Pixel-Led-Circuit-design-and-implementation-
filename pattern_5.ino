#include <FastLED.h>

#define DATA_PIN 10

#define NUM_STRIPS 12
#define LEDS_PER_STRIP 8
#define NUM_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

CRGB leds[NUM_LEDS];

// =====================================================
// Zig-zag mapping
// =====================================================
int getIndex(int strip, int led) {

  // reverse odd strips
  if (strip % 2 == 1) {
    led = (LEDS_PER_STRIP - 1) - led;
  }

  return strip * LEDS_PER_STRIP + led;
}

// =====================================================
// Motion variables
// =====================================================
int pos = 0;
bool outward = true;

unsigned long lastModeChange = 0;
uint8_t mode = 0;

// =====================================================
// Colors
// =====================================================
CRGB GOLD = CRGB(255, 180, 20);

// =====================================================
// Setup
// =====================================================
void setup() {

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.clear();
  FastLED.show();
}

// =====================================================
// Draw pulse
// =====================================================
void drawPulse(CRGB oddColor, CRGB evenColor) {

  // fast smooth fading
  fadeToBlackBy(leds, NUM_LEDS, 120);

  for (int strip = 0; strip < NUM_STRIPS; strip++) {

    CRGB color;

    // odd-even strip coloring
    if (strip % 2 == 0)
      color = evenColor;
    else
      color = oddColor;

    // main LED
    leds[getIndex(strip, pos)] += color;

    // second LED beside it
    if (pos + 1 < LEDS_PER_STRIP) {
      leds[getIndex(strip, pos + 1)] += color;
    }
  }
}

// =====================================================
// Loop
// =====================================================
void loop() {

  // -------------------------------------------------
  // Change mode every 5 seconds
  // -------------------------------------------------
  if (millis() - lastModeChange > 5000) {

    mode++;
    if (mode > 8) mode = 0;

    lastModeChange = millis();
  }

  // -------------------------------------------------
  // Pattern Modes
  // -------------------------------------------------

  switch (mode) {

    // Blue
    case 0:
      drawPulse(CRGB::Blue, CRGB::Blue);
      break;

    // Golden
    case 1:
      drawPulse(GOLD, GOLD);
      break;

    // White
    case 2:
      drawPulse(CRGB::White, CRGB::White);
      break;

    // Red
    case 3:
      drawPulse(CRGB::Red, CRGB::Red);
      break;

    // Green
    case 4:
      drawPulse(CRGB::Green, CRGB::Green);
      break;

    // Yellow
    case 5:
      drawPulse(CRGB::Yellow, CRGB::Yellow);
      break;

    // Odd White + Even Blue
    case 6:
      drawPulse(CRGB::White, CRGB::Blue);
      break;

    // Odd Red + Even Green
    case 7:
      drawPulse(CRGB::Red, CRGB::Green);
      break;

    // Odd Golden + Even Purple
    case 8:
      drawPulse(GOLD, CRGB::Purple);
      break;
  }

  // -------------------------------------------------
  // Show LEDs
  // -------------------------------------------------
  FastLED.show();

  delay(120);

  // -------------------------------------------------
  // Radial movement
  // -------------------------------------------------
  if (outward) {

    pos++;

    if (pos >= LEDS_PER_STRIP - 2) {
      outward = false;
    }

  } else {

    pos--;

    if (pos <= 0) {
      outward = true;
    }
  }
}