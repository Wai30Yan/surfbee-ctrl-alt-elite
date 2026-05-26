const int potPin = A0;   // Analog pin connected to the potentiometer

// RGB LED Pins
const int RED_PIN = 4;
const int GREEN_PIN = 3;
const int BLUE_PIN = 2;

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.println("pH Sensor Simulation Started");
}

void loop() {
  int sensorValue;
  
  noInterrupts(); 
  sensorValue = analogRead(potPin);
  interrupts(); 

  // --- MATH CONVERSIONS ---
  // Convert raw analog reading (0-1023) to Voltage (0.0V - 5.0V)
  float voltage = (sensorValue * 5.0) / 1023.0;
  
  // Convert raw analog reading (0-1023) to pH scale (0.0 - 14.0)
  float pHValue = (sensorValue * 14.0) / 1023.0;

  // --- PRINT TO SERIAL MONITOR ---
  Serial.print("Voltage: ");
  Serial.print(voltage, 2);       // Prints voltage with 2 decimal places
  Serial.print("V  |  pH Value: ");
  Serial.println(pHValue, 2);     // Prints pH with 2 decimal places

  // Option 1 Logic: Hard steps for Red, Green, and Blue
  if (sensorValue < 341) {          // Low Range (0 to 340)
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  } 
  else if (sensorValue < 682) {     // Mid Range (341 to 681)
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  } 
  else {                            // High Range (682 to 1023)
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  }

  delay(250); // Increased slightly to 250ms so the Serial Monitor text doesn't scroll too fast to read
}