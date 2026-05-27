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
  Serial.println("--- Temperature Monitor Active ---");
}

void loop() {
  int rawValue = analogRead(POT_PIN);
  
  // Convert 0-1023 to 0°C - 40°C range
  float temp = rawValue * 40.0 / 1023.0;
  
  Serial.print("Temperature: "); Serial.print(temp, 1); Serial.println("°C");

  // Chart Logic: Cold (Green), Warm (Yellow), Hot (Red)
  if (temp <= 17.0) {
    // Green (Cold / 10°C - 15°C)
    analogWrite(RED_PIN, 0); analogWrite(GREEN_PIN, 255); analogWrite(BLUE_PIN, 0); 
  } 
  else if (temp > 17.0 && temp <= 23.0) {
    // Yellow (Medium / 20°C)
    analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 200); analogWrite(BLUE_PIN, 0); 
  } 
  else {
    // Red (Hot / 25°C+)
    analogWrite(RED_PIN, 255); analogWrite(GREEN_PIN, 0); analogWrite(BLUE_PIN, 0); 
  }

  delay(200); 
}