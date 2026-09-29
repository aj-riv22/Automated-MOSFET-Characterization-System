void setup() {
  // Initialize serial communication and wait up to 2.5 seconds for a connection
  Serial.begin(115200);
  for (auto startNow = millis() + 2500; !Serial && millis() < startNow; delay(500));
  
  // Set DAC resolution to 12-bit for maximum precision
  analogWriteResolution(12);
  
  Serial.println("- Arduino Nano R4 - DAC Basic Output Example started...");
  Serial.println("- Generating precise voltages on pin A0");
  Serial.println("- Connect a multimeter to A0 to measure output");
}

void loop() {
  // 51 steps from 0 V to 5 V, 0.1 V each step
  float dacValues[51] = {};
  float voltages[51] = {};
  
  for (int i = 0; i <= 50; i++) {
    voltages[i] = (0.1 * i);
    dacValues[i] = (0.1 * i * 4095) / 5.121; // mapping the voltage to a specific bit
    
    analogWrite(DAC, dacValues[i]);
    Serial.print("- DAC Value: ");
    Serial.print(dacValues[i]);
    Serial.print(" | Target Voltage: ");
    Serial.print(voltages[i]);
    Serial.println(" VDC");
    // Hold each voltage for 5 seconds
    delay(1);  
  }
  
  Serial.println("- Cycle completed, repeating...");
  delay(1000);
}
