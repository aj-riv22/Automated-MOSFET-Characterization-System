
#include <Arduino.h>

bool initialized = false;
bool ledOn = false;
unsigned long lastToggle = 0;
const int LED_PIN = LED_BUILTIN;
const unsigned long HALF_PERIOD_US = 500;

void setRunningState(bool running)
{
    initialized = running;

    if (!running)
    {
        ledOn = false;
        digitalWrite(LED_PIN, LOW);
    }
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(115200);
}

void loop()
{
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command.length() == 0)
        {
            return;
        }

        char cmd = command.charAt(0);

        if (cmd == 'S' || cmd == 's')
        {
            setRunningState(true);
            lastToggle = micros();
            Serial.println("START");
        }
        else if (cmd == 'O' || cmd == 'o' || cmd == '0' || cmd == 'F' || cmd == 'f' || cmd == 'X' || cmd == 'x')
        {
            setRunningState(false);
            Serial.println("OFF");
        }
        else
        {
            // Any other value turns the Arduino off.
            setRunningState(false);
            Serial.print("UNKNOWN_CMD:");
            Serial.println(command);
        }
    }

    if (initialized)
    {
        unsigned long currentTime = micros();

        if ((unsigned long)(currentTime - lastToggle) >= HALF_PERIOD_US)
        {
            lastToggle += HALF_PERIOD_US;
            ledOn = !ledOn;
            digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
        }
    }
}

