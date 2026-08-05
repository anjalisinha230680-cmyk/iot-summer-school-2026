# Smart Home Security and Automation System using ESP32

## Project Overview

This project is an IoT-based Smart Home Security and Automation System developed using the ESP32 microcontroller. The system enhances home security and improves convenience by integrating multiple sensors and actuators. It continuously monitors the surrounding environment, detects potential security threats, automates lighting, controls door and gate access, and provides real-time alerts through an OLED display, RGB LED, and buzzer.

The project demonstrates how IoT technology can be used to build an affordable and intelligent smart home solution.

---

## Components Used

### Controller
- ESP32 Development Board

### Sensors
- DHT11 Temperature and Humidity Sensor
- Flame Sensor
- LDR (Light Sensor)
- Touch Sensor
- Reed Switch
- Shock Sensor
- IR Tracking Sensor
- High Sensitive Voice Sensor

### Output Devices
- OLED Display (128×64 I2C)
- Servo Motor (Door Lock)
- 28BYJ-48 Stepper Motor with ULN2003 Driver (Gate Control)
- Relay Module
- RGB LED Module
- Buzzer

### Other Components
- Breadboard
- Jumper Wires
- USB Cable
- Resistors
- ESP32 Power Supply

## Working Methodology

The ESP32 continuously collects data from all connected sensors.

- The DHT11 monitors temperature and humidity.
- The Flame Sensor detects fire.
- The Reed Switch detects whether the door is open or closed.
- The IR Tracking Sensor detects nearby objects or intruders.
- The Shock Sensor detects vibrations or tampering.
- The Voice Sensor detects loud sounds.
- The LDR automatically controls lighting using the relay.
- The Touch Sensor arms and disarms the security system.

Based on the sensor readings, the ESP32 performs the following actions:

- Displays information on the OLED screen.
- Locks or unlocks the door using the servo motor.
- Opens or closes the gate using the stepper motor.
- Turns the relay ON or OFF for automatic lighting.
- Activates the RGB LED for status indication.
- Sounds the buzzer whenever an emergency is detected.

---

## Features

- Smart Home Security
- Fire Detection
- Automatic Lighting
- Smart Door Lock
- Smart Gate Control
- Environmental Monitoring
- OLED Status Display
- Visual and Audible Alerts
- Real-Time Sensor Monitoring

---

## Applications

- Smart Homes
- Apartments
- Offices
- Shops
- Warehouses
- Laboratories
- Hostels


## Future Improvements

- Mobile Application
- Cloud Dashboard
- Email Notifications
- SMS Alerts
- Face Recognition
- Voice Assistant Integration
- Remote Monitoring using Wi-Fi

---

## Output

The system provides:

- Real-time temperature and humidity monitoring.
- Fire detection and emergency alerts.
- Intruder detection using multiple sensors.
- Automatic lighting control.
- Smart door and gate operation.
- OLED display showing live system status.
- Audio and visual alarm during emergencies.

---

## Conclusion

The Smart Home Security and Automation System successfully integrates multiple sensors with the ESP32 to provide security, automation, and environmental monitoring. The project is affordable, scalable, and demonstrates the practical implementation of IoT in modern smart homes.
