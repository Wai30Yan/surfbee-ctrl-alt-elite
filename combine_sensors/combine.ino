#include "CQRobotTDS.h"
#include <Wire.h>

// Updated to map to your 4-Potentiometer hardware setup
const int pHSensorPin = A0; 
const int tdsSensorPin = A3; 
const int ldrTurbidityPin = A4; // NEW: Wired to your new Turbidity Dial
const int ntcSensorPin = A5;    // NEW: Wired to your new Temp Dial

// pH LED
const int red_pH = 2;
const int green_pH = 3;

// Turbidity LED
const int red_turb = 4;
const int green_turb = 5;

// Temperature LED
const int red_temp = 6;
const int green_temp = 7;

// TDS LED
const int red_tds = 8;
const int green_tds = 9;

// TDS sensor simulation via CQRobotTDS library
CQRobotTDS tds(tdsSensorPin);

unsigned long printTimeout = 0;

// Helper to control 2-pin common cathode RGB states (Red, Green, Yellow)
void setRGB(int redPin, int greenPin, String color) {
  if (color == "GREEN") {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  } else if (color == "YELLOW") {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH); // Red + Green mixed = Yellow
  } else if (color == "RED") {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
  }
}

void setup() {
  Serial.begin(9600); 
  
  pinMode(red_pH, OUTPUT);   pinMode(green_pH, OUTPUT);
  pinMode(red_turb, OUTPUT);  pinMode(green_turb, OUTPUT);
  pinMode(red_temp, OUTPUT);  pinMode(green_temp, OUTPUT);
  pinMode(red_tds, OUTPUT);   pinMode(green_tds, OUTPUT);
  
  Serial.println("--- 4-Channel Dashboard Active ---");
}

void loop() {
  // 1. pH Sensor Simulation (0.0 to 14.0)
  int sensorValue_pH = analogRead(pHSensorPin);
  float actual_pH = sensorValue_pH * 14.0 / 1023.0; 

  // 2. Temperature Sensor Simulation (0°C to 70°C)
  int analogValue_ntc = analogRead(ntcSensorPin);
  float celsius = analogValue_ntc * 70.0 / 1023.0;

  // 3. TDS Sensor Simulation (Reads via CQRobot library calculation)
  float tdsValue = tds.update(celsius);

  // 4. Turbidity Sensor Simulation (0% to 100%)
  int analogValue_ldr = analogRead(ldrTurbidityPin);
  float turbidityPercent = analogValue_ldr * 100.0 / 1023.0;

  // --- EVALUATION COLOR LOGIC BASED ON YOUR SPECIFICATIONS ---
  
  // pH LED status (Matches the chart parameters)
  if (actual_pH >= 6.8 && actual_pH <= 8.2) {
    setRGB(red_pH, green_pH, "GREEN"); // Safe zone (7.0, 7.5, 8.0)
  } else if ((actual_pH >= 6.3 && actual_pH < 6.8) || (actual_pH > 8.2 && actual_pH <= 8.8)) {
    setRGB(red_pH, green_pH, "YELLOW"); // Warning zones (6.5 and 8.5)
  } else {
    setRGB(red_pH, green_pH, "RED"); // Danger zones (6.0- and 9.0+)
  }

  // TDS LED status (Green <= 300, Yellow = 301-500, Red > 500)
  if (tdsValue <= 300.0) {
    setRGB(red_tds, green_tds, "GREEN");
  } else if (tdsValue > 300.0 && tdsValue <= 500.0) {
    setRGB(red_tds, green_tds, "YELLOW");
  } else {
    setRGB(red_tds, green_tds, "RED");
  }

  // Turbidity LED status (0-33% Low, 34-66% Med, 67-100% High)
  if (turbidityPercent <= 33.0) {
    setRGB(red_turb, green_turb, "GREEN");
  } else if (turbidityPercent > 33.0 && turbidityPercent <= 66.0) {
    setRGB(red_turb, green_turb, "YELLOW");
  } else {
    setRGB(red_turb, green_turb, "RED");
  }

  // Temperature LED status (Below 18 Green, 18-45 Yellow, Above 45 Red)
  if (celsius < 18.0) {
    setRGB(red_temp, green_temp, "GREEN");
  } else if (celsius >= 18.0 && celsius <= 45.0) {
    setRGB(red_temp, green_temp, "YELLOW");
  } else {
    setRGB(red_temp, green_temp, "RED");
  }

  // Safe Drinking Water Global Check
  bool isClean = (actual_pH >= 6.8 && actual_pH <= 8.2) && (tdsValue <= 500.0) && (turbidityPercent <= 33.0) && (celsius <= 25.0);

  // Print Interval Handler
  if (millis() > printTimeout) {
    Serial.println("--- WATER MONITORING REPORT ---");
    Serial.print("pH Value: "); Serial.println(actual_pH, 1);
    Serial.print("TDS Content: "); Serial.print(tdsValue, 0); Serial.println(" ppm");
    Serial.print("Turbidity Level: "); Serial.print(turbidityPercent, 0); Serial.println("%");
    Serial.print("Temperature: "); Serial.print(celsius, 1); Serial.println(" °C");
    
    Serial.println("\n--- FINAL EVALUATION ---");
    if (isClean) {
      Serial.println("Overall Condition: WATER IS SAFE TO DRINK");
    } else {
      Serial.println("Overall Condition: WATER IS UNCLEAN / UNHEALTHY");
      Serial.println("Alerts:");
      if (actual_pH < 6.8) Serial.println(" - Target pH Alert: Water is too Acidic.");
      if (actual_pH > 8.2) Serial.println(" - Target pH Alert: Water is too Alkaline.");
      if (tdsValue > 500.0) Serial.println(" - Contaminant Alert: Total Dissolved Solids are too high.");
      if (turbidityPercent > 33.0) Serial.println(" - Clarity Alert: Water turbidity is hazy or dirty.");
      if (celsius > 25.0) Serial.println(" - Thermal Alert: Temperature exceeds cool drinking standards.");
    }
    Serial.println("--------------------------------------------\n");
    
    printTimeout = millis() + 2000; // Update logs cleanly every 2 seconds
  }
}