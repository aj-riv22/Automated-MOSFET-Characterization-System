#include <Arduino.h>

// Analog input pin
const int analogPin = A0;

void setup() {
  // Initialize serial communication and wait up to 2.5 seconds for a connection
  Serial.begin(115200);
  for (auto startNow = millis() + 2500; !Serial && millis() < startNow; delay(500));
  
  // Set analog read resolution to 14-bit (0 - 16383)
  analogReadResolution(14);
  
}

void loop() {
  // Read the analog value (0 - 16383 with 14-bit resolution)
  int analogValue = analogRead(analogPin);
  
  // Convert to voltage (0 - +5 VDC)
  float voltage = analogValue * (5.121 / 16383.0);
  
  // Calculate percentage (0 - 100%)
  float percentage = (analogValue / 16383.0) * 100.0;
  
  // Display the results
  Serial.print("- Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" VDC | Percentage: ");
  Serial.print(percentage, 1);
  Serial.println(" %");
  
  // Wait half a second before next reading
  delay(500);
}