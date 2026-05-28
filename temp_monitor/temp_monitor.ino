// Hardware Pins
const int POT_PIN = A0;
const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("--- Temperature Monitor Active (0-70C Scale) ---");
}

void loop() {
  int rawValue = analogRead(POT_PIN);
  
  // UPDATED: Convert 0-1023 to 0°C - 70°C scale range
  float temp = rawValue * 70.0 / 1023.0;
  
  Serial.print("Temperature: "); Serial.print(temp, 1); Serial.println("°C");

  // --- UPDATED TEMPERATURE LOGIC ---
  
  if (currentTemp < 18.0) {
    updateRGB(TEMP_RED, TEMP_GREEN, TEMP_BLUE, 0, 255, 0);   // Green (10°C, 15°C)
  } else if (currentTemp >= 18.0 && currentTemp <= 23.0) {
    updateRGB(TEMP_RED, TEMP_GREEN, TEMP_BLUE, 255, 200, 0); // Yellow (20°C)
  } else {
    updateRGB(TEMP_RED, TEMP_GREEN, TEMP_BLUE, 255, 0, 0);   // Red (25°C+)
  }

  delay(200); 
}