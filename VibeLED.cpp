/*
  VibeLED.cpp - Library for creating stunning LED effects.
  Created by SKR Electronics Lab, 2025.
  Released under the MIT License.
  https://github.com/skr-electronics-lab
*/

#include "VibeLED.h"

// Constructor for single color LEDs
VibeLED::VibeLED(uint8_t pin, uint16_t numLeds) {
  _ledType = LED_TYPE_SINGLE;
  _numLeds = numLeds;
  _numPins = 1;
  _pins = new uint8_t[1];
  _pins[0] = pin;

  _ledStates = new bool[numLeds];
  _ledColors = nullptr;

  _currentEffect = EFFECT_NONE;
  _updateInterval = 100;
  _lastUpdate = 0;
  _step = 0;

  _groupStart = 0;
  _groupEnd = numLeds - 1;
}

// Constructor for RGB LEDs
VibeLED::VibeLED(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint16_t numLeds) {
  _ledType = LED_TYPE_RGB;
  _numLeds = numLeds;
  _numPins = 3;
  _pins = new uint8_t[3];
  _pins[0] = rPin;
  _pins[1] = gPin;
  _pins[2] = bPin;

  _ledStates = nullptr;
  _ledColors = new Color[numLeds];

  _currentEffect = EFFECT_NONE;
  _updateInterval = 100;
  _lastUpdate = 0;
  _step = 0;

  _groupStart = 0;
  _groupEnd = numLeds - 1;
}

// Initialize the library
void VibeLED::begin() {
  // Initialize pins
  for (uint8_t i = 0; i < _numPins; i++) {
    pinMode(_pins[i], OUTPUT);
  }

  // Initialize LED states
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = 0; i < _numLeds; i++) {
      _ledStates[i] = false;
    }
  } else {
    for (uint16_t i = 0; i < _numLeds; i++) {
      _ledColors[i] = Color(0, 0, 0);
    }
  }

  // Apply initial states
  _applyStates();
}

// Update the effect (should be called in loop())
void VibeLED::update() {
  if (millis() - _lastUpdate >= _updateInterval) {
    _lastUpdate = millis();
    _updateEffect();
    _applyStates();
  }
}

// Clear all LEDs
void VibeLED::clear() {
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = 0; i < _numLeds; i++) {
      _ledStates[i] = false;
    }
  } else {
    for (uint16_t i = 0; i < _numLeds; i++) {
      _ledColors[i] = Color(0, 0, 0);
    }
  }
  _applyStates();
}

// Set brightness (only affects RGB LEDs)
void VibeLED::setBrightness(uint8_t brightness) {
  _effectParams.brightness = brightness;
}

// Set delay between effect updates
void VibeLED::setDelay(uint16_t ms) {
  _updateInterval = ms;
}

// Set color (RGB)
void VibeLED::setColor(uint8_t r, uint8_t g, uint8_t b) {
  _effectParams.color1 = Color(r, g, b);
}

// Set color (Color struct)
void VibeLED::setColor(Color color) {
  _effectParams.color1 = color;
}

// Set effect by type
void VibeLED::setEffect(EffectType effect) {
  _currentEffect = effect;
  _step = 0;
}

// Set effect with parameters
void VibeLED::setEffect(EffectType effect, EffectParams params) {
  _currentEffect = effect;
  _effectParams = params;
  _updateInterval = params.speed;
  _step = 0;
}

// Set effect with speed
void VibeLED::setEffect(EffectType effect, uint16_t speed) {
  _currentEffect = effect;
  _effectParams.speed = speed;
  _updateInterval = speed;
  _step = 0;
}

// Set effect with speed and color
void VibeLED::setEffect(EffectType effect, uint16_t speed, Color color) {
  _currentEffect = effect;
  _effectParams.speed = speed;
  _effectParams.color1 = color;
  _updateInterval = speed;
  _step = 0;
}

// Set effect with speed and RGB color
void VibeLED::setEffect(EffectType effect, uint16_t speed, uint8_t r, uint8_t g, uint8_t b) {
  _currentEffect = effect;
  _effectParams.speed = speed;
  _effectParams.color1 = Color(r, g, b);
  _updateInterval = speed;
  _step = 0;
}

