#include <FastLED.h>

// Configuration of WS2812B LED array
#define NUM_LEDS 256      // 8x32 array (8 rows, 32 columns)
#define DATA_PIN 10        // Data pin connected to Arduino Pin 6

CRGB leds[NUM_LEDS];

#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define ARRAY_WIDTH  32
#define ARRAY_HEIGHT 8

uint8_t rainbowHue = 0;    // Hue for the rainbow background
uint8_t barHue = 0;        // Hue for the moving bars

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void loop() {
  // Continuously animate both the rainbow background and the moving bars
  while (true) {
    animateRainbowBackground();  // Continuously move the rainbow background
    animateMovingBarsWithChangingColors();  // Animate the bars with color changes
  }
}

// Animate the rainbow background (keeps shifting colors over time)
void animateRainbowBackground() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(rainbowHue + (i / ARRAY_HEIGHT) * 10, 255, 255);  // Shift hue based on column
  }
  FastLED.show();
  rainbowHue += 15;  // Increase this value for faster background color change (was 5 before)
  delay(9);  // Decrease this for faster background animation (was 20 before)
}

// Function to map (x, y) coordinates in sequential zigzag configuration (column-based)
int XY(int x, int y) {
  if (x % 2 == 0) {
    // Even columns: top to bottom
    return (x * ARRAY_HEIGHT + y);
  } else {
    // Odd columns: bottom to top
    return (x * ARRAY_HEIGHT + (ARRAY_HEIGHT - 1 - y));
  }
}

// Animate two bars moving from the center to the edges and back, changing colors independently
void animateMovingBarsWithChangingColors() {
  int leftCol = 16;   // Starting position for the left bar
  int rightCol = 17;  // Starting position for the right bar

  // Move the bars outward to the edges
  for (int step = 0; step <= 16; step++) {
    // No need to restore background as it is already moving
    // Calculate the color for the bars using the barHue
    CRGB barColor = CHSV(barHue, 255, 255);

    // Draw left and right bars with the current color
    drawBar(leftCol - step, barColor);
    drawBar(rightCol + step, barColor);

    FastLED.show();
    delay(9);  // Control the speed of the bar movement

    barHue += 15;  // Change the hue slightly faster for the bars
  }

  // Move the bars back inward to the center
  for (int step = 16; step >= 0; step--) {
    // No need to restore background as it is already moving
    // Calculate the color for the bars using the barHue
    CRGB barColor = CHSV(barHue, 255, 255);

    // Draw left and right bars with the current color
    drawBar(leftCol - step, barColor);
    drawBar(rightCol + step, barColor);

    FastLED.show();
    delay(20);  // Control the speed of the bar movement

    barHue += 15;  // Change the hue slightly faster for the bars
  }
}

// Draw a vertical bar in a specified column
void drawBar(int col, CRGB color) {
  if (col >= 0 && col < ARRAY_WIDTH) {  // Ensure we're within bounds of the matrix
    for (int row = 0; row < ARRAY_HEIGHT; row++) {
      leds[XY(col, row)] = color;  // Set the color for the entire column
    }
  }
}
