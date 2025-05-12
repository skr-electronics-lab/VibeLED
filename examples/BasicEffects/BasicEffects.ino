/*
  BasicEffects.ino - Basic example for the VibeLED library
  Created by SKR Electronics Lab, 2025.
  Released under the MIT License.
  https://github.com/skr-electronics-lab
*/

#include <VibeLED.h>

// Define the number of LEDs
#define NUM_LEDS 10

// For single color LEDs
// VibeLED(pin, numLeds)
VibeLED leds(9, NUM_LEDS);  // 10 LEDs connected to pin 9

// For RGB LEDs (uncomment to use)
// VibeLED leds(9, 10, 11, NUM_LEDS);  // 10 RGB LEDs with R, G, B on pins 9, 10, 11

// Variables for effect cycling
unsigned long lastEffectChange = 0;
uint8_t currentEffect = 0;
const uint8_t numEffects = 10;  // Number of effects to cycle through
const unsigned long effectDuration = 5000;  // 5 seconds per effect

void setup() {
  Serial.begin(9600);
  Serial.println(F("VibeLED Basic Effects Example"));

  // Initialize the library
  leds.begin();

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
      Serial.println(F("Effect: Knight Rider"));
      leds.setEffect(EFFECT_KNIGHT_RIDER);
      leds.setDelay(100);
      break;
    case 1:
      Serial.println(F("Effect: Cylon"));
      leds.setEffect(EFFECT_CYLON);
      leds.setDelay(80);
      break;
    case 2:
      Serial.println(F("Effect: Meteor"));
      leds.setEffect(EFFECT_METEOR);
      leds.setDelay(50);
      break;
    case 3:
      Serial.println(F("Effect: Fire"));
      leds.setEffect(EFFECT_FIRE);
      leds.setDelay(30);
      break;
    case 4:
      Serial.println(F("Effect: Breathe"));
      leds.setEffect(EFFECT_BREATHE);
      leds.setDelay(20);
      break;
    case 5:
      Serial.println(F("Effect: Rainbow"));
      leds.setEffect(EFFECT_RAINBOW);
      leds.setDelay(50);
      break;
    case 6:
      Serial.println(F("Effect: Sparkle"));
      leds.setEffect(EFFECT_SPARKLE);
      leds.setDelay(40);
      break;
    case 7:
      Serial.println(F("Effect: Snake"));
      leds.setEffect(EFFECT_SNAKE);
      leds.setDelay(100);
      break;
    case 8:
      Serial.println(F("Effect: Wave"));
      leds.setEffect(EFFECT_WAVE);
      leds.setDelay(50);
      break;
    case 9:
      Serial.println(F("Effect: Bounce"));
      leds.setEffect(EFFECT_BOUNCE);
      leds.setDelay(30);
      break;
  }
}
