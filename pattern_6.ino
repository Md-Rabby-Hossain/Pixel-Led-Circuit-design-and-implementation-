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
uint8_t shiftA = 0;
uint8_t shiftB = 0;

unsigned long lastMove = 0;

// =====================================================
// Setup
// =====================================================
void setup() {

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.clear();
}

// =====================================================
// Main loop
// =====================================================
void loop() {

  // smooth fading trails
  fadeToBlackBy(leds, NUM_LEDS, 85);

  // slower movement
  if (millis() - lastMove > 180) {

    lastMove = millis();

    // BLUE system → forward
    shiftA = (shiftA + 1) % NUM_STRIPS;

    // GOLD system → reverse
    shiftB = (shiftB + NUM_STRIPS - 1) % NUM_STRIPS;
  }

  // =====================================================
  // BLUE SYSTEM (LED 0–3)
  // 2 WINDOWS
  // =====================================================
  for (int w = 0; w < 2; w++) {

    // 6-strip separation between windows
    int base = (shiftA + w * 6) % NUM_STRIPS;

    for (int i = 0; i < 4; i++) {

      int strip = (base + i) % NUM_STRIPS;
      int led   = i;

      leds[getIndex(strip, led)] += CRGB::Blue;
    }
  }

  // =====================================================
  // GOLD SYSTEM (LED 4–7)
  // 2 WINDOWS (opposite direction)
  // =====================================================
  for (int w = 0; w < 2; w++) {

    int base = (shiftB + w * 6) % NUM_STRIPS;

    for (int i = 0; i < 4; i++) {

      int strip = (base + i) % NUM_STRIPS;
      int led   = i + 4;

      leds[getIndex(strip, led)] += CRGB(255, 180, 20); // gold
    }
  }

  FastLED.show();
}