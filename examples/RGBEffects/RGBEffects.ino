/*
  RGBEffects.ino - RGB LED example for the VibeLED library
  Created by SKR Electronics Lab, 2025.
  Released under the MIT License.
  https://github.com/skr-electronics-lab
*/

#include <VibeLED.h>

// Define the number of LEDs
#define NUM_LEDS 10

// Define the pins for RGB LEDs
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Create the VibeLED instance for RGB LEDs
VibeLED leds(RED_PIN, GREEN_PIN, BLUE_PIN, NUM_LEDS);

// Variables for effect cycling
unsigned long lastEffectChange = 0;
uint8_t currentEffect = 0;
const uint8_t numEffects = 10;  // Number of effects to cycle through
const unsigned long effectDuration = 5000;  // 5 seconds per effect

// Color definitions
Color RED(255, 0, 0);
Color GREEN(0, 255, 0);
Color BLUE(0, 0, 255);
Color YELLOW(255, 255, 0);
Color CYAN(0, 255, 255);
Color MAGENTA(255, 0, 255);
Color WHITE(255, 255, 255);

void setup() {
  Serial.begin(9600);
  Serial.println(F("VibeLED RGB Effects Example"));

  // Initialize the library
  leds.begin();

  // Set initial brightness
  leds.setBrightness(255);  // Full brightness

  // Set initial effect
  setNextEffect();
}

void loop() {
  // Update the effect animation (non-blocking)
  leds.update();

  // Change effect every 5 seconds
  if (millis() - lastEffectChange > effectDuration) {
    lastEffectChange = millis();
    setNextEffect();
  }
}

void setNextEffect() {
  // Cycle through a selection of effects
  currentEffect = (currentEffect + 1) % numEffects;

  switch (currentEffect) {
    case 0:
      Serial.println(F("Effect: Rainbow"));
      leds.setEffect(EFFECT_RAINBOW);
      leds.setDelay(50);
      break;
    case 1:
      Serial.println(F("Effect: Fire"));
      leds.setEffect(EFFECT_FIRE);
      leds.setDelay(30);
      break;
    case 2:
      Serial.println(F("Effect: Color Wipe (Red)"));
      leds.setEffect(EFFECT_COLOR_WIPE, 100, RED);
      break;
    case 3:
      Serial.println(F("Effect: Knight Rider (Blue)"));
      leds.setEffect(EFFECT_KNIGHT_RIDER, 80, BLUE);
      break;
    case 4:
      Serial.println(F("Effect: Meteor (Green)"));
      leds.setEffect(EFFECT_METEOR, 50, GREEN);
      break;
    case 5:
      Serial.println(F("Effect: Breathe (Cyan)"));
      leds.setEffect(EFFECT_BREATHE, 20, CYAN);
      break;
    case 6:
      Serial.println(F("Effect: Sparkle (Yellow)"));
      leds.setEffect(EFFECT_SPARKLE, 40, YELLOW);
      break;
    case 7:
      Serial.println(F("Effect: Snake (Magenta)"));
      leds.setEffect(EFFECT_SNAKE, 100, MAGENTA);
      break;
    case 8:
      Serial.println(F("Effect: Wave (White)"));
      leds.setEffect(EFFECT_WAVE, 50, WHITE);
      break;
    case 9:
      Serial.println(F("Effect: Random Blink"));
      leds.setEffect(EFFECT_RANDOM_BLINK);
      leds.setDelay(100);
      break;
  }
}
