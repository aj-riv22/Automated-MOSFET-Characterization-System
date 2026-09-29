#include <Arduino.h>

const int analogReadPin = A1;

void setup() {
  // Initialize serial communication and wait up to 2.5 seconds for a connection
  Serial.begin(115200);
  for (auto startNow = millis() + 2500; !Serial && millis() < startNow; delay(500));
  
  // Set DAC resolution to 12-bit for maximum precision
  analogWriteResolution(12);

  // set analog read resolution to 14 bit
  analogReadResolution(14);
  
  Serial.println("Writing voltage from A0");
  Serial.println("Wire A0 pin and A1 pin together, to have A1 pin read in voltages from A0");
}

void loop() {
  // write
  float writeBit[51] = {}; // 51 steps from 0 V to 5 V, 0.1 V increments
  float writeVoltage[51] = {};

  // read
  float readVoltage[51] = {};
  
  for (int i = 0; i <= 50; i++) {
    writeVoltage[i] = (0.1 * i);
    writeBit[i] = (writeVoltage[i] * 4095) / 5.121; // mapping the voltage to a specific bit in the dac
    analogWrite(DAC, writeBit[i]);

    delay(10); // delay between writing and reading. time to settle

    int readBit = analogRead(analogReadPin);
    readVoltage[i] = readBit * (5.121 / 16383.0);

    //input / write values
    Serial.print("- DAC Value: ");
    Serial.print(writeBit[i]);
    Serial.print(" | Applied Voltage: ");
    Serial.print(writeVoltage[i]);
    Serial.println(" VDC");
    
    // output / read values
    Serial.print("- Read Bit Value: ");
    Serial.print(readBit);
    Serial.print(" | Sensed Voltage: ");
    Serial.print(readVoltage[i], 3);
    Serial.println(" VDC");
    
    delay(5000);  
  }

  Serial.println("- Cycle completed, repeating...");
  delay(1000);
}

// create function for writing and reading voltage

// create function for exporting data