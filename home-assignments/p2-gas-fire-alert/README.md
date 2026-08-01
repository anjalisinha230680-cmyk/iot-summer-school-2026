#  Gas & Fire Safety Alert System

##  Project Overview
The **Gas & Fire Safety Alert System** is an Arduino-based safety project that monitors gas leakage and fire hazards. An **MQ Gas Sensor** is used to detect gas concentration, while a **slide switch simulates flame detection** in the Wokwi simulation. The system alerts users using LEDs, a buzzer, and Serial Monitor messages.

##  Features
- Detects gas leakage using an MQ Gas Sensor.
- Simulates flame detection using a slide switch.
- Visual indication using Green, Yellow, and Red LEDs.
- Audible alert using a passive buzzer.
- Displays real-time system status on the Serial Monitor.

## Components Used
- Arduino Uno
- MQ Gas Sensor
- Slide Switch (Flame Simulation)
- Green LED
- Yellow LED
- Red LED
- Passive Buzzer
- 220Ω Resistors
- Breadboard
- Jumper Wires


## Working Principle

### Safe Condition
- Gas level is below the threshold.
- Slide switch is OFF.
- Green LED is ON.
- Buzzer is OFF.

### Gas Leak Warning
- Gas level exceeds the threshold.
- Yellow LED turns ON.
- Warning message is displayed on the Serial Monitor.

###  Fire Detection
- The slide switch is turned ON to simulate flame detection.
- Red LED turns ON.
- The buzzer sounds.

###  Gas + Fire Emergency
- Gas concentration exceeds the threshold and the slide switch is ON.
- Red LED remains ON.
- The buzzer beeps continuously.
- Emergency status is displayed on the Serial Monitor.

## Applications
- Home Safety Systems
- Kitchen Safety Monitoring
- Industrial Safety
- Educational Arduino & IoT Projects


