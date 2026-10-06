#include <Arduino.h>

const int analogReadPin = A1;
void writeAndReadVoltage(float writeVoltage[], float readVoltage[], int numSteps, int timeDelay); // fnction to write a sequence of voltages to the DAC and read the corresponding sensed voltages

void setup() {
  // Initialize serial communication and wait up to 2.5 seconds for a connection
  Serial.begin(115200);
  for (auto startNow = millis() + 2500; !Serial && millis() < startNow; delay(500));
  
  analogWriteResolution(12);  // set DAC resolution to 12-bit
  analogReadResolution(14); // set analog read resolution to 14 bit
}

void loop() {
  float writeVoltage[51] = {};
  float readVoltage[51] = {};
  writeAndReadVoltage(writeVoltage, readVoltage, 51, 10); // call the function to write and read voltages

  // print the collected voltages for verification
  for (int i = 0; i < 51; i++) {
    Serial.print(writeVoltage[i], 3);
    Serial.print(",");
    Serial.print(readVoltage[i], 3);
    Serial.println();
  }
  delay(1000); // delay between loop iterations

}

void writeAndReadVoltage(float writeVoltage[], float readVoltage[], int numSteps, int timeDelay) {
  for (int i = 0; i < numSteps; i++) {
    writeVoltage[i] = (0.1 * i);
    float writeBit = (writeVoltage[i] * 4095) / 5.121; // mapping the voltage to a specific bit in the dac
    analogWrite(DAC, writeBit);

    delay(timeDelay); // delay between writing and reading. giving time for the voltage to settle

    int readBit = analogRead(analogReadPin);
    readVoltage[i] = readBit * (5.121 / 16383.0); // mapping the bit read to a corresponding voltage
    delay(timeDelay); // delay between next iteration
  }
}

// create function for exporting data

//create function for finding the threshold voltage of the MOSFET

//create function for finding the kn parameter of the MOSFET

