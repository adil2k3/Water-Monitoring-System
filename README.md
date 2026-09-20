# Water Monitoring System

## Team: Purelytics

---

## About
This is a simple water monitoring system using ESP32 and sensors. It reads water quality data and shows it in real time on LCD and mobile app.

The goal is to make water checking simple and low cost.

---

## What it does
- Reads pH, TDS, turbidity, and temperature  
- Shows values on LCD screen  
- Sends data to Blynk app  
- Shows water status (Clear / Cloudy / Dirty)  
- Gives alert when water is not safe  

---

## Hardware Used
- ESP32  
- pH sensor  
- TDS sensor  
- Turbidity sensor  
- DS18B20 temperature sensor  
- 16x2 LCD  

---

## Files in this project

### Arduino Code
Main code for reading sensors and showing output.

👉 [waterMonitoring.ino](waterMonitoring.ino)

---

### Circuit Architecture
Circuit design showing how everything is connected.

👉 [Circuit PDF](Circuit_architecture.pdf)  
👉 [CAD File](CADfile_circuitarchitecture.dwg)

---

### Bill of Materials (BOM)
List of all components used in the project.

👉 [BOM File](BOM.pdf)

---

## How it works
1. Sensors read water data  
2. ESP32 processes the data  
3. LCD shows values  
4. Blynk app shows live data  
5. Alert is given if water is unsafe  

---

## Team
Purelytics  
