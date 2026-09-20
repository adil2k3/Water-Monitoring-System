# Water Monitoring System

## Team: Purelytics

## About the Project

This project is a simple water monitoring system built using ESP32 and sensors. It checks basic water quality parameters and shows the readings in real time. The idea was to build something low-cost that can give quick information about water condition.

---

## What it does

* Reads pH, TDS, turbidity and temperature
* Displays values on an LCD screen
* Sends data to the Blynk app
* Shows water condition as Clear / Cloudy / Dirty
* Gives alert if water is not safe

---

## Components Used

* ESP32
* pH sensor
* TDS sensor
* Turbidity sensor
* DS18B20 temperature sensor
* 16x2 LCD (I2C)

---

## Code

The Arduino code in this repository handles:

* Sensor readings
* Basic calibration
* Water condition logic
* LCD display
* Sending data to Blynk

---

## How to run

1. Open the .ino file in Arduino IDE
2. Install required libraries (Blynk, OneWire, etc.)
3. Select ESP32 board
4. Upload the code
5. Check output on LCD or Blynk

---

## Files in this repo

* Arduino code (.ino)
* CAD design file
* Design image (for preview)

---

## Note

This is a prototype built for learning and project purposes. It is not tested for large-scale real-world use yet.

---

## Made by

Team Purelytics
