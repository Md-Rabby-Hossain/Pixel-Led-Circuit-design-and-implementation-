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
uint8_t slowPos = 0;
uint8_t fastPos = 0;

unsigned long lastSlow = 0;
unsigned long lastFast = 0;
unsigned long lastTheme = 0;

uint8_t theme = 0;

// =====================================================
// Themes (4 combinations)
// =====================================================
struct Theme {
  CRGB slowColor;
  CRGB fastColor;
};

Theme themes[] = {

  {CRGB(255, 180, 20), CRGB::HotPink},  // GOLD + PINK
  {CRGB::Green, CRGB::Blue},            // GREEN + BLUE
  {CRGB::Yellow, CRGB::Purple},         // YELLOW + PURPLE
  {CRGB(255, 180, 20), CRGB::Aqua}      // GOLD + AQUA
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
// Loop
// =====================================================
void loop() {

  fadeToBlackBy(leds, NUM_LEDS, 80);

  // =================================================
  // Theme switch every 5 seconds
  // =================================================
  if (millis() - lastTheme > 5000) {
    lastTheme = millis();
    theme = (theme + 1) % THEME_COUNT;
  }

  CRGB slowColor = themes[theme].slowColor;
  CRGB fastColor = themes[theme].fastColor;

  // =================================================
  // SPEED CONTROL
  // =================================================
  if (millis() - lastSlow > 140) {
    lastSlow = millis();
    slowPos++;
  }

  if (millis() - lastFast > 80) {
    lastFast = millis();
    fastPos++;
  }

  // =================================================
  // SLOW GROUP (0,3,6,9)
  // =================================================
  for (int strip = 0; strip < NUM_STRIPS; strip++) {

    if (strip == 0 || strip == 3 || strip == 6 || strip == 9) {

      for (int i = 0; i < 3; i++) {

        int led = (slowPos + i) % LEDS_PER_STRIP;

        leds[getIndex(strip, led)] += slowColor;
      }
    }
  }

  // =================================================
  // FAST GROUP (others)
  // =================================================
  for (int strip = 0; strip < NUM_STRIPS; strip++) {

    if (!(strip == 0 || strip == 3 || strip == 6 || strip == 9)) {

      for (int i = 0; i < 4; i++) {

        int led = (fastPos + i) % LEDS_PER_STRIP;

        leds[getIndex(strip, led)] += fastColor;
      }
    }
  }

  FastLED.show();
}