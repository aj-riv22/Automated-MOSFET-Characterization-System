#include <Arduino.h>

const int analogReadPin = A1;

void setup() {
  // Initialize serial communication and wait up to 2.5 seconds for a connection
  Serial.begin(115200);
  for (auto startNow = millis() + 2500; !Serial && millis() < startNow; delay(500));
  
  analogWriteResolution(12);  // set DAC resolution to 12-bit
  analogReadResolution(14); // set analog read resolution to 14 bit
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

    delay(10); // delay between writing and reading. giving time for the voltage to settle

    int readBit = analogRead(analogReadPin);
    readVoltage[i] = readBit * (5.121 / 16383.0); // mapping the bit read to a corresponding voltage
    
    // print the applied and sensed voltages in the format "applied_voltage,sensed_voltage"
    Serial.print(writeVoltage[i], 3);
    Serial.print(",");
    Serial.print(readVoltage[i], 3);
    Serial.println();

    delay(500); // delay between next iteration
  }
}

// create function for writing and reading voltage

// create function for exporting data

