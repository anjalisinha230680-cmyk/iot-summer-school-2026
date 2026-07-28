# Smart Room Climate Monitor

## Project Overview
This project is an IoT-based Smart Room Climate Monitor built using the ESP32 development board. It measures room temperature and humidity using a DHT11 sensor and provides alerts through an LED and buzzer when the room becomes too hot or too humid.

## Components Used
- ESP32 Development Board
- DHT11 Temperature & Humidity Sensor
- 0.96" OLED Display (SSD1306 I2C)
- Active Buzzer
- LED
- 220Ω Resistor
- Breadboard
- Jumper Wires
- USB Cable

## Libraries Used
- DHTesp
- Wire
- Adafruit GFX
- Adafruit SSD1306

## Wiring Connections

### DHT11
- VCC → 3.3V
- GND → GND
- DATA → GPIO4

### OLED Display
- VCC → 3.3V
- GND → GND
- SDA → GPIO21
- SCL → GPIO22

### LED
- Positive → GPIO25 (through 220Ω resistor)
- Negative → GND

### Buzzer
- Positive → GPIO27
- Negative → GND

## Features
- Reads temperature and humidity every 2 seconds.
- Displays readings on the OLED display.
- Shows status as COOL, COMFORT, HOT, or DANGER.
- Activates the LED and buzzer when the alert threshold is exceeded.
- Logs sensor readings to the Serial Monitor in CSV format every 5 seconds.

## Expected Output

OLED:
- Temperature
- Humidity
- Status (COOL/COMFORT/HOT/DANGER)

Serial Monitor (CSV):
5000,29.5,65.0,COMFORT
10000,29.6,65.2,COMFORT
15000,30.1,72.0,HOT
