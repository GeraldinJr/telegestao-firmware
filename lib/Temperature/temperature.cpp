#include "temperature.h"
#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"
#include "../TaskManagement/taskManagement.h"

float currentTemperature;
float lastTemperature = 0;
xTaskHandle TEMPERATURE_HANDLE;

float readTemperature() {
  float analogValue = (float) analogRead(TEMPERATURE_PIN);
  //float celsius = 1 / (log(1 / (MAX_ANALOG / analogValue - 1.0)) / BETA + 1.0 / 298.15) - 273.15;
  return (analogValue / 40.0) - 25.0;
}

void taskCheckTemperature(void *params) {
    while (1) {
        currentTemperature = readTemperature();

        if(abs(currentTemperature - lastTemperature) > SENSIBILITY) {
            lastTemperature = currentTemperature;

            if(( currentTemperature > MAX_TEMPERATURE || currentTemperature < MIN_TEMPERATURE)) {
                apagaLed();
                suspendAllByTemperature();

                do {
                    logwarn("CRITICAL TEMPERATURE: %2.f°C", currentTemperature);
                    currentTemperature = readTemperature();
                    vTaskDelay(pdMS_TO_TICKS(TEMPERATURE_DELAY_MS));

                } while (( currentTemperature > MAX_TEMPERATURE || currentTemperature < MIN_TEMPERATURE));

                resumeAllByTemperature();
                
            }
        }

        vTaskDelay(pdMS_TO_TICKS(TEMPERATURE_DELAY_MS));
    }  
}

void resumeTemperatureTask() {
    vTaskResume(TEMPERATURE_HANDLE);
}

void suspendTemperatudeTask() {
    vTaskSuspend(TEMPERATURE_HANDLE);
}

float getTemperature() {
    return readTemperature();
}

void initTemperature() {
    pinMode(TEMPERATURE_PIN, INPUT);
    currentTemperature = readTemperature();

    xTaskCreatePinnedToCore(taskCheckTemperature, "taskCheckTemperature", TEMPERATURE_STACK_DEPTH, NULL, TEMPERATURE_TASK_PRIORITY, &TEMPERATURE_HANDLE, APP_CPU_NUM);
}