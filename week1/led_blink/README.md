Updated
# LED Blink Project

## Project Title
LED Blink with Serial Monitor and Potentiometer Control

## Hardware Required
- Arduino Uno / ESP32
- LED
- 220Ω Resistor
- Potentiometer (10kΩ)
- Breadboard
- Jumper Wires
- USB Cable
- Computer with Arduino IDE

## Circuit Diagram Description
- Connect the LED anode (+) to Digital Pin 13 through a 220Ω resistor.
- Connect the LED cathode (-) to GND.
- Connect the potentiometer:
  - Left pin → 5V
  - Middle pin → A0
  - Right pin → GND.
- Connect the Arduino to the computer using a USB cable.

## How to Upload Code
1. Connect the Arduino board to the computer.
2. Open Arduino IDE.
3. Open the blink.ino file.
4. Select the correct Board from the Tools menu.
5. Select the correct Port from the Tools menu.
6. Click the Verify button to compile the code.
7. Click the Upload button to upload the program.
8. Open the Serial Monitor to view the blink count.

## Expected Output
- The LED blinks continuously.
- Blink speed changes when the potentiometer is rotated.
- The Serial Monitor displays the blink count.

## Troubleshooting Tips
1. Check that the correct board and COM port are selected.
2. Verify all circuit connections, especially the LED polarity.
3. Ensure the USB cable supports data transfer and not just charging.
