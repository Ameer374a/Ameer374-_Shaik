// Pin Definitions
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
const int flowSensorPin = 2;   // Water flow sensor connected to digital pin 2 (interrupt pin)
const int buzzerPin = 13;       // Buzzer connected to digital pin 8
const int relayPin = 9;        // Relay connected to digital pin 9

// Variables
volatile int pulseCount = 0;   // Variable to count water flow sensor pulses
float calibrationFactor = 4.5; // Calibration factor for flow sensor (adjust based on your sensor's specs)
float flowRate;                // Flow rate in liters per minute (L/min)
unsigned long oldTime = 0;     // Variable to store the last time the flow was measured

void setup() {
  // Set up the pins
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  pinMode(flowSensorPin, INPUT);  // Water flow sensor as input
  pinMode(buzzerPin, OUTPUT);     // Buzzer as output
  pinMode(relayPin, OUTPUT);      // Relay as output
lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("welcome to ");
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("water Detection System");
  delay(1000);
  lcd.clear();
  digitalWrite(relayPin, LOW);    // Ensure relay starts off
  digitalWrite(buzzerPin, LOW);   // Buzzer off initially

  // Attach an interrupt to the water flow sensor pin
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), countPulses, RISING);

  Serial.begin(9600);  // Start serial communication for debugging
}

void loop() {
  // Measure water flow every second (1000 ms)
  if (millis() - oldTime > 1000) {
    // Calculate flow rate (in liters/minute)
    flowRate = (pulseCount / calibrationFactor);  // Calculate flow rate based on pulses and calibration factor

    // Print the flow rate to the serial monitor
    Serial.print("Flow Rate: ");
    lcd.print("Flow Rate: ");
    Serial.print(flowRate);
    lcd.print(flowRate);
    Serial.println(" L/min");
lcd.println(" L/min");
delay(2000);
lcd.clear();
    // Reset pulse count for the next second
    pulseCount = 0;
    oldTime = millis();

    // Check if the flow rate exceeds a threshold (e.g., 2 L/min)
    if (flowRate > 2.0) {
      triggerAlarm();  // Activate relay and buzzer if the flow exceeds the threshold
    } else {
      resetAlarm();    // Deactivate relay and buzzer if flow rate is below threshold
    }
  }
}

void countPulses() {
  // Increment the pulse count whenever a pulse is detected from the sensor
  pulseCount++;
}

void triggerAlarm() {
  digitalWrite(relayPin, HIGH);  // Activate relay
  digitalWrite(buzzerPin, HIGH);
  delay(1000); // Turn on buzzer
  digitalWrite(buzzerPin, LOW);
}

void resetAlarm() {
  digitalWrite(relayPin, LOW);   // Deactivate relay
  digitalWrite(buzzerPin, LOW);  // Turn off buzzer
}
