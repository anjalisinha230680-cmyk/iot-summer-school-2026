#  Full IoT Home Automation Hub

##  Project Overview

This project is a complete IoT-based Home Automation Hub built using an ESP32. 
It monitors environmental conditions, automatically controls appliances, displays live data on an OLED screen, hosts a web dashboard, 
and publishes sensor data to an MQTT broker.

This project combines multiple IoT concepts including sensors, automation, Wi-Fi networking, MQTT communication, OLED display, 
and relay control.


#  Objectives

- Monitor room temperature and humidity
- Detect gas leakage
- Detect motion using PIR sensor
- Measure ambient light intensity
- Automatically control fan and lighting
- Display live sensor data on OLED
- Host a web dashboard using ESP32
- Publish sensor readings to MQTT
- Provide manual relay override
- Trigger safety shutdown during gas leakage


# Components Used

- ESP32 Dev Board
- DHT22 Temperature & Humidity Sensor
- MQ-2 Gas Sensor
- PIR Motion Sensor
- LDR Module
- 2-Channel Relay Module
- OLED Display (SSD1306 I2C)
- Passive Buzzer
- Red LED
- Green LED
- Push Buttons ×2
- Breadboard
- Jumper Wires

#  Automation Rules

##  Fan Automation

- Fan ON when temperature > 32°C
- Fan OFF when temperature < 28°C

##  Light Automation

- Light turns ON when:
  - Motion is detected
  - Room is dark

- Light turns OFF when:
  - Daylight is detected
  - No motion for 3 minutes

##  Gas Safety

When gas concentration exceeds the threshold:

- Red LED turns ON
- Passive buzzer sounds
- All relays switch OFF
- MQTT alert is published

#  OLED Display

The OLED cycles through three screens every 5 seconds.

### Screen 1

- Temperature
- Humidity

### Screen 2

- Gas Level
- Motion Status
- Light Level

### Screen 3

- Fan Status
- Light Status
- System Uptime


#  Web Dashboard

The ESP32 hosts a web server displaying:

- Temperature
- Humidity
- Gas Level
- Motion Status
- Light Level
- Fan Status
- Light Status
- Automation Status
- Manual Override Status
- System Uptime


# 📡 MQTT

Broker:

broker.hivemq.com

Example Topic:

iitjammu/anjali/home


Published JSON Example

 json
{
  "temp":29.6,
  "humidity":64,
  "gas":18,
  "pir":1,
  "light":42,
  "fan":0,
  "light_relay":1,
  "alert":0
}


#  How to Run

1. Open the project in Wokwi.
2. Install the required libraries.
3. Upload the sketch to the ESP32.
4. Connect to Wi-Fi.
5. Open the ESP32 IP address in a browser.
6. Monitor live sensor values.
7. Observe automatic relay operation.
8. Verify MQTT messages.



#  Demonstration

The demonstration includes:

- Temperature change
- Motion detection
- Dark/light detection
- Gas leakage alert
- OLED updates
- Web dashboard
- MQTT messages
- Relay switching

#  Features

- Multi-sensor monitoring
- Automatic fan control
- Automatic light control
- Gas leakage safety shutdown
- OLED live display
- Web dashboard
- MQTT communication
- Manual relay override
- Non-blocking programming using millis()
- System uptime monitoring