// Set effect by name
void VibeLED::setEffect(String effectName) {
  // Convert string to effect type
  if (effectName.equalsIgnoreCase("static")) setEffect(EFFECT_STATIC);
  else if (effectName.equalsIgnoreCase("blink")) setEffect(EFFECT_BLINK);
  else if (effectName.equalsIgnoreCase("breathe")) setEffect(EFFECT_BREATHE);
  else if (effectName.equalsIgnoreCase("pulse")) setEffect(EFFECT_PULSE);
  else if (effectName.equalsIgnoreCase("fade_in")) setEffect(EFFECT_FADE_IN);
  else if (effectName.equalsIgnoreCase("fade_out")) setEffect(EFFECT_FADE_OUT);
  else if (effectName.equalsIgnoreCase("knight_rider")) setEffect(EFFECT_KNIGHT_RIDER);
  else if (effectName.equalsIgnoreCase("cylon")) setEffect(EFFECT_CYLON);
  else if (effectName.equalsIgnoreCase("meteor")) setEffect(EFFECT_METEOR);
  else if (effectName.equalsIgnoreCase("fire")) setEffect(EFFECT_FIRE);
  else if (effectName.equalsIgnoreCase("waterfall")) setEffect(EFFECT_WATERFALL);
  else if (effectName.equalsIgnoreCase("chase")) setEffect(EFFECT_CHASE);
  else if (effectName.equalsIgnoreCase("stack")) setEffect(EFFECT_STACK);
  else if (effectName.equalsIgnoreCase("rainbow")) setEffect(EFFECT_RAINBOW);
  else if (effectName.equalsIgnoreCase("sparkle")) setEffect(EFFECT_SPARKLE);
  else if (effectName.equalsIgnoreCase("marquee")) setEffect(EFFECT_MARQUEE);
  else if (effectName.equalsIgnoreCase("bounce")) setEffect(EFFECT_BOUNCE);
  else if (effectName.equalsIgnoreCase("color_wipe")) setEffect(EFFECT_COLOR_WIPE);
  else if (effectName.equalsIgnoreCase("random_blink")) setEffect(EFFECT_RANDOM_BLINK);
  else if (effectName.equalsIgnoreCase("snake")) setEffect(EFFECT_SNAKE);
  else if (effectName.equalsIgnoreCase("wave")) setEffect(EFFECT_WAVE);
  else setEffect(EFFECT_NONE);
}

// Set group of LEDs to control
void VibeLED::setGroup(uint16_t startLed, uint16_t endLed) {
  _groupStart = constrain(startLed, 0, _numLeds - 1);
  _groupEnd = constrain(endLed, _groupStart, _numLeds - 1);
}

// Reset group to all LEDs
void VibeLED::resetGroup() {
  _groupStart = 0;
  _groupEnd = _numLeds - 1;
}

// Set single LED state (for single color LEDs)
void VibeLED::setLED(uint16_t led, bool state) {
  if (_ledType == LED_TYPE_SINGLE && led < _numLeds) {
    _ledStates[led] = state;
  }
}

// Set single LED color (for RGB LEDs)
void VibeLED::setLED(uint16_t led, uint8_t r, uint8_t g, uint8_t b) {
  if (_ledType == LED_TYPE_RGB && led < _numLeds) {
    _ledColors[led] = Color(r, g, b);
  }
}

// Set single LED color (for RGB LEDs)
void VibeLED::setLED(uint16_t led, Color color) {
  if (_ledType == LED_TYPE_RGB && led < _numLeds) {
    _ledColors[led] = color;
  }
}

// Update the current effect
void VibeLED::_updateEffect() {
  switch (_currentEffect) {
    case EFFECT_NONE:
      _effectNone();
      break;
    case EFFECT_STATIC:
      _effectStatic();
      break;
    case EFFECT_BLINK:
      _effectBlink();
      break;
    case EFFECT_BREATHE:
      _effectBreathe();
      break;
    case EFFECT_PULSE:
      _effectPulse();
      break;
    case EFFECT_FADE_IN:
      _effectFadeIn();
      break;
    case EFFECT_FADE_OUT:
      _effectFadeOut();
      break;
    case EFFECT_KNIGHT_RIDER:
      _effectKnightRider();
      break;
    case EFFECT_CYLON:
      _effectCylon();
      break;
    case EFFECT_METEOR:
      _effectMeteor();
      break;
    case EFFECT_FIRE:
      _effectFire();
      break;
    case EFFECT_WATERFALL:
      _effectWaterfall();
      break;
    case EFFECT_CHASE:
      _effectChase();
      break;
    case EFFECT_STACK:
      _effectStack();
      break;
    case EFFECT_RAINBOW:
      _effectRainbow();
      break;
    case EFFECT_SPARKLE:
      _effectSparkle();
      break;
    case EFFECT_MARQUEE:
      _effectMarquee();
      break;
    case EFFECT_BOUNCE:
      _effectBounce();
      break;
    case EFFECT_COLOR_WIPE:
      _effectColorWipe();
      break;
    case EFFECT_RANDOM_BLINK:
      _effectRandomBlink();
      break;
    case EFFECT_SNAKE:
      _effectSnake();
      break;
    case EFFECT_WAVE:
      _effectWave();
      break;
    default:
      _effectNone();
      break;
  }

  _step++;
}

