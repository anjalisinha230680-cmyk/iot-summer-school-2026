#  Overview
This project is a Wi-Fi-based weather monitoring system built using the ESP32. 
The ESP32 hosts a live weather dashboard that can be accessed from any device connected to the same Wi-Fi network. 
The dashboard displays real-time temperature, humidity, atmospheric pressure, altitude, and light intensity without using any cloud services.



##  Features
 Connects to a local Wi-Fi network.
 Measures temperature and humidity using the DHT11 sensor.
 Measures atmospheric pressure and altitude using the BMP280 sensor.
 Measures ambient light intensity using an LDR.
 Hosts a live HTML dashboard on the ESP32.
 Automatically refreshes every 10 seconds.
 Provides sensor data in JSON format through the `/data` endpoint.
 Changes webpage background color based on temperature.
 Displays the connected Wi-Fi SSID and IP address on the OLED display.
 Automatically reconnects if Wi-Fi is disconnected.

##  Components Used
 ESP32 Dev Board
 DHT11 Temperature & Humidity Sensor
 BMP280 Pressure & Altitude Sensor
 0.96" OLED Display (I2C)
 LDR (Light Dependent Resistor)
 10kΩ Resistor
 Breadboard
 Jumper Wires



## Libraries Used

  WiFi.h
  ESPAsyncWebServer.h
  AsyncTCP.h
  DHTesp.h
  Adafruit_BMP280.h
  Adafruit_GFX.h
  Adafruit_SSD1306.h
  ArduinoJson.h



##  Setup

1. Install all required libraries.
2. Enter your Wi-Fi SSID and password in the code.
3. Upload the sketch to the ESP32.
4. Open the Serial Monitor (115200 baud).
5. Wait for the ESP32 to connect to Wi-Fi.
6. Copy the displayed IP address.
7. Open a web browser on a device connected to the same Wi-Fi network.
8. Enter the IP address to view the dashboard.
  

##  Local Network Format

* **IP Address Structure**: `http://192.168.X.X/`
* **JSON Endpoint**: `http://192.168.X.X/data`

---

##  Served HTML Source Code Structure


<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="10">
  <title>ESP32 Weather Dashboard</title>
  <style>
    body { font-family: Arial, sans-serif; background-color: #e6f2ff; text-align: center; margin:0; padding:20px; }
    h1 { color: #0275d8; }
    .grid { display: flex; flex-wrap: wrap; justify-content: center; gap: 15px; margin-top: 20px; }
    .card { background: white; padding: 20px; border-radius: 12px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 160px; }
    .card h3 { margin: 0 0 10px 0; color: #555; font-size: 16px; }
    .card p { margin: 0; font-size: 24px; font-weight: bold; color: #333; }
  </style>
</head>
<body>
  <h1>🌤️ Local Weather Station</h1>
  <p>Live ESP32 Dashboard</p>
  <div class="grid">
    <div class="card"><h3>Temperature</h3><p>24.2 °C</p></div>
    <div class="card"><h3>Humidity</h3><p>58.0 %</p></div>
    <div class="card"><h3>Pressure</h3><p>1012.4 hPa</p></div>
    <div class="card"><h3>Altitude</h3><p>420.1 m</p></div>
    <div class="card"><h3>Light Level</h3><p>65 %</p></div>
  </div>
</body>
</html>
```

