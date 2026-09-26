#include <FastLED.h>

#define DATA_PIN 10
#define NUM_STRIPS 12
#define LEDS_PER_STRIP 8
#define NUM_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

CRGB leds[NUM_LEDS];

uint8_t radius = 0;

// ---------- Zig-zag mapping ----------
int getIndex(int angle, int r) {
  if (angle % 2 == 0) {
    return angle * LEDS_PER_STRIP + r;
  } else {
    return angle * LEDS_PER_STRIP + (LEDS_PER_STRIP - 1 - r);
  }
}

// ---------- Color pairs (4 sets = 20 sec total) ----------
CRGB colorA[] = {
  CRGB::Blue,
  CRGB::Red,
  CRGB::Yellow,
  CRGB::Purple
};

CRGB colorB[] = {
  CRGB::Gold,
  CRGB::Green,
  CRGB::Cyan,
  CRGB::White
};

uint8_t mode = 0;
unsigned long lastColorChange = 0;

// ---------- setup ----------
void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
}

// ---------- loop ----------
void loop() {

  // strong dim trail (your preferred setting)
  fadeToBlackBy(leds, NUM_LEDS, 150);

  // ---------- TWO HEAD EFFECT ----------
  int head1 = radius;
  int head2 = (radius + 1) % LEDS_PER_STRIP;

  for (int a = 0; a < NUM_STRIPS; a++) {

    int idx1 = getIndex(a, head1);
    int idx2 = getIndex(a, head2);

    leds[idx1] = colorA[mode];
    leds[idx2] = colorB[mode];
  }

  FastLED.show();
  delay(100);

  // move wave
  radius++;

  if (radius >= LEDS_PER_STRIP) {
    radius = 0;
  }

  // ---------- COLOR CHANGE EVERY 5 SECONDS ----------
  if (millis() - lastColorChange >= 5000) {
    mode++;
    if (mode >= 4) mode = 0;   // 4 color sets = 20 sec loop
    lastColorChange = millis();
  }
}