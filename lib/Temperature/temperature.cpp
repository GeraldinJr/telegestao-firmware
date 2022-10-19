#include "temperature.h"

float currentTemperature;
float lastTemperature = 0;

float readTemperature() {
  int analogValue = analogRead(TEMPERATURE_PIN);
  float celsius = 1 / (log(1 / (MAX_ANALOG / analogValue - 1.0)) / BETA + 1.0 / 298.15) - 273.15;
  return celsius;
}

void taskCheckTemperature(void *params) {
    while (1) {
        currentTemperature = readTemperature();

        if(abs(currentTemperature - lastTemperature) > SENSIBILITY) {
            lastTemperature = currentTemperature;
            loginfo("Temperature: %.2f", currentTemperature);

            if(( currentTemperature > MAX_TEMPERATURE || currentTemperature < MIN_TEMPERATURE)) {
                setLed(LOW);
            }
        }
    }  
}

void initTemperature() {
    pinMode(TEMPERATURE_PIN, INPUT);
    currentTemperature = readTemperature();

    xTaskCreatePinnedToCore(taskCheckTemperature, "taskCheckTemperature", STACK_DEPTH, NULL, TASK_PRIORITY, NULL, APP_CPU_NUM);
}