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

  if (strip % 2 == 1) {
    led = (LEDS_PER_STRIP - 1) - led;
  }

  return strip * LEDS_PER_STRIP + led;
}

// =====================================================
// Motion
// =====================================================
int shift = 0;
int direction = 1;

unsigned long lastMove = 0;
unsigned long lastMode = 0;

int mode = 0; // 0 = CW, 1 = CCW

// =====================================================
// Thunder colors
// =====================================================
CRGB cwColors[4] = {
  CRGB::Blue,
  CRGB(255, 180, 20), // Gold
  CRGB::Red,
  CRGB::Yellow
};

CRGB ccwColors[4] = {
  CRGB::Green,
  CRGB::White,
  CRGB::Purple,
  CRGB::Orange
};

// =====================================================
// Setup
// =====================================================
void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  fadeToBlackBy(leds, NUM_LEDS, 90);

  // =================================================
  // Mode switch every 5 seconds
  // =================================================
  if (millis() - lastMode > 5000) {

    lastMode = millis();

    mode = 1 - mode; // toggle

    direction = (mode == 0) ? 1 : -1;
  }

  // =================================================
  // Movement speed
  // =================================================
  if (millis() - lastMove > 110) {

    lastMove = millis();
    shift += direction;
  }

  // =================================================
  // 4 THUNDER STREAMS
  // =================================================
  for (int wave = 0; wave < 4; wave++) {

    int startStrip = wave * 3; // 0,3,6,9

    CRGB color = (mode == 0) ? cwColors[wave] : ccwColors[wave];

    for (int i = 0; i < LEDS_PER_STRIP; i++) {

      int strip = (startStrip + (i * direction) + shift) % NUM_STRIPS;
      if (strip < 0) strip += NUM_STRIPS;

      int led = i;

      leds[getIndex(strip, led)] += color;
    }
  }

  FastLED.show();
}