// Apply LED states to physical pins
void VibeLED::_applyStates() {
  if (_ledType == LED_TYPE_SINGLE) {
    // For single color LEDs, we directly set the pin state for each LED
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      digitalWrite(_pins[0], _ledStates[i] ? HIGH : LOW);
      // Note: This is a simplified implementation. In a real-world scenario,
      // you would need to use shift registers or other methods to control multiple LEDs
      // with a single pin.
    }
  } else {
    // For RGB LEDs, we need to set the RGB values for each LED
    // This is a simplified implementation. In a real-world scenario,
    // you would need to use a library like FastLED or Adafruit NeoPixel
    // to control RGB LEDs.
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      analogWrite(_pins[0], map(_ledColors[i].r, 0, 255, 0, _effectParams.brightness));
      analogWrite(_pins[1], map(_ledColors[i].g, 0, 255, 0, _effectParams.brightness));
      analogWrite(_pins[2], map(_ledColors[i].b, 0, 255, 0, _effectParams.brightness));
    }
  }
}

// Effect implementations

// No effect (all LEDs off)
void VibeLED::_effectNone() {
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = false;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledColors[i] = Color(0, 0, 0);
    }
  }
}

// Static effect (all LEDs on with the current color)
void VibeLED::_effectStatic() {
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = true;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledColors[i] = _effectParams.color1;
    }
  }
}

// Blink effect (all LEDs blink together)
void VibeLED::_effectBlink() {
  bool state = (_step % 2 == 0);

  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = state;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledColors[i] = state ? _effectParams.color1 : Color(0, 0, 0);
    }
  }
}

// Breathe effect (fade in and out)
void VibeLED::_effectBreathe() {
  // Use sine wave for smooth breathing effect
  float breath = (sin((_step % 100) / 15.0) + 1.0) / 2.0;

  if (_ledType == LED_TYPE_SINGLE) {
    bool state = breath > 0.5;
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = state;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _effectParams.color1.r * breath;
      uint8_t g = _effectParams.color1.g * breath;
      uint8_t b = _effectParams.color1.b * breath;
      _ledColors[i] = Color(r, g, b);
    }
  }
}

// Pulse effect (quick fade in, slow fade out)
void VibeLED::_effectPulse() {
  uint8_t pulseStep = _step % 100;
  float intensity;

  if (pulseStep < 20) {
    // Quick fade in (20% of the cycle)
    intensity = pulseStep / 20.0;
  } else {
    // Slow fade out (80% of the cycle)
    intensity = 1.0 - ((pulseStep - 20) / 80.0);
  }

  if (_ledType == LED_TYPE_SINGLE) {
    bool state = intensity > 0.5;
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = state;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _effectParams.color1.r * intensity;
      uint8_t g = _effectParams.color1.g * intensity;
      uint8_t b = _effectParams.color1.b * intensity;
      _ledColors[i] = Color(r, g, b);
    }
  }
}

// Fade in effect
void VibeLED::_effectFadeIn() {
  float intensity = min(1.0, _step / 100.0);

  if (_ledType == LED_TYPE_SINGLE) {
    bool state = intensity > 0.5;
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = state;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _effectParams.color1.r * intensity;
      uint8_t g = _effectParams.color1.g * intensity;
      uint8_t b = _effectParams.color1.b * intensity;
      _ledColors[i] = Color(r, g, b);
    }
  }

  // Reset effect when fully faded in
  if (intensity >= 1.0) {
    _currentEffect = EFFECT_STATIC;
  }
}

// Fade out effect
void VibeLED::_effectFadeOut() {
  float intensity = max(0.0, 1.0 - (_step / 100.0));

  if (_ledType == LED_TYPE_SINGLE) {
    bool state = intensity > 0.5;
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = state;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _effectParams.color1.r * intensity;
      uint8_t g = _effectParams.color1.g * intensity;
      uint8_t b = _effectParams.color1.b * intensity;
      _ledColors[i] = Color(r, g, b);
    }
  }

  // Reset effect when fully faded out
  if (intensity <= 0.0) {
    _currentEffect = EFFECT_NONE;
  }
}

