#include <FastLED.h>

#define DATA_PIN 10

#define NUM_STRIPS 12
#define LEDS_PER_STRIP 8
#define NUM_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

CRGB leds[NUM_LEDS];

// =====================================================
// Zig-zag mapping
// =====================================================
int getIndex(int strip, int led)
{
  if (strip % 2 == 1)
    led = (LEDS_PER_STRIP - 1) - led;

  return strip * LEDS_PER_STRIP + led;
}

// =====================================================
// Motion
// =====================================================
int pos = 0;

unsigned long lastMove = 0;

// =====================================================
// Setup
// =====================================================
void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.clear();
  FastLED.show();
}

// =====================================================
// Loop
// =====================================================
void loop()
{
  // VERY FAST fade
  fadeToBlackBy(leds, NUM_LEDS, 120);

  // =================================================
  // Draw pulse on all strips
  // =================================================
  for (int strip = 0; strip < NUM_STRIPS; strip++)
  {
    // head
    if (pos < LEDS_PER_STRIP)
      leds[getIndex(strip, pos)] += CRGB::Blue;

    // brighter core
    if (pos + 1 < LEDS_PER_STRIP)
      leds[getIndex(strip, pos + 1)] += CRGB(100,100,255);
  }

  FastLED.show();

  // =================================================
  // Motion
  // =================================================
  if (millis() - lastMove > 70)
  {
    lastMove = millis();

    pos++;

    // allow pulse to leave naturally
    if (pos >= LEDS_PER_STRIP + 2)
    {
      pos = 0;
    }
  }
}