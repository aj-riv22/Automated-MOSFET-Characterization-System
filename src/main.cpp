#include <Arduino.h>
#include <Wire.h>              // Library for I2C communication
#include <Adafruit_MCP4725.h>  // Library for MCP4725 12-bit DAC

bool completed = false;
const int ina213_out = A1;
const int drainVoltage = A2;
Adafruit_MCP4725 dac_drain;

void setup() {
  // initialize serial communication and wait up to 2.5 seconds for a connection
  Serial.begin(115200); 
  for (auto startNow = millis() + 2500; !Serial && millis() < startNow; delay(500));
  
  analogWriteResolution(12);  // set DAC resolution to 12-bit
  analogReadResolution(14); // set ADC resolution to 14 bit
  Wire.begin();                // initialize standard I2C bus (A4 = SDA, A5 = SCL)
  dac_drain.begin(0x62, &Wire);      // 0x62 = MCP4725 I2C address
  dac_drain.setVoltage(0, false); // initialize external dac to 0 V
}

void loop() {
  analogWrite(DAC, 0); // set internal DAC to 0 V initially
  int samples = 201;
  float step_size = 0.025; 
  float vcc = 5.121;
  float gateBit[samples] = {}; // 0.025 V increments
  float gateVoltage[samples] = {};
  float drainCurrent[samples] = {};
  float ina213_gain = 50.02; // gain of the current amplifier
  float drain_amp_gain = 2.011423; // gain of the amplifier for the drain voltage
  
  while (!completed) {
    dac_drain.setVoltage(1999, false); 
    delay(5000);
    for (int i = 0; i < samples; i++) {
      gateVoltage[i] = (step_size * i);
      gateBit[i] = (gateVoltage[i] * 4095) / vcc; // mapping the voltage to a specific bit in the dac
      analogWrite(DAC, gateBit[i]);

      delay(10); // delay between writing and reading. giving time for the voltage to settle

      drainCurrent[i] = analogRead(ina213_out) * (vcc / 16383.0 ) * (1 / ina213_gain); 

      Serial.print(gateVoltage[i], 5);
      Serial.print(",");
      Serial.print(drainCurrent[i], 5);
      Serial.print(",");
      Serial.print(analogRead(drainVoltage) * (vcc / 16383.0 ) * drain_amp_gain, 5);
      Serial.println();

      if(drainCurrent[i] > 0.1) { // break out once drain current is greater than 100 mA
        gateVoltage[i] = 0; // assign gate voltage to 0
        dac_drain.setVoltage(0, false); // assign drain voltage to 0
        completed = true;
        break;
      }
      delay(10); // delay between next iteration
    }
  }
}
