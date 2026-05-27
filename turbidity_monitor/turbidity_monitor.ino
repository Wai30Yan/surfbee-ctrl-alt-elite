// Hardware Pin Definitions
const int POT_PIN = A0;   // Single potentiometer input
const int RED_PIN = 9;    // RGB Red Pin (PWM)
const int GREEN_PIN = 10; // RGB Green Pin (PWM)
const int BLUE_PIN = 11;  // RGB Blue Pin (PWM)

void setup() {
  // Configure RGB pins as outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("--- Turbidity Monitor Active ---");
}

void loop() {
  // Read raw value from the single potentiometer (0 to 1023)
  int rawValue = analogRead(POT_PIN);
  
  // Convert the 0-1023 analog range to a 0 - 100% scale
  float turbidity = rawValue * 100.0 / 1023.0;
  
  // Print current status to the Serial Monitor
  Serial.print("Turbidity Level: "); 
  Serial.print(turbidity, 0); 
  Serial.print("% -> ");

  // --- COLOR SELECTION LOGIC ---
  
  // 1. Low Turbidity: Green
  if (turbidity <= 33.0) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
    Serial.println("LOW [GREEN]");
  } 
  // 2. Medium Turbidity: Yellow (Red + Green mixed)
  else if (turbidity > 33.0 && turbidity <= 66.0) {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 200); // Shifting slightly below 255 gives a richer yellow
    analogWrite(BLUE_PIN, 0);
    Serial.println("MEDIUM [YELLOW]");
  } 
  // 3. High Turbidity: Red
  else {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    Serial.println("HIGH [RED]");
  }

  delay(200); // Short delay to prevent serial flooding
}