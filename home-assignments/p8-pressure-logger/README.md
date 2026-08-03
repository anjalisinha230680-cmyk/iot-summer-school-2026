#  Project 8: IoT Pressure & Altitude Logger

An ESP32-based atmospheric pressure, altitude, and temperature monitoring station that calculates sea-level pressure compensation
and predicts local weather trends using circular buffering.

## Project Overview

Meteorologists track atmospheric pressure trends over time to forecast local weather conditions. 
This project logs station pressure using a **BMP280 sensor**, displays live telemetry and local weather forecasts
on an **SSD1306 OLED screen**, and uses a **10kΩ potentiometer** to dynamically calibrate the local altitude baseline for accurate 
sea-level pressure compensation.

## Hardware Components

* **ESP32 Development Board**
* **BMP280** Pressure, Temperature & Altitude Sensor (I2C)
* **0.96" OLED Display** (SSD1306, 128x64, I2C)
* **10kΩ Potentiometer** (For altitude baseline calibration)
* **1x Green LED** (Stable/Rising trend indicator)
* **1x Red LED** (Falling trend/Rain prediction indicator)
* **2x 220Ω Resistors**
* Breadboard and Jumper Wires


## Mathematical Model & Physics

### 1. Altitude Effect on Pressure
Atmospheric pressure is caused by the weight of the air column above a given point. As altitude increases, the density of the air overhead decreases exponentially. 
Near sea level, pressure drops by approximately **1 hPa for every 8.4 meters** of elevation gain.

Because raw station pressure ($P$) varies drastically based on geographical altitude, raw readings cannot be directly used to compare weather patterns across different locations or baseline elevations.

### 2. Barometric Sea-Level Compensation Formula
To normalize station pressure ($P$) to Sea-Level Equivalent Pressure ($P_0$) given a known altitude ($h$ in meters), the **International Standard Atmosphere (ISA)** reduction equation is applied:

$$P_0 = \frac{P}{\left(1 - \frac{h}{44330.0}\right)^{5.255}}$$

* **$P_0$**: Equivalent Sea-Level Pressure (Pa / hPa)
* **$P$**: Raw Measured Station Pressure (Pa / hPa)
* **$h$**: Baseline Altitude mapped from the potentiometer ($0 - 2000\text{ m}$)
* **$44330.0$**: Constant based on atmospheric lapse rate
* **$5.255$**: Exponent calculated from standard hydrostatic constants 

### Required Arduino Libraries
* **Adafruit BMP280 Library**
* **Adafruit SSD1306**
* **Adafruit GFX Library**

