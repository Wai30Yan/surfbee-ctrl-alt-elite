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
  Serial.println("--- pH Monitor with Safety Classification Active ---");
}

void loop() {
  int rawValue = analogRead(POT_PIN);
  
  // Convert 0-1023 to 0.0 - 14.0 pH scale
  float pH = rawValue * 14.0 / 1023.0;
  
  // Print the raw pH value first
  Serial.print("pH Value: "); 
  Serial.print(pH, 1);
  Serial.print(" -> Status: ");

  // --- COLOR & CLASSIFICATION LOGIC ---
  
  if (pH < 6.5) {
    // Red for acidic
    analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 0); analogWrite(BLUE_PIN, 0); 
    Serial.println("Acidic");
  } 
  else if (pH >= 6.5 && pH <= 8.2) {
    // Green for safe drinking water (Matches your 7.5 current target)
    analogWrite(RED_PIN, 0); analogWrite(GREEN_PIN, 255); analogWrite(BLUE_PIN, 0); 
    Serial.println("Safe for drinking");
  } 
  else if (pH > 8.2 && pH <= 8.8) {
    // Yellow for warning / mildly alkaline
    analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 200); analogWrite(BLUE_PIN, 0); 
    Serial.println("Alkaline (Base Warning)");
  } 
  else {
    // Blue for highly alkaline
    analogWrite(RED_PIN, 0); analogWrite(GREEN_PIN, 0); analogWrite(BLUE_PIN, 255); 
    Serial.println("Alkaline (Base)");
  }

  delay(200); 
}