// Knight Rider effect (back and forth)
void VibeLED::_effectKnightRider() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t position;
  uint16_t cycleLength = (numLeds * 2) - 2;

  if (cycleLength < 2) cycleLength = 2;

  uint16_t cyclePos = _step % cycleLength;

  if (cyclePos < numLeds) {
    position = cyclePos;
  } else {
    position = cycleLength - cyclePos;
  }

  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = (i == _groupStart + position);
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      if (i == _groupStart + position) {
        _ledColors[i] = _effectParams.color1;
      } else {
        // Create a trail effect
        uint16_t distance = abs((int)(i - (_groupStart + position)));
        if (distance <= 2) {
          float intensity = (3 - distance) / 3.0;
          uint8_t r = _effectParams.color1.r * intensity;
          uint8_t g = _effectParams.color1.g * intensity;
          uint8_t b = _effectParams.color1.b * intensity;
          _ledColors[i] = Color(r, g, b);
        } else {
          _ledColors[i] = Color(0, 0, 0);
        }
      }
    }
  }
}

// Cylon effect (similar to Knight Rider but with different trail)
void VibeLED::_effectCylon() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t position;
  uint16_t cycleLength = (numLeds * 2) - 2;

  if (cycleLength < 2) cycleLength = 2;

  uint16_t cyclePos = _step % cycleLength;

  if (cyclePos < numLeds) {
    position = cyclePos;
  } else {
    position = cycleLength - cyclePos;
  }

  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = (i == _groupStart + position);
    }
  } else {
    // First, dim all LEDs
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _ledColors[i].r * 0.8;
      uint8_t g = _ledColors[i].g * 0.8;
      uint8_t b = _ledColors[i].b * 0.8;
      _ledColors[i] = Color(r, g, b);
    }

    // Set the current position to full brightness
    _ledColors[_groupStart + position] = _effectParams.color1;
  }
}

// Meteor effect
void VibeLED::_effectMeteor() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t meteorSize = min(numLeds / 4, 4);
  uint16_t position = (_step % (numLeds + meteorSize)) - meteorSize;

  // First, dim all LEDs
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = false;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _ledColors[i].r * 0.7;
      uint8_t g = _ledColors[i].g * 0.7;
      uint8_t b = _ledColors[i].b * 0.7;
      _ledColors[i] = Color(r, g, b);
    }
  }

  // Draw meteor
  for (uint16_t i = 0; i < meteorSize; i++) {
    uint16_t index = position + i;
    if (index >= 0 && index < numLeds) {
      if (_ledType == LED_TYPE_SINGLE) {
        _ledStates[_groupStart + index] = true;
      } else {
        float intensity = (meteorSize - i) / (float)meteorSize;
        uint8_t r = _effectParams.color1.r * intensity;
        uint8_t g = _effectParams.color1.g * intensity;
        uint8_t b = _effectParams.color1.b * intensity;
        _ledColors[_groupStart + index] = Color(r, g, b);
      }
    }
  }
}

// Fire effect
void VibeLED::_effectFire() {
  if (_ledType == LED_TYPE_RGB) {
    uint16_t numLeds = _groupEnd - _groupStart + 1;

    // Heat array (0-255) for each LED
    static uint8_t* heat = nullptr;
    if (heat == nullptr) {
      heat = new uint8_t[numLeds];
      for (uint16_t i = 0; i < numLeds; i++) {
        heat[i] = 0;
      }
    }

    // Step 1: Cool down every LED a little
    for (uint16_t i = 0; i < numLeds; i++) {
      uint8_t cooldown = random(0, ((10 * 10) / numLeds) + 2);
      if (cooldown > heat[i]) {
        heat[i] = 0;
      } else {
        heat[i] = heat[i] - cooldown;
      }
    }

    // Step 2: Heat rises - move heat up
    for (uint16_t i = numLeds - 1; i >= 2; i--) {
      heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    // Step 3: Randomly ignite new sparks at the bottom
    if (random(255) < 120) {
      uint8_t y = random(7);
      heat[y] = min(heat[y] + random(160, 255), 255);
    }

    // Step 4: Convert heat to LED colors
    for (uint16_t i = 0; i < numLeds; i++) {
      // Scale heat value to RGB
      uint8_t temperature = heat[i];
      uint8_t r, g, b;

      // Hotter = more red and yellow, cooler = more blue
      if (temperature < 85) {
        r = temperature * 3;
        g = 0;
        b = 0;
      } else if (temperature < 170) {
        r = 255;
        g = (temperature - 85) * 3;
        b = 0;
      } else {
        r = 255;
        g = 255;
        b = (temperature - 170) * 3;
      }

      _ledColors[_groupStart + i] = Color(r, g, b);
    }
  } else {
    // For single color LEDs, just do a random flicker
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = random(100) < 50;
    }
  }
}

