# Bluetooth Home Light Controller

## Objective
A Bluetooth-controlled home light controller using ESP32 and a 2-channel relay module.

## Components Used
- ESP32 Dev Board
- 2-Channel Relay Module
- 2 LEDs
- 220Ω Resistors
- Active Buzzer
- Push Buttons
- Breadboard
- Jumper Wires

## Bluetooth Name
IIT_IoT_HomeCtrl

## Pairing Instructions
1. Power the ESP32.
2. Enable Bluetooth on your Android phone.
3. Pair with **IIT_IoT_HomeCtrl**.
4. Open the Serial Bluetooth Terminal app.
5. Connect to the ESP32.
6. Send the commands below.

## Supported Commands

| Command | Function |
|---------|----------|
| 1 | Light 1 ON |
| 2 | Light 1 OFF |
| 3 | Light 2 ON |
| 4 | Light 2 OFF |
| 5 | Both Lights ON |
| 6 | Both Lights OFF |
| ? | Display Current Status |
