#include <FastLED.h>

#define DATA_PIN 10
#define NUM_STRIPS 12
#define LEDS_PER_STRIP 8
#define NUM_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

CRGB leds[NUM_LEDS];

// ---------- Zig-Zag Mapping ----------
int getIndex(int angle, int radius) {

  if (angle % 2 == 0) {
    return angle * LEDS_PER_STRIP + radius;
  }
  else {
    return angle * LEDS_PER_STRIP + (LEDS_PER_STRIP - 1 - radius);
  }
}

uint8_t angle = 0;

unsigned long startTime;

void setup() {

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.clear();
  FastLED.show();

  startTime = millis();
}

void loop() {

  FastLED.clear();

  // ---------- BLUE HEAD ----------
  for(int i = 0; i < LEDS_PER_STRIP; i++) {

    int strip = angle - i;

    while(strip < 0)
      strip += NUM_STRIPS;

    strip %= NUM_STRIPS;

    leds[getIndex(strip, i)] = CRGB::Blue;
  }

  // ---------- GOLD TRAIL AFTER 5s ----------
  if(millis() - startTime > 5000) {

    for(int i = 0; i < LEDS_PER_STRIP; i++) {

      int strip = angle - i - 1;

      while(strip < 0)
        strip += NUM_STRIPS;

      strip %= NUM_STRIPS;

      leds[getIndex(strip, i)] = CRGB(255, 180, 0); // gold
    }
  }

  // ---------- RED TRAIL AFTER 15s ----------
  if(millis() - startTime > 15000) {

    for(int i = 0; i < LEDS_PER_STRIP; i++) {

      int strip = angle - i - 2;

      while(strip < 0)
        strip += NUM_STRIPS;

      strip %= NUM_STRIPS;

      leds[getIndex(strip, i)] = CRGB::Red;
    }
  }

  FastLED.show();

  delay(160);

  angle++;

  if(angle >= NUM_STRIPS)
    angle = 0;
}