// Waterfall effect
void VibeLED::_effectWaterfall() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;

  // Shift all LEDs down by one
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupEnd; i > _groupStart; i--) {
      _ledStates[i] = _ledStates[i - 1];
    }

    // Randomly add new drops at the top
    _ledStates[_groupStart] = random(100) < 20;
  } else {
    for (uint16_t i = _groupEnd; i > _groupStart; i--) {
      _ledColors[i] = _ledColors[i - 1];
    }

    // Randomly add new drops at the top
    if (random(100) < 20) {
      _ledColors[_groupStart] = _effectParams.color1;
    } else {
      _ledColors[_groupStart] = Color(0, 0, 0);
    }
  }
}

// Chase effect
void VibeLED::_effectChase() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t position = _step % numLeds;

  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = (i == _groupStart + position);
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      if (i == _groupStart + position) {
        _ledColors[i] = _effectParams.color1;
      } else {
        _ledColors[i] = Color(0, 0, 0);
      }
    }
  }
}

// Stack effect
void VibeLED::_effectStack() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t cycleLength = numLeds * 2;
  uint16_t cyclePos = _step % cycleLength;

  if (cyclePos < numLeds) {
    // Filling phase
    uint16_t position = cyclePos;

    if (_ledType == LED_TYPE_SINGLE) {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        _ledStates[i] = (i <= _groupStart + position);
      }
    } else {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        if (i <= _groupStart + position) {
          _ledColors[i] = _effectParams.color1;
        } else {
          _ledColors[i] = Color(0, 0, 0);
        }
      }
    }
  } else {
    // Emptying phase
    uint16_t position = cycleLength - cyclePos;

    if (_ledType == LED_TYPE_SINGLE) {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        _ledStates[i] = (i <= _groupStart + position);
      }
    } else {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        if (i <= _groupStart + position) {
          _ledColors[i] = _effectParams.color1;
        } else {
          _ledColors[i] = Color(0, 0, 0);
        }
      }
    }
  }
}

// Rainbow effect (RGB only)
void VibeLED::_effectRainbow() {
  if (_ledType == LED_TYPE_RGB) {
    uint16_t numLeds = _groupEnd - _groupStart + 1;

    for (uint16_t i = 0; i < numLeds; i++) {
      // Calculate hue based on position and time
      uint8_t hue = (i * 255 / numLeds + _step) % 256;

      // Convert HSV to RGB (simplified)
      uint8_t region = hue / 43;
      uint8_t remainder = (hue - (region * 43)) * 6;

      uint8_t p = 0;
      uint8_t q = 255 - remainder;
      uint8_t t = remainder;

      uint8_t r, g, b;

      switch (region) {
        case 0: r = 255; g = t; b = p; break;
        case 1: r = q; g = 255; b = p; break;
        case 2: r = p; g = 255; b = t; break;
        case 3: r = p; g = q; b = 255; break;
        case 4: r = t; g = p; b = 255; break;
        default: r = 255; g = p; b = q; break;
      }

      _ledColors[_groupStart + i] = Color(r, g, b);
    }
  } else {
    // For single color LEDs, just do a wave pattern
    uint16_t numLeds = _groupEnd - _groupStart + 1;
    for (uint16_t i = 0; i < numLeds; i++) {
      float sinVal = sin((_step / 10.0) + (i / 2.0));
      _ledStates[_groupStart + i] = sinVal > 0;
    }
  }
}

// Sparkle effect
void VibeLED::_effectSparkle() {
  // First, dim all LEDs
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = false;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      uint8_t r = _ledColors[i].r * 0.8;
      uint8_t g = _ledColors[i].g * 0.8;
      uint8_t b = _ledColors[i].b * 0.8;
      _ledColors[i] = Color(r, g, b);
    }
  }

  // Randomly turn on some LEDs
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t numSparkles = max(1, numLeds / 10);

  for (uint16_t i = 0; i < numSparkles; i++) {
    uint16_t pos = random(numLeds);

    if (_ledType == LED_TYPE_SINGLE) {
      _ledStates[_groupStart + pos] = true;
    } else {
      _ledColors[_groupStart + pos] = _effectParams.color1;
    }
  }
}

