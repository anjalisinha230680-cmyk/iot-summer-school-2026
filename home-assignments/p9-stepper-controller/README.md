# Project 9: Stepper Motor Precision Positioner

## Overview

This project demonstrates a precision stepper motor positioning system using an Arduino Uno, A4988 stepper motor driver, bipolar stepper motor, OLED display, potentiometer, push buttons, and LED.

The system allows the user to set a target angle using a potentiometer, rotate the motor clockwise or counterclockwise using dedicated push buttons, and reset the current position using a Home button.
The OLED continuously displays the motor status.

This project introduces the basic concepts used in CNC machines, robotic arms, and 3D printers where accurate position control is essential.

## Features

- Precision stepper motor control
- Clockwise and Counter-Clockwise rotation
- Home position reset
- Potentiometer controlled target angle (0°–360°)
- OLED display showing:
  - Current Angle
  - Target Angle
  - Direction
  - Step Count
- LED status indicator
- Serial Monitor output for debugging


## Components Used

- Arduino Uno
- A4988 Stepper Motor Driver
- Bipolar Stepper Motor
- 0.96" OLED Display (I2C)
- 10k Potentiometer
- 3 Push Buttons
- LED
- 220Ω Resistor
- Breadboard
- Jumper Wires


## Working Principle

1. The potentiometer selects the desired target angle between 0° and 360°.

2. Pressing the **CW** button rotates the stepper motor approximately 45° clockwise.

3. Pressing the **CCW** button rotates the motor approximately 45° counter-clockwise.

4. Pressing the **HOME** button resets the logical position to zero without physically moving the motor.

5. The OLED continuously displays:
   - Current Angle
   - Target Angle
   - Motor Direction
   - Total Steps

6. The LED lights while the motor is moving.


## Applications

- CNC Machines
- Robotic Arms
- 3D Printers
- Camera Slider Systems
- Pick and Place Machines
- Automated Positioning Systems
- Industrial Automation


## Future Improvements

- Automatic movement to target angle
- Acceleration and deceleration control
- Speed adjustment
- Position memory using EEPROM
- Rotary encoder support
- Closed-loop feedback using encoders
