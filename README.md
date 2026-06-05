# Quiz ESP32 — Who Wants to Be a Millionaire?

![ESP32](https://img.shields.io/badge/ESP--32-black?style=flat-square&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino_IDE-00979D?style=flat-square&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)

An interactive quiz game inspired by *Who Wants to Be a Millionaire?*, built with an ESP32 microcontroller and components from the Freenove Ultimate Starter Kit.

---

## Demonstrations

| Scenario | Video |
|----------|-------|
| Victory — All questions answered correctly | [![Victory](https://youtube.com/shorts/QfdzBolSjC4) |
| Wrong Answer & Time Expired  — Game Over | [![Wrong Answer](https://img.youtube.com/vi/VIDEO_ID_2/0.jpg)](https://youtube.com/shorts/gII11c-Q8kY) |


---

## Circuit

![ESP32 Quiz Circuit](aura.WEBP)


---

## Features

- **Multiple Choice** — 8 general knowledge questions, each with 4 answer options.
- **Dynamic Timer** — 9-second countdown per question, displayed in real time on the LCD.
- **Visual Feedback** — Green LED for correct answers; Red LED for wrong answers or timeout.
- **Audio Feedback** — Passive buzzer plays distinct sound effects for correct answers, a defeat melody on game over, and a victory tune on completion.
- **Intuitive Interface** — A dedicated `START` button initiates the game and toggles the LCD between option pairs (A/B and C/D).
- **Complete Game Loop** — Victory screen with animation before auto-reset; automatic restart after game over or victory.

---

## Hardware

| Component | Qty | Notes |
|-----------|:---:|-------|
| ESP32-WROVER-CAM | 1 | Freenove board, or any compatible ESP32 |
| LCD 16x2 I2C | 1 | Default address `0x27` |
| Large Push Button | 1 | START / Toggle function |
| Small Push Button | 4 | Answer options A, B, C, D |
| Green LED | 1 | Correct answer indicator |
| Red LED | 1 | Wrong answer indicator |
| Passive Buzzer | 1 | Melodies and sound effects |
| 220 Ohm Resistor | 2 | LED current limiting |
| Breadboard | 1 | Assembly base |
| Jumper Wires | — | Male-to-Male and Male-to-Female |

---

## Pin Mapping

| Component | ESP32 GPIO |
|-----------|-----------|
| BTN START | `GPIO 5` |
| BTN A | `GPIO 26` |
| BTN B | `GPIO 27` |
| BTN C | `GPIO 12` |
| BTN D | `GPIO 13` |
| LED Green | `GPIO 32` |
| LED Red | `GPIO 25` |
| Buzzer | `GPIO 19` |
| LCD SDA | `GPIO 22` |
| LCD SCL | `GPIO 23` |

---

## Setup and Installation

### Prerequisites

1. Install [Arduino IDE 2.x](https://www.arduino.cc/en/software).
2. Add ESP32 board support:
   - Go to **File → Preferences**.
   - Under *Additional Boards Manager URLs*, paste:
     ```
     https://espressif.github.io/arduino-esp32/package_esp32_index.json
     ```
   - Go to **Tools → Board → Boards Manager**, search for `esp32`, and install.

### Required Library

The LCD display requires the I2C library by Frank de Brabander:

1. Go to **Tools → Manage Libraries**.
2. Search for `LiquidCrystal I2C`.
3. Install the version by **Frank de Brabander**.

### Uploading the Code

1. Open the project `.ino` file in the Arduino IDE.
2. Select your ESP32 board under **Tools → Board**.
3. Select the correct port under **Tools → Port**.
4. Click **Upload**.

---

## Built With

- [Arduino IDE](https://www.arduino.cc/) — Development environment
- [ESP32 Core for Arduino](https://github.com/espressif/arduino-esp32) — Programming core
- [Freenove ESP32 Ultimate Starter Kit](https://freenove.com/) — Hardware platform

---

## License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute this code.
