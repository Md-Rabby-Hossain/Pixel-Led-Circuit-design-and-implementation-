# ATmega8A Multi-Pattern Pixel LED Controller

A custom-mapped pixel LED decoration controller powered by an **ATmega8A** microcontroller using the **FastLED** library. This project cycles continuously through 10 distinct, high-energy lighting patterns and dynamic color themes without blocking execution.

## 🛠️ Hardware Specifications
* **Microcontroller:** ATmega8A (programmed via USBasp / ISP)
* **LED Type:** WS2812 / NeoPixel (12 strips × 8 LEDs = 96 total LEDs)
* **Data Pin:** Digital Pin 10
* **Mapping:** Custom Zig-Zag mapping (handles alternating reversed odd/even strip layouts)

## ✨ Features
* **10 Unique Patterns:** Includes multi-speed chasing trains, radial pulses, dual-direction color waves, and thunder streams.
* **Non-Blocking Architecture:** Uses master timers (`millis()`) to seamlessly transition between patterns every 20 seconds.
* **Dynamic Themes:** Cycles through multiple vibrant color palettes (Gold, Blue, Purple, Hot Pink, Aqua, etc.).
* **Memory Optimized:** Compiled using **MinCore** to maximize the ATmega8's 8KB flash memory.

## 📂 Code Structure
The main sketch (`pixel_led_controller.ino`) encapsulates each pattern into its own non-blocking function:
- **Patterns 1–2:** Dual-head radial waves and multi-color shifting sets.
- **Patterns 3–4:** Full bright/dim train combinations and inward/outward radial pulses.
- **Patterns 5–6:** Dual-system counter-flows and eye-catching multi-theme trains.
- **Patterns 7–10:** Speed-segregated groups, diagonal wave sweeps, thunder streams, and high-speed core pulses.

## 🚀 How to Build & Upload
1. Install the **FastLED** library in your Arduino IDE.
2. Use **MinCore** by MCUdude in the Boards Manager to configure the target as **ATmega8** with **No bootloader**.
3. Connect your **USBasp** programmer to the ISP pins.
4. Select **Sketch > Upload Using Programmer** to flash the code.
