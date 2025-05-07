# Engineering_Calculator
DSN Engineering Calculator A touchscreen-based scientific calculator built with an ESP32-S3 and an ILI9341 TFT display, using the TFT_eSPI library. 

✨ Features
Touch-enabled button interface

Basic operations: +, -, *, /

Scientific functions: sin(), cos(), tan(), sqrt(), log(), exp()

Expression evaluation support (e.g. 9+3, sin30, etc.)

Touch calibration screen on startup

Clean, responsive UI

Backspace < and clear C support

Splash screen with project logo

📷 Hardware Requirements
ESP32-S3 development board

2.8" or similar TFT display (ILI9341 driver)

Touch support enabled and calibrated

Optional: MicroSD or LittleFS support (for future expansion)

📦 Libraries Used
TFT_eSPI by Bodmer

LittleFS for calibration data

Standard ESP32 Arduino Core

🔧 Setup
Connect your ILI9341 display to the ESP32-S3 as per your pinout.

Make sure you’ve configured User_Setup.h in TFT_eSPI properly.

Upload the code via Arduino IDE.

On first boot, follow the on-screen touch calibration.

💡 Future Improvements
Expression parser for more complex formulas

Multi-line input and history

Dark/light theme toggle

Save and recall calculations

Tutorial link : https://youtu.be/t3zZAPMOZzM
