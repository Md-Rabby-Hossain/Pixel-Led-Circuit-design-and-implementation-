#include <FastLED.h>

#define DATA_PIN 10

#define NUM_STRIPS 12
#define LEDS_PER_STRIP 8
#define NUM_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

CRGB leds[NUM_LEDS];

// =====================================================
// Zig-Zag Mapping
// =====================================================
int getIndex(int strip, int radius) {

  if (strip % 2 == 0) {
    return strip * LEDS_PER_STRIP + radius;
  }
  else {
    return strip * LEDS_PER_STRIP +
           (LEDS_PER_STRIP - 1 - radius);
  }
}

// =====================================================
// Motion
// =====================================================
uint8_t angle = 0;

unsigned long lastMove = 0;
unsigned long cycleStart = 0;

// =====================================================
// Setup
// =====================================================
void setup() {

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.clear();
  FastLED.show();

  cycleStart = millis();
}

// =====================================================
// Loop
// =====================================================
void loop() {

  FastLED.clear();

  // ===================================================
  // 20-second repeating cycle
  // ===================================================
  unsigned long elapsed = millis() - cycleStart;

  if (elapsed >= 20000) {

    cycleStart = millis();
    elapsed = 0;
  }

  // ===================================================
  // GOLD + BLUE
  // 0–10 seconds
  // ===================================================

  for (int i = 0; i < LEDS_PER_STRIP; i++) {

    int strip = angle - i - 0;

    while (strip < 0)
      strip += NUM_STRIPS;

    strip %= NUM_STRIPS;

    leds[getIndex(strip, i)] = CRGB(255, 180, 0);  // GOLD
  }


  // ===================================================
  // BLUE — one strip behind GOLD
  // ===================================================

  for (int i = 0; i < LEDS_PER_STRIP; i++) {

    int strip = angle - i - 1;

    while (strip < 0)
      strip += NUM_STRIPS;

    strip %= NUM_STRIPS;

    leds[getIndex(strip, i)] = CRGB::Blue;
  }


  // ===================================================
  // RED
  // Added after 10 seconds
  // ===================================================

  if (elapsed >= 10000) {

    for (int i = 0; i < LEDS_PER_STRIP; i++) {

      int strip = angle - i - 2;

      while (strip < 0)
        strip += NUM_STRIPS;

      strip %= NUM_STRIPS;

      leds[getIndex(strip, i)] = CRGB::Red;
    }
  }


  // ===================================================
  // Show
  // ===================================================

  FastLED.show();


  // ===================================================
  // Faster rotation
  // ===================================================

  if (millis() - lastMove >= 120) {

    lastMove = millis();

    angle++;

    if (angle >= NUM_STRIPS)
      angle = 0;
  }
}