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
unsigned long lastMove = 0;

// =====================================================
// Theme control
// =====================================================
unsigned long lastTheme = 0;
int theme = 0;

// =====================================================
// 4 color themes (3 waves each)
// =====================================================
struct Theme {
  CRGB c1;
  CRGB c2;
  CRGB c3;
};

Theme themes[] = {

  {CRGB::Blue,   CRGB::Red,     CRGB::Green},
  {CRGB(255,180,20), CRGB::HotPink, CRGB::Aqua},
  {CRGB::Purple, CRGB::Yellow,  CRGB::Cyan},
  {CRGB::White,  CRGB::Orange,  CRGB::Magenta}
};

#define THEME_COUNT 4

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

  fadeToBlackBy(leds, NUM_LEDS, 85);

  // =================================================
  // change theme every 5 seconds
  // =================================================
  if (millis() - lastTheme > 5000) {

    lastTheme = millis();
    theme = (theme + 1) % THEME_COUNT;
  }

  // =================================================
  // slow movement
  // =================================================
  if (millis() - lastMove > 120) {

    lastMove = millis();
    shift++;
  }

  // =================================================
  // 3 diagonal waves
  // =================================================
  for (int wave = 0; wave < 3; wave++) {

    int startStrip = wave * 4;

    CRGB col;

    if (wave == 0) col = themes[theme].c1;
    else if (wave == 1) col = themes[theme].c2;
    else col = themes[theme].c3;

    for (int i = 0; i < 8; i++) {

      int strip = (startStrip + i + shift) % NUM_STRIPS;
      int led   = i;

      leds[getIndex(strip, led)] += col;
    }
  }

  FastLED.show();
}