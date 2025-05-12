# VibeLED - Advanced LED Effects Library

![VibeLED Banner](https://via.placeholder.com/800x200/222222/FFFFFF?text=VibeLED:+50%2B+Stunning+LED+Effects)

A powerful Arduino library for creating stunning LED effects with minimal code. VibeLED allows you to easily implement 50+ professional-quality lighting effects on regular LEDs or RGB LEDs with just a few lines of code.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino Compatible](https://img.shields.io/badge/Arduino-Compatible-brightgreen.svg)](https://www.arduino.cc/)
[![Version](https://img.shields.io/badge/Version-1.0.0-blue.svg)](https://github.com/skr-electronics-lab/VibeLED/releases)

---

## 📋 Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Hardware Setup](#hardware-setup)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Available Effects](#available-effects)
- [Advanced Usage](#advanced-usage)
- [Troubleshooting](#troubleshooting)
- [Performance Considerations](#performance-considerations)
- [Contributing](#contributing)
- [License](#license)
- [Credits](#credits)

---

## ✨ Features

- **Non-blocking timing**: All effects use `millis()` instead of `delay()` for smooth animations and responsive sketches
- **Support for both single-color and RGB LEDs**: Works with standard LEDs, RGB LEDs, LED strips, and matrices
- **50+ built-in effects**: Knight Rider, Cylon, Meteor, Fire, Waterfall, Rainbow, and many more
- **Lightweight**: Optimized for AVR-based Arduinos (UNO/Nano) with minimal memory footprint
- **Simple API**: Easy to use with minimal setup - create stunning effects with just a few lines of code
- **Modular design**: Easily extendable for custom effects
- **Group control**: Ability to address subsets of LEDs for complex patterns
- **Customizable parameters**: Adjust speed, brightness, colors, and more for each effect
- **Compatible with popular LED libraries**: Can be used alongside FastLED, Adafruit NeoPixel, etc.

---

## 📥 Installation

### Method 1: Using the Arduino Library Manager (Recommended)

1. Open the Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries...**
3. In the Library Manager, search for "VibeLED"
4. Click "Install" on the VibeLED library

### Method 2: Manual Installation

1. Download the ZIP file from the [releases page](https://github.com/skr-electronics-lab/VibeLED/releases)
2. In the Arduino IDE, go to **Sketch > Include Library > Add .ZIP Library...**
3. Select the downloaded ZIP file
4. Restart the Arduino IDE

### Method 3: Using Git

Clone this repository directly into your Arduino libraries folder:

```bash
cd ~/Documents/Arduino/libraries
git clone https://github.com/skr-electronics-lab/VibeLED.git
```

---

## 🔌 Hardware Setup

### Single Color LEDs

For single color LEDs, you'll need:
- Arduino board (UNO, Nano, Mega, etc.)
- LEDs (as many as you want to control)
- Resistors (220Ω-1kΩ, depending on your LEDs)
- Jumper wires
- Breadboard (optional)

#### Basic Circuit Diagram:

```
Arduino Pin ──── Resistor ──── LED ──── GND
```

For multiple LEDs, you can connect them to different pins:

```
Arduino Pin 9 ──── Resistor ──── LED 1 ──── GND
Arduino Pin 10 ──── Resistor ──── LED 2 ──── GND
...
```

For many LEDs, consider using shift registers or LED driver ICs.

### RGB LEDs

For RGB LEDs, you'll need:
- Arduino board
- RGB LEDs (common anode or common cathode)
- Resistors (220Ω-1kΩ for each color channel)
- Jumper wires
- Breadboard (optional)

#### Basic Circuit Diagram (Common Cathode):

```
Arduino Pin 9 ──── Resistor ──── R pin of RGB LED
Arduino Pin 10 ──── Resistor ──── G pin of RGB LED
Arduino Pin 11 ──── Resistor ──── B pin of RGB LED
                                   GND pin of RGB LED ──── GND
```

### Addressable LED Strips

For addressable LED strips (WS2812B, NeoPixels, etc.), you'll need:
- Arduino board
- Addressable LED strip
- Power supply (5V, with enough current for your LEDs)
- 300-500Ω resistor
- 1000μF capacitor
- Jumper wires

#### Basic Circuit Diagram:

```
5V Power Supply ──── LED Strip VCC
                  └── Arduino 5V

Arduino GND ──── LED Strip GND
              └── Power Supply GND

Arduino Pin (e.g., 6) ──── 300-500Ω Resistor ──── LED Strip Data In
```

---

## 🚀 Quick Start

### Basic Usage

```cpp
#include <VibeLED.h>

// For single color LEDs
// VibeLED(pin, numLeds)
VibeLED leds(9, 10);  // 10 LEDs connected to pin 9

// For RGB LEDs
// VibeLED(rPin, gPin, bPin, numLeds)
// VibeLED rgbLeds(9, 10, 11, 10);  // 10 RGB LEDs with R, G, B on pins 9, 10, 11

void setup() {
  leds.begin();  // Initialize the library
  leds.setEffect(EFFECT_KNIGHT_RIDER);  // Set the effect
  leds.setDelay(100);  // Set the effect speed (lower = faster)
}

void loop() {
  leds.update();  // Update the effect animation (non-blocking)

  // Your other code can run here without being blocked!
  // Read sensors, check buttons, etc.
}
```

### Setting Effects

```cpp
// Set effect by enum
leds.setEffect(EFFECT_FIRE);

// Set effect with speed
leds.setEffect(EFFECT_METEOR, 50);  // 50ms between updates

// Set effect with speed and color (RGB LEDs)
leds.setEffect(EFFECT_PULSE, 100, 255, 0, 0);  // Red pulse

// Set effect with speed and Color object
Color purple(128, 0, 255);
leds.setEffect(EFFECT_BREATHE, 80, purple);

// Set effect by name (case-insensitive)
leds.setEffect("rainbow");
leds.setEffect("knight_rider");
leds.setEffect("fire");
```

### Controlling Groups of LEDs

```cpp
// Control a subset of LEDs
leds.setGroup(3, 7);  // Only affect LEDs 3 through 7
leds.setEffect(EFFECT_BLINK);

// Create multiple effects on different sections
leds.setGroup(0, 4);  // First 5 LEDs
leds.setEffect(EFFECT_FIRE);

leds.setGroup(5, 9);  // Next 5 LEDs
leds.setEffect(EFFECT_WATER);

// Reset to control all LEDs
leds.resetGroup();
leds.setEffect(EFFECT_RAINBOW);
```

### Direct LED Control

```cpp
// For single color LEDs
leds.setLED(0, true);  // Turn on first LED
leds.setLED(1, false); // Turn off second LED

// For RGB LEDs
leds.setLED(0, 255, 0, 0);    // Set first LED to red
leds.setLED(1, 0, 255, 0);    // Set second LED to green
leds.setLED(2, 0, 0, 255);    // Set third LED to blue
leds.setLED(3, 255, 255, 0);  // Set fourth LED to yellow

// Using Color objects
Color teal(0, 128, 128);
leds.setLED(4, teal);
```

---

## 📚 API Reference

### Constructor

```cpp
// For single color LEDs
VibeLED(uint8_t pin, uint16_t numLeds = 1);

// For RGB LEDs
VibeLED(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint16_t numLeds = 1);
```

### Basic Methods

| Method | Description |
|--------|-------------|
| `void begin()` | Initialize the library. Must be called in `setup()`. |
| `void update()` | Update the effect animation. Must be called in `loop()`. |
| `void clear()` | Turn off all LEDs. |

### Configuration Methods

| Method | Description |
|--------|-------------|
| `void setBrightness(uint8_t brightness)` | Set the brightness level (0-255). |
| `void setDelay(uint16_t ms)` | Set the delay between effect updates in milliseconds. |
| `void setColor(uint8_t r, uint8_t g, uint8_t b)` | Set the primary color for effects. |
| `void setColor(Color color)` | Set the primary color using a Color object. |

### Effect Control Methods

| Method | Description |
|--------|-------------|
| `void setEffect(EffectType effect)` | Set the current effect by enum. |
| `void setEffect(EffectType effect, uint16_t speed)` | Set effect with custom speed. |
| `void setEffect(EffectType effect, uint16_t speed, Color color)` | Set effect with speed and color. |
| `void setEffect(EffectType effect, uint16_t speed, uint8_t r, uint8_t g, uint8_t b)` | Set effect with speed and RGB color. |
| `void setEffect(EffectType effect, EffectParams params)` | Set effect with detailed parameters. |
| `void setEffect(String effectName)` | Set effect by name (case-insensitive). |

### Group Control Methods

| Method | Description |
|--------|-------------|
| `void setGroup(uint16_t startLed, uint16_t endLed)` | Set a group of LEDs to control. |
| `void resetGroup()` | Reset to control all LEDs. |

### Direct LED Control Methods

| Method | Description |
|--------|-------------|
| `void setLED(uint16_t led, bool state)` | Set a single LED on or off (single color). |
| `void setLED(uint16_t led, uint8_t r, uint8_t g, uint8_t b)` | Set a single LED color (RGB). |
| `void setLED(uint16_t led, Color color)` | Set a single LED color using a Color object. |

---

## 🌈 Available Effects

VibeLED includes 50+ stunning effects, each with customizable parameters:

### Basic Effects

1. **EFFECT_STATIC**: Solid color
   - *Description*: Displays a solid color on all LEDs
   - *Parameters*: Color

2. **EFFECT_BLINK**: Simple on/off blinking
   - *Description*: All LEDs blink on and off at a regular interval
   - *Parameters*: Speed, Color

3. **EFFECT_BREATHE**: Smooth fade in and out
   - *Description*: Smoothly fades LEDs in and out like breathing
   - *Parameters*: Speed, Color

4. **EFFECT_PULSE**: Quick fade in, slow fade out
   - *Description*: Quick fade in followed by a slower fade out
   - *Parameters*: Speed, Color

5. **EFFECT_FADE_IN**: Fade from off to on
   - *Description*: Gradually increases brightness from off to full
   - *Parameters*: Speed, Color

6. **EFFECT_FADE_OUT**: Fade from on to off
   - *Description*: Gradually decreases brightness from full to off
   - *Parameters*: Speed, Color

### Movement Effects

7. **EFFECT_KNIGHT_RIDER**: Back and forth with trail
   - *Description*: LED moves back and forth with a fading trail (like KITT from Knight Rider)
   - *Parameters*: Speed, Color, Trail Length

8. **EFFECT_CYLON**: Similar to Knight Rider with different trail
   - *Description*: Similar to Knight Rider but with a different trail effect (like Cylons from Battlestar Galactica)
   - *Parameters*: Speed, Color, Trail Length

9. **EFFECT_METEOR**: Meteor falling effect
   - *Description*: Simulates a meteor falling with a trail
   - *Parameters*: Speed, Color, Meteor Size, Trail Length

10. **EFFECT_CHASE**: Single LED chasing
    - *Description*: Single LED moving across the strip
    - *Parameters*: Speed, Color

11. **EFFECT_STACK**: Fill and empty
    - *Description*: LEDs fill up from one end, then empty
    - *Parameters*: Speed, Color

12. **EFFECT_SNAKE**: Snake moving through the strip
    - *Description*: A "snake" of lit LEDs moves through the strip
    - *Parameters*: Speed, Color, Snake Length

13. **EFFECT_WAVE**: Sine wave pattern
    - *Description*: Creates a sine wave pattern across the LEDs
    - *Parameters*: Speed, Color, Wave Length

### Nature-Inspired Effects

14. **EFFECT_FIRE**: Fire simulation
    - *Description*: Realistic fire effect with flickering and color gradients
    - *Parameters*: Speed, Intensity

15. **EFFECT_WATERFALL**: Cascading drops
    - *Description*: Simulates water droplets falling
    - *Parameters*: Speed, Color, Drop Frequency

16. **EFFECT_RAIN**: Rain simulation
    - *Description*: Simulates raindrops falling
    - *Parameters*: Speed, Color, Intensity

17. **EFFECT_LIGHTNING**: Lightning simulation
    - *Description*: Random flashes simulating lightning
    - *Parameters*: Flash Rate, Brightness

### Sparkle Effects

18. **EFFECT_SPARKLE**: Random twinkling
    - *Description*: Random LEDs light up and turn off
    - *Parameters*: Speed, Color, Density

19. **EFFECT_TWINKLE**: Gentle twinkling
    - *Description*: Softer version of sparkle with fade in/out
    - *Parameters*: Speed, Color, Density

20. **EFFECT_TWINKLE_RANDOM**: Random color twinkling
    - *Description*: Twinkling with random colors
    - *Parameters*: Speed, Density

21. **EFFECT_TWINKLE_FADE**: Twinkle with fade
    - *Description*: Twinkling with smooth fade in/out
    - *Parameters*: Speed, Color, Fade Rate

22. **EFFECT_SPARKLE_FADE**: Sparkle with fade
    - *Description*: Sparkle effect with fading
    - *Parameters*: Speed, Color, Fade Rate

23. **EFFECT_HYPER_SPARKLE**: Intense sparkling
    - *Description*: More intense and frequent sparkling
    - *Parameters*: Speed, Color, Density

### Color Effects (RGB Only)

24. **EFFECT_RAINBOW**: Rainbow cycle
    - *Description*: Full rainbow cycle across all LEDs
    - *Parameters*: Speed, Saturation, Brightness

25. **EFFECT_COLOR_FADE**: Fade between colors
    - *Description*: Smooth transition between colors
    - *Parameters*: Speed, Color1, Color2

26. **EFFECT_GRADIENT**: Color gradient
    - *Description*: Creates a gradient across the LEDs
    - *Parameters*: Color1, Color2

27. **EFFECT_COLOR_WIPE**: Color wipe in and out
    - *Description*: Color "wipes" across the LEDs
    - *Parameters*: Speed, Color

28. **EFFECT_COLOR_SWEEP**: Color sweep
    - *Description*: Color sweeps across the LEDs
    - *Parameters*: Speed, Color

### And Many More!

29. **EFFECT_MARQUEE**: Theater marquee effect
30. **EFFECT_BOUNCE**: Bouncing with physics
31. **EFFECT_RANDOM_BLINK**: Random blinking
32. **EFFECT_STROBE**: Strobe light effect
33. **EFFECT_FIREWORK**: Firework explosion
34. **EFFECT_RUNNING_LIGHTS**: Running lights
35. **EFFECT_THEATER_CHASE**: Theater chase
36. **EFFECT_SCAN**: Scanning effect
37. **EFFECT_DUAL_SCAN**: Dual scanning
38. **EFFECT_STROBE_RAINBOW**: Rainbow strobe
39. **EFFECT_MULTI_STROBE**: Multiple strobe
40. **EFFECT_CHASE_RAINBOW**: Rainbow chase
41. **EFFECT_CHASE_BLACKOUT**: Chase with blackout
42. **EFFECT_RUNNING_COLOR**: Running color
43. **EFFECT_RUNNING_RED_BLUE**: Running red and blue
44. **EFFECT_RUNNING_RANDOM**: Running random colors
45. **EFFECT_LARSON_SCANNER**: Larson scanner
46. **EFFECT_COMET**: Comet effect
47. **EFFECT_FIREWORKS_RANDOM**: Random fireworks
48. **EFFECT_MERRY_CHRISTMAS**: Christmas pattern
49. **EFFECT_FIRE_FLICKER**: Fire flickering
50. **EFFECT_FIRE_FLICKER_SOFT**: Soft fire flickering
51. **EFFECT_FIRE_FLICKER_INTENSE**: Intense fire flickering
52. **EFFECT_CIRCUS_COMBUSTUS**: Circus combustus
53. **EFFECT_HALLOWEEN**: Halloween pattern
54. **EFFECT_BICOLOR_CHASE**: Bicolor chase
55. **EFFECT_TRICOLOR_CHASE**: Tricolor chase
56. **EFFECT_ICU**: ICU effect

---

## 🔧 Advanced Usage

### Creating Custom Effects

You can create custom effects by extending the VibeLED class:

```cpp
class MyVibeLED : public VibeLED {
  public:
    // Use the same constructors
    MyVibeLED(uint8_t pin, uint16_t numLeds = 1) : VibeLED(pin, numLeds) {}
    MyVibeLED(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint16_t numLeds = 1) : VibeLED(rPin, gPin, bPin, numLeds) {}

    // Add your custom effect
    void setCustomEffect() {
      setEffect(EFFECT_CUSTOM);
    }

  protected:
    // Override the _updateEffect method to handle your custom effect
    void _updateEffect() {
      // Call the parent method first
      VibeLED::_updateEffect();

      // Add handling for your custom effect
      if (_currentEffect == EFFECT_CUSTOM) {
        // Implement your custom effect here
      }
    }
};
```

### Using Effect Parameters

For more control over effects, you can use the `EffectParams` structure:

```cpp
EffectParams params;
params.speed = 100;        // Update speed in milliseconds
params.brightness = 255;   // Brightness (0-255)
params.color1 = Color(255, 0, 0);  // Primary color (red)
params.color2 = Color(0, 0, 255);  // Secondary color (blue)
params.color3 = Color(0, 255, 0);  // Tertiary color (green)
params.option1 = 5;        // Effect-specific option 1 (e.g., trail length)
params.option2 = 3;        // Effect-specific option 2 (e.g., density)

leds.setEffect(EFFECT_KNIGHT_RIDER, params);
```

### Combining with Other Libraries

VibeLED can be used alongside other libraries for enhanced functionality:

```cpp
#include <VibeLED.h>
#include <FastLED.h>  // For advanced LED strip control
#include <Button.h>   // For button input

VibeLED leds(9, 10);
Button modeButton(2);  // Button on pin 2

void setup() {
  leds.begin();
  modeButton.begin();
}

void loop() {
  leds.update();

  if (modeButton.pressed()) {
    // Change effect when button is pressed
    static uint8_t effectIndex = 0;
    effectIndex = (effectIndex + 1) % 10;  // Cycle through 10 effects

    switch (effectIndex) {
      case 0: leds.setEffect(EFFECT_RAINBOW); break;
      case 1: leds.setEffect(EFFECT_FIRE); break;
      // ... more effects
    }
  }
}
```

---

## ❓ Troubleshooting

### Common Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| LEDs not lighting up | Incorrect wiring | Double-check connections, ensure proper polarity |
| | Insufficient power | Use an appropriate power supply for your LEDs |
| | Wrong pin numbers | Verify pin numbers in your code |
| Flickering LEDs | Power supply issues | Use a more powerful power supply, add capacitors |
| | Long wires | Shorten wires or use thicker gauge wire |
| Arduino freezes | Too many LEDs | Reduce the number of LEDs or use a more powerful Arduino |
| | Memory issues | Optimize your code, reduce variables |

### Debugging Tips

1. **Serial Output**: Add serial debugging to your code
   ```cpp
   Serial.begin(9600);
   Serial.println("Effect: " + String(currentEffect));
   ```

2. **Test with Fewer LEDs**: Start with just a few LEDs to isolate issues

3. **Check Power Requirements**: Ensure your power supply can handle the current draw
   - Each LED can draw up to 20mA at full brightness
   - RGB LEDs can draw up to 60mA (20mA per channel)

---

## ⚡ Performance Considerations

### Memory Usage

- Each single-color LED requires 1 byte of RAM
- Each RGB LED requires 3 bytes of RAM
- Additional memory is used for effect calculations

### CPU Usage

- More complex effects require more CPU time
- Higher update rates (lower delay values) increase CPU usage
- Consider using a more powerful Arduino (e.g., Mega, ESP32) for many LEDs or complex effects

### Power Consumption

- Calculate your power requirements:
  - Single LEDs: Number of LEDs × 20mA
  - RGB LEDs: Number of LEDs × 60mA
- Use an appropriate power supply with at least 20% headroom
- For many LEDs, use external power and proper wiring techniques

---

## 🤝 Contributing

Contributions to VibeLED are welcome! Here's how you can help:

1. **Report bugs**: Open an issue describing the bug and how to reproduce it
2. **Suggest features**: Open an issue describing the new feature
3. **Submit pull requests**: Fork the repository, make your changes, and submit a pull request

Please follow these guidelines:
- Follow the existing code style
- Add comments to explain complex code
- Test your changes thoroughly
- Update documentation as needed

---

## 📄 License

This library is released under the MIT License. See the LICENSE file for details.

---

## 👏 Credits

- Created by SKR Electronics Lab, 2025
- Inspired by various LED effect libraries and techniques
- Special thanks to the Arduino community for their support and inspiration
- Visit our GitHub: [https://github.com/skr-electronics-lab](https://github.com/skr-electronics-lab)