// Marquee effect
void VibeLED::_effectMarquee() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;

  for (uint16_t i = 0; i < numLeds; i++) {
    bool isOn = ((i + _step) % 3 == 0);

    if (_ledType == LED_TYPE_SINGLE) {
      _ledStates[_groupStart + i] = isOn;
    } else {
      _ledColors[_groupStart + i] = isOn ? _effectParams.color1 : Color(0, 0, 0);
    }
  }
}

// Bounce effect
void VibeLED::_effectBounce() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;

  // Calculate position with easing (slowing down at the ends)
  float normalized = (_step % 100) / 100.0;
  float position = sin(normalized * PI) * (numLeds - 1);
  uint16_t pos = (uint16_t)position;

  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = (i == _groupStart + pos);
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      if (i == _groupStart + pos) {
        _ledColors[i] = _effectParams.color1;
      } else {
        _ledColors[i] = Color(0, 0, 0);
      }
    }
  }
}

// Color wipe effect
void VibeLED::_effectColorWipe() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t position = _step % (numLeds * 2);

  if (position < numLeds) {
    // Wipe in
    if (_ledType == LED_TYPE_SINGLE) {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        _ledStates[i] = (i <= _groupStart + position);
      }
    } else {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        if (i <= _groupStart + position) {
          _ledColors[i] = _effectParams.color1;
        } else {
          _ledColors[i] = Color(0, 0, 0);
        }
      }
    }
  } else {
    // Wipe out
    position = position - numLeds;
    if (_ledType == LED_TYPE_SINGLE) {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        _ledStates[i] = (i > _groupStart + position);
      }
    } else {
      for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
        if (i > _groupStart + position) {
          _ledColors[i] = _effectParams.color1;
        } else {
          _ledColors[i] = Color(0, 0, 0);
        }
      }
    }
  }
}

// Random blink effect
void VibeLED::_effectRandomBlink() {
  if (_step % 5 == 0) {  // Update every 5 steps
    uint16_t numLeds = _groupEnd - _groupStart + 1;

    for (uint16_t i = 0; i < numLeds; i++) {
      bool isOn = random(100) < 30;  // 30% chance to be on

      if (_ledType == LED_TYPE_SINGLE) {
        _ledStates[_groupStart + i] = isOn;
      } else {
        if (isOn) {
          // Random color for RGB LEDs
          uint8_t r = random(256);
          uint8_t g = random(256);
          uint8_t b = random(256);
          _ledColors[_groupStart + i] = Color(r, g, b);
        } else {
          _ledColors[_groupStart + i] = Color(0, 0, 0);
        }
      }
    }
  }
}

// Snake effect
void VibeLED::_effectSnake() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;
  uint16_t snakeLength = max(1, numLeds / 4);

  // Clear all LEDs
  if (_ledType == LED_TYPE_SINGLE) {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledStates[i] = false;
    }
  } else {
    for (uint16_t i = _groupStart; i <= _groupEnd; i++) {
      _ledColors[i] = Color(0, 0, 0);
    }
  }

  // Draw snake
  for (uint16_t i = 0; i < snakeLength; i++) {
    uint16_t pos = (_step + i) % numLeds;

    if (_ledType == LED_TYPE_SINGLE) {
      _ledStates[_groupStart + pos] = true;
    } else {
      // Gradient color for the snake
      float intensity = 1.0 - (i / (float)snakeLength);
      uint8_t r = _effectParams.color1.r * intensity;
      uint8_t g = _effectParams.color1.g * intensity;
      uint8_t b = _effectParams.color1.b * intensity;
      _ledColors[_groupStart + pos] = Color(r, g, b);
    }
  }
}

// Wave effect
void VibeLED::_effectWave() {
  uint16_t numLeds = _groupEnd - _groupStart + 1;

  for (uint16_t i = 0; i < numLeds; i++) {
    float sinVal = sin((_step / 10.0) + (i / 2.0));
    float intensity = (sinVal + 1.0) / 2.0;  // Convert from -1..1 to 0..1

    if (_ledType == LED_TYPE_SINGLE) {
      _ledStates[_groupStart + i] = intensity > 0.5;
    } else {
      uint8_t r = _effectParams.color1.r * intensity;
      uint8_t g = _effectParams.color1.g * intensity;
      uint8_t b = _effectParams.color1.b * intensity;
      _ledColors[_groupStart + i] = Color(r, g, b);
    }
  }
}