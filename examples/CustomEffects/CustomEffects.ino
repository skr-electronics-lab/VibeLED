/*
  CustomEffects.ino - Custom effects example for the VibeLED library
  Created by SKR Electronics Lab, 2025.
  Released under the MIT License.
  https://github.com/skr-electronics-lab
*/

#include <VibeLED.h>

// Define the number of LEDs
#define NUM_LEDS 10

// Define the pins
#define LED_PIN 9  // For single color LEDs
// For RGB LEDs
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Create a custom class that extends VibeLED
class MyVibeLED : public VibeLED {
  public:
    // Constructors - pass through to parent class
    MyVibeLED(uint8_t pin, uint16_t numLeds = 1) : VibeLED(pin, numLeds) {}
    MyVibeLED(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint16_t numLeds = 1) : VibeLED(rPin, gPin, bPin, numLeds) {}

    // Custom effect methods
    void setAlternatingEffect(uint16_t speed, Color color1, Color color2) {
      _effectParams.speed = speed;
      _effectParams.color1 = color1;
      _effectParams.color2 = color2;
      _currentEffect = EFFECT_CUSTOM;
      _updateInterval = speed;
      _step = 0;
    }

    void setPoliceEffect(uint16_t speed) {
      _effectParams.speed = speed;
      _effectParams.color1 = Color(255, 0, 0);  // Red
      _effectParams.color2 = Color(0, 0, 255);  // Blue
      _currentEffect = 100;  // Custom effect ID
      _updateInterval = speed;
      _step = 0;
    }

  protected:
    // Override the _updateEffect method to handle custom effects
    void _updateEffect() {
      // Call the parent method first to handle built-in effects
      if (_currentEffect < EFFECT_CUSTOM) {
        VibeLED::_updateEffect();
        return;
      }

      // Handle custom effects
      switch (_currentEffect) {
        case EFFECT_CUSTOM:
          _customAlternatingEffect();
          break;
        case 100:  // Police effect
          _customPoliceEffect();
          break;
        default:
          // Unknown effect, fall back to none
          _effectNone();
          break;
      }

      _step++;
    }

  private:
    // Implementation of custom alternating effect
    void _customAlternatingEffect() {
      if (_ledType == LED_TYPE_RGB) {
        uint16_t numLeds = _groupEnd - _groupStart + 1;

        for (uint16_t i = 0; i < numLeds; i++) {
          // Alternate between two colors based on position and time
          bool useColor1 = ((i + (_step / 5)) % 2) == 0;

          if (useColor1) {
            _ledColors[_groupStart + i] = _effectParams.color1;
          } else {
            _ledColors[_groupStart + i] = _effectParams.color2;
          }
        }
      } else {
        // For single color LEDs, just alternate on/off
        uint16_t numLeds = _groupEnd - _groupStart + 1;

        for (uint16_t i = 0; i < numLeds; i++) {
          bool isOn = ((i + (_step / 5)) % 2) == 0;
          _ledStates[_groupStart + i] = isOn;
        }
      }
    }

    // Implementation of custom police effect
    void _customPoliceEffect() {
      if (_ledType == LED_TYPE_RGB) {
        uint16_t numLeds = _groupEnd - _groupStart + 1;
        uint16_t halfPoint = numLeds / 2;

        // Determine which side is active based on time
        bool leftSide = (_step / 10) % 2 == 0;

        for (uint16_t i = 0; i < numLeds; i++) {
          if (i < halfPoint) {
            // Left side
            _ledColors[_groupStart + i] = leftSide ? _effectParams.color1 : Color(0, 0, 0);
          } else {
            // Right side
            _ledColors[_groupStart + i] = leftSide ? Color(0, 0, 0) : _effectParams.color2;
          }
        }
      } else {
        // For single color LEDs, just alternate left/right
        uint16_t numLeds = _groupEnd - _groupStart + 1;
        uint16_t halfPoint = numLeds / 2;

        // Determine which side is active based on time
        bool leftSide = (_step / 10) % 2 == 0;

        for (uint16_t i = 0; i < numLeds; i++) {
          if (i < halfPoint) {
            // Left side
            _ledStates[_groupStart + i] = leftSide;
          } else {
            // Right side
            _ledStates[_groupStart + i] = !leftSide;
          }
        }
      }
    }
};

// Create an instance of our custom class
// For single color LEDs
MyVibeLED leds(LED_PIN, NUM_LEDS);

// For RGB LEDs (uncomment to use)
// MyVibeLED leds(RED_PIN, GREEN_PIN, BLUE_PIN, NUM_LEDS);

// Variables for effect cycling
unsigned long lastEffectChange = 0;
uint8_t currentEffect = 0;
const uint8_t numEffects = 4;  // Number of effects to cycle through
const unsigned long effectDuration = 5000;  // 5 seconds per effect

void setup() {
  Serial.begin(9600);
  Serial.println(F("VibeLED Custom Effects Example"));

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
  // Cycle through effects including custom ones
  currentEffect = (currentEffect + 1) % numEffects;

  switch (currentEffect) {
    case 0:
      Serial.println(F("Effect: Built-in Knight Rider"));
      leds.setEffect(EFFECT_KNIGHT_RIDER);
      leds.setDelay(100);
      break;
    case 1:
      Serial.println(F("Effect: Built-in Rainbow"));
      leds.setEffect(EFFECT_RAINBOW);
      leds.setDelay(50);
      break;
    case 2:
      Serial.println(F("Effect: Custom Alternating"));
      leds.setAlternatingEffect(100, Color(255, 0, 0), Color(0, 255, 0));  // Red and Green alternating
      break;
    case 3:
      Serial.println(F("Effect: Custom Police"));
      leds.setPoliceEffect(100);  // Red and Blue police lights
      break;
  }
}
