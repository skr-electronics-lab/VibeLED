/*
  VibeLED.h - Library for creating stunning LED effects.
  Created by SKR Electronics Lab, 2025.
  Released under the MIT License.
  https://github.com/skr-electronics-lab
*/

#ifndef VibeLED_h
#define VibeLED_h

#include "Arduino.h"

// LED Types
#define LED_TYPE_SINGLE 0
#define LED_TYPE_RGB 1

// Effect IDs
enum EffectType {
  EFFECT_NONE = 0,
  EFFECT_STATIC = 1,
  EFFECT_BLINK = 2,
  EFFECT_BREATHE = 3,
  EFFECT_PULSE = 4,
  EFFECT_FADE_IN = 5,
  EFFECT_FADE_OUT = 6,
  EFFECT_KNIGHT_RIDER = 7,
  EFFECT_CYLON = 8,
  EFFECT_METEOR = 9,
  EFFECT_FIRE = 10,
  EFFECT_WATERFALL = 11,
  EFFECT_CHASE = 12,
  EFFECT_STACK = 13,
  EFFECT_RAINBOW = 14,
  EFFECT_SPARKLE = 15,
  EFFECT_MARQUEE = 16,
  EFFECT_BOUNCE = 17,
  EFFECT_COLOR_WIPE = 18,
  EFFECT_RANDOM_BLINK = 19,
  EFFECT_SNAKE = 20,
  EFFECT_WAVE = 21,
  EFFECT_TWINKLE = 22,
  EFFECT_STROBE = 23,
  EFFECT_LIGHTNING = 24,
  EFFECT_FIREWORK = 25,
  EFFECT_GRADIENT = 26,
  EFFECT_RUNNING_LIGHTS = 27,
  EFFECT_COLOR_FADE = 28,
  EFFECT_THEATER_CHASE = 29,
  EFFECT_SCAN = 30,
  EFFECT_DUAL_SCAN = 31,
  EFFECT_TWINKLE_RANDOM = 32,
  EFFECT_TWINKLE_FADE = 33,
  EFFECT_SPARKLE_FADE = 34,
  EFFECT_HYPER_SPARKLE = 35,
  EFFECT_STROBE_RAINBOW = 36,
  EFFECT_MULTI_STROBE = 37,
  EFFECT_CHASE_RAINBOW = 38,
  EFFECT_CHASE_BLACKOUT = 39,
  EFFECT_COLOR_SWEEP = 40,
  EFFECT_RUNNING_COLOR = 41,
  EFFECT_RUNNING_RED_BLUE = 42,
  EFFECT_RUNNING_RANDOM = 43,
  EFFECT_LARSON_SCANNER = 44,
  EFFECT_COMET = 45,
  EFFECT_FIREWORKS_RANDOM = 46,
  EFFECT_RAIN = 47,
  EFFECT_MERRY_CHRISTMAS = 48,
  EFFECT_FIRE_FLICKER = 49,
  EFFECT_FIRE_FLICKER_SOFT = 50,
  EFFECT_FIRE_FLICKER_INTENSE = 51,
  EFFECT_CIRCUS_COMBUSTUS = 52,
  EFFECT_HALLOWEEN = 53,
  EFFECT_BICOLOR_CHASE = 54,
  EFFECT_TRICOLOR_CHASE = 55,
  EFFECT_ICU = 56,
  EFFECT_CUSTOM = 99
};

// RGB Color structure
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  Color() : r(255), g(255), b(255) {}
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

// Effect parameters structure
struct EffectParams {
  uint16_t speed;      // Effect speed (lower = faster)
  uint8_t brightness;  // Effect brightness (0-255)
  Color color1;        // Primary color
  Color color2;        // Secondary color (for effects that use multiple colors)
  Color color3;        // Tertiary color (for effects that use multiple colors)
  uint8_t option1;     // Effect-specific option 1
  uint8_t option2;     // Effect-specific option 2

  EffectParams() :
    speed(100),
    brightness(255),
    color1(255, 255, 255),
    color2(0, 0, 255),
    color3(255, 0, 0),
    option1(0),
    option2(0) {}
};

class VibeLED {
  public:
    // Constructors
    VibeLED(uint8_t pin, uint16_t numLeds = 1);  // Single color LEDs
    VibeLED(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint16_t numLeds = 1);  // RGB LEDs

    // Basic methods
    void begin();
    void update();
    void clear();

    // Configuration methods
    void setBrightness(uint8_t brightness);
    void setDelay(uint16_t ms);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(Color color);

    // Effect control
    void setEffect(EffectType effect);
    void setEffect(EffectType effect, EffectParams params);
    void setEffect(EffectType effect, uint16_t speed);
    void setEffect(EffectType effect, uint16_t speed, Color color);
    void setEffect(EffectType effect, uint16_t speed, uint8_t r, uint8_t g, uint8_t b);
    void setEffect(String effectName);

    // Group control
    void setGroup(uint16_t startLed, uint16_t endLed);
    void resetGroup();

    // Direct LED control
    void setLED(uint16_t led, bool state);
    void setLED(uint16_t led, uint8_t r, uint8_t g, uint8_t b);
    void setLED(uint16_t led, Color color);

  private:
    uint8_t _ledType;
    uint16_t _numLeds;
    uint16_t _groupStart;
    uint16_t _groupEnd;
    uint8_t* _pins;
    uint8_t _numPins;

    EffectType _currentEffect;
    EffectParams _effectParams;

    unsigned long _lastUpdate;
    uint16_t _updateInterval;
    uint16_t _step;

    // Internal state
    bool* _ledStates;       // For single color LEDs
    Color* _ledColors;      // For RGB LEDs

    // Effect implementation methods
    void _updateEffect();
    void _applyStates();

    // Effect implementations
    void _effectNone();
    void _effectStatic();
    void _effectBlink();
    void _effectBreathe();
    void _effectPulse();
    void _effectFadeIn();
    void _effectFadeOut();
    void _effectKnightRider();
    void _effectCylon();
    void _effectMeteor();
    void _effectFire();
    void _effectWaterfall();
    void _effectChase();
    void _effectStack();
    void _effectRainbow();
    void _effectSparkle();
    void _effectMarquee();
    void _effectBounce();
    void _effectColorWipe();
    void _effectRandomBlink();
    void _effectSnake();
    void _effectWave();
    // Additional effect methods will be implemented as needed
};

#endif
