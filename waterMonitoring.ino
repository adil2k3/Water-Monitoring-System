#define BLYNK_TEMPLATE_ID "TMPL3GArgM2wY"
#define BLYNK_TEMPLATE_NAME "water quality"
#define BLYNK_AUTH_TOKEN "GkBwZSYKRY4JmfwGRimjD7m9Xh1h-tur"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char ssid[] = "OnePlus Nord CE3 5G";
char pass[] = "1234567899";

BlynkTimer timer;

// PINS
#define TDS_PIN 34
#define TURB_PIN 32
#define PH_PIN 35
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// VARIABLES
float temperature = 25.0;

float aref = 3.3;
float ec = 0;
float ec25 = 0;
float tdsRawCalc = 0;
int tds = 0;

float ecCalibration = 1.5;

// TDS SMOOTHING
float tdsFiltered = 0;
float alphaTDS = 0.2;

// TURBIDITY
float turbFiltered = 0;
float alpha = 0.2;

// PH
float pHFiltered = 7.0;
float alphaPH = 0.05;
float pHValue = 7.0;

String waterStatus = "Checking";

bool alertSent = false;

// LCD SWITCH
bool screenToggle = true;
unsigned long lastSwitch = 0;

void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  sensors.begin();

  lcd.setCursor(0, 0);
  lcd.print("Water Monitor");
  delay(2000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, readSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}

void readSensors() {

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  int tdsRaw = analogRead(TDS_PIN);
  int turbRaw = analogRead(TURB_PIN);
  int phRaw  = analogRead(PH_PIN);

  // ===== TDS =====
  float voltage = tdsRaw * aref / 4095.0;

  ec = (voltage * ecCalibration) - 0.14;
  if (ec < 0) ec = 0;

  ec25 = ec / (1 + 0.02 * (temperature - 25.0));

  tdsRawCalc = (133.42 * pow(ec25, 3) - 255.86 * ec25 * ec25 + 857.39 * ec25) * 0.5;

  tdsFiltered = (alphaTDS * tdsRawCalc) + ((1 - alphaTDS) * tdsFiltered);

  tds = (0.78 * tdsFiltered) + 40;

  if (tds < 0) tds = 0;

  // ===== TURBIDITY =====
  turbFiltered = (alpha * turbRaw) + ((1 - alpha) * turbFiltered);
  int turb = (int)turbFiltered;

  int ntu;

  if (turb >= 1800) {
    ntu = map(turb, 1800, 2200, 0, 5);
  }
  else if (turb >= 1400) {
    ntu = map(turb, 1400, 1799, 100, 5);
  }
  else {
    ntu = map(turb, 0, 1399, 3000, 100);
  }

  ntu = ntu * 0.2;   // (kept from previous working version)

  if (ntu < 0) ntu = 0;

  if (ntu < 5) {
    waterStatus = "Clear";
  }
  else if (ntu <= 100) {
    waterStatus = "Cloudy";
  }
  else {
    waterStatus = "Dirty";
  }

  // ===== PH =====
  float phVoltage = phRaw * 3.3 / 4095.0;
  float rawPH = 7 + ((2.5 - phVoltage) / 0.18);
  float calibratedPH = rawPH - 3.2;

  pHFiltered = (alphaPH * calibratedPH) + ((1 - alphaPH) * pHFiltered);
  pHValue = pHFiltered;

  // ===== BLYNK =====
  Blynk.virtualWrite(V0, tds);
  Blynk.virtualWrite(V1, ec25);
  Blynk.virtualWrite(V2, ntu);
  Blynk.virtualWrite(V4, waterStatus);
  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V6, pHValue);

  // ===== LCD SWITCH =====
  if (millis() - lastSwitch > 2000) {
    screenToggle = !screenToggle;
    lastSwitch = millis();
    lcd.clear();
  }

  if (screenToggle) {

    lcd.setCursor(0, 0);
    lcd.print("TDS:");
    lcd.print(tds);

    lcd.print(" pH:");
    lcd.print(pHValue, 1);

    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print((int)temperature);
    lcd.print("C ");

    lcd.print("TUR:");
    lcd.print(ntu);

  } else {

    lcd.setCursor(0, 0);
    lcd.print("Water Status");

    // ✅ FIXED SPLIT SCREEN
    lcd.setCursor(0, 1);
    lcd.print("                "); // clear line
    lcd.setCursor(0, 1);
    lcd.print(waterStatus);
  }

  // ===== ALERT =====
  if ((waterStatus == "Cloudy" || waterStatus == "Dirty") && pHValue > 8.7 && !alertSent) {

    Blynk.logEvent("poor_water_quality",
      "⚠️ Unsafe Water!\nStatus: " + waterStatus +
      "\nNTU: " + String(ntu) +
      "\npH: " + String(pHValue));

    alertSent = true;
  }

  if ((waterStatus == "Clear") || pHValue <= 8.7) {
    alertSent = false;
  }

  Serial.print("TDS: ");
  Serial.print(tds);
  Serial.print(" | TUR: ");
  Serial.print(ntu);
  Serial.print(" | pH: ");
  Serial.println(pHValue);
}