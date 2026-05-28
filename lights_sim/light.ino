// RGB LED 1: pH Pins
const int PH_RED = 2;
const int PH_GREEN = 3;
const int PH_BLUE = 4;

// RGB LED 2: TDS Pins
const int TDS_RED = 5;
const int TDS_GREEN = 6;
const int TDS_BLUE = 7;

// RGB LED 3: Turbidity Pins
const int TURB_RED = 8;
const int TURB_GREEN = 9;
const int TURB_BLUE = 10;

// RGB LED 4: Temperature Pins
const int TEMP_RED = 11;
const int TEMP_GREEN = 12;
const int TEMP_BLUE = 13;

// Hardcoded Simulation Data Cycles (2 seconds per cycle)
const int TOTAL_CYCLES = 5;
float phData[TOTAL_CYCLES]   = {5.8,   6.5,   7.0,   8.5,   9.0};  
float tdsData[TOTAL_CYCLES]  = {250.0, 300.0, 500.0, 850.0, 400.0}; 
float turbData[TOTAL_CYCLES] = {50.0,  85.0,  15.0, 20.0, 10.0};  // 0-33 Low, 34-66 Med, 67+ High
float tempData[TOTAL_CYCLES] = {20.0, 12.0,  15.0,  27.0,  10.0};  

int currentCycle = 0;

void setup() {
  // Set all 12 pins as outputs
  for (int pin = 2; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }
  
  Serial.begin(9600);
  Serial.println("--- 4-Channel Water Quality Dashboard Active ---");
}

void loop() {
  float currentPH = phData[currentCycle];
  float currentTDS = tdsData[currentCycle];
  float currentTurb = turbData[currentCycle];
  float currentTemp = tempData[currentCycle];

  // Serial Diagnostics Output
  Serial.print("\n[CYCLE "); Serial.print(currentCycle + 1); Serial.println("]");
  Serial.print("pH: "); Serial.print(currentPH, 1);
  Serial.print(" | TDS: "); Serial.print(currentTDS, 0);
  Serial.print(" | Turb: "); Serial.print(currentTurb, 0); Serial.print("%");
  Serial.print(" | Temp: "); Serial.print(currentTemp, 1); Serial.println("°C");

  // ----------------------------------------------------
  // 1. pH LED LOGIC (Red <= 6.0, Yellow = 6.5 & 8.5, Green = 7.0-8.0, Blue >= 9.0)
  // ----------------------------------------------------
  if (currentPH <= 6.2) {
    updateRGB(PH_RED, PH_GREEN, PH_BLUE, 255, 0, 0);    
  } else if (currentPH > 6.2 && currentPH < 6.8) {
    updateRGB(PH_RED, PH_GREEN, PH_BLUE, 255, 200, 0);  
  } else if (currentPH >= 6.8 && currentPH <= 8.2) {
    updateRGB(PH_RED, PH_GREEN, PH_BLUE, 0, 255, 0);     
  } else if (currentPH > 8.2 && currentPH < 8.8) {
    updateRGB(PH_RED, PH_GREEN, PH_BLUE, 255, 200, 0);  
  } else {
    updateRGB(PH_RED, PH_GREEN, PH_BLUE, 0, 0, 255);    
  }

  // ----------------------------------------------------
  // 2. TDS LED LOGIC (Green <= 300, Yellow = 500, Red >= 800)
  // ----------------------------------------------------
  if (currentTDS <= 400.0) {
    updateRGB(TDS_RED, TDS_GREEN, TDS_BLUE, 0, 255, 0);  
  } else if (currentTDS > 400.0 && currentTDS < 650.0) {
    updateRGB(TDS_RED, TDS_GREEN, TDS_BLUE, 255, 200, 0);
  } else {
    updateRGB(TDS_RED, TDS_GREEN, TDS_BLUE, 255, 0, 0);  
  }

  // ----------------------------------------------------
  // 3. TURBIDITY LED LOGIC (Green = Low, Yellow = Med, Red = High)
  // ----------------------------------------------------
  if (currentTurb <= 33.0) {
    updateRGB(TURB_RED, TURB_GREEN, TURB_BLUE, 0, 255, 0);  
  } else if (currentTurb > 33.0 && currentTurb <= 66.0) {
    updateRGB(TURB_RED, TURB_GREEN, TURB_BLUE, 255, 200, 0); 
  } else {
    updateRGB(TURB_RED, TURB_GREEN, TURB_BLUE, 255, 0, 0);   
  }

  // ----------------------------------------------------
  // 4. TEMPERATURE LED LOGIC (Green <= 17°C, Yellow = 18-23°C, Red >= 24°C)
  // ----------------------------------------------------
  if (currentTemp < 18.0) {
    updateRGB(TEMP_RED, TEMP_GREEN, TEMP_BLUE, 0, 255, 0);  
  } else if (currentTemp >= 18.0 && currentTemp <= 23.0) {
    updateRGB(TEMP_RED, TEMP_GREEN, TEMP_BLUE, 255, 200, 0); 
  } else {
    updateRGB(TEMP_RED, TEMP_GREEN, TEMP_BLUE, 255, 0, 0);  
  }

  // Cycle loop sequence index
  currentCycle = (currentCycle + 1) % TOTAL_CYCLES;
  delay(2000); 
}

void updateRGB(int rPin, int gPin, int bPin, int rVal, int gVal, int bVal) {
  analogWrite(rPin, rVal);
  analogWrite(gPin, gVal);
  analogWrite(bPin, bVal);
}