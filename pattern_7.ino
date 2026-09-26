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

unsigned long lastMove = 0;
unsigned long lastColorChange = 0;

// =====================================================
// Theme structure
// =====================================================
struct Theme {

  CRGB evenBody;
  CRGB evenHead;
  CRGB oddBody;
};

// =====================================================
// Eye-catching themes
// =====================================================
Theme themes[] = {

  // RED + GOLD | PURPLE
  {CRGB::Red, CRGB(255,180,20), CRGB::Purple},

  // BLUE + WHITE | AQUA
  {CRGB::Blue, CRGB::White, CRGB::Aqua},

  // GREEN + YELLOW | MAGENTA
  {CRGB::Green, CRGB::Yellow, CRGB::Magenta},

  // HOT PINK + GOLD | BLUE
  {CRGB::DeepPink, CRGB(255,180,20), CRGB::Blue},

  // ORANGE + WHITE | CYAN
  {CRGB::Orange, CRGB::White, CRGB::Cyan},

  // MAGENTA + GOLD | GREEN
  {CRGB::Magenta, CRGB(255,180,20), CRGB::Green}
};

#define THEME_COUNT 6

uint8_t currentTheme = 0;

// =====================================================
// Setup
// =====================================================
void setup() {

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.clear();
  FastLED.show();
}

// =====================================================
// Main Loop
// =====================================================
void loop() {

  // =================================================
  // FAST BACK FADE
  // Bigger number = faster fade
  // =================================================
  fadeToBlackBy(leds, NUM_LEDS, 110);

  // =================================================
  // Change theme every 5 sec
  // =================================================
  if (millis() - lastColorChange > 5000) {

    lastColorChange = millis();

    currentTheme++;

    if (currentTheme >= THEME_COUNT) {
      currentTheme = 0;
    }
  }

  // =================================================
  // Current colors
  // =================================================
  CRGB EVEN_BODY = themes[currentTheme].evenBody;
  CRGB EVEN_HEAD = themes[currentTheme].evenHead;
  CRGB ODD_BODY  = themes[currentTheme].oddBody;

  // =================================================
  // Draw trains
  // =================================================
  for (int strip = 0; strip < NUM_STRIPS; strip++) {

    // -------------------------------------------------
    // EVEN STRIPS
    // -------------------------------------------------
    if (strip % 2 == 0) {

      // shorter fading body
      for (int t = pos - 3; t < pos; t++) {

        if (t >= 0 && t < LEDS_PER_STRIP) {

          leds[getIndex(strip, t)] += EVEN_BODY;
        }
      }

      // bright golden head
      if (pos < LEDS_PER_STRIP)
        leds[getIndex(strip, pos)] += EVEN_HEAD;

      if (pos + 1 < LEDS_PER_STRIP)
        leds[getIndex(strip, pos + 1)] += EVEN_HEAD;
    }

    // -------------------------------------------------
    // ODD STRIPS
    // -------------------------------------------------
    else {

      // purple moving train
      for (int t = pos - 3; t <= pos; t++) {

        if (t >= 0 && t < LEDS_PER_STRIP) {

          leds[getIndex(strip, t)] += ODD_BODY;
        }
      }
    }
  }

  FastLED.show();

  // =================================================
  // Slower outward motion
  // =================================================
if (millis() - lastMove > 115) {

  lastMove = millis();

  pos++;

  // allow tail to fully leave board
  if (pos >= LEDS_PER_STRIP + 5) {

    pos = 0;
  }
}
}