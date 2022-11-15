#include "luminosity.h"
#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"

float currentLux;
float lastLux = 0;
xTaskHandle LUMINOSITY_HANDLE;

float read_lux() {
  int analogValue = analogRead(LDR_PIN);
  //float voltage = analogValue * VOLTAGE_COEFFICIENT;
  //float resistance = SENSOR_RESISTANCE * voltage / (1 - voltage / VIN);
  //float lux = pow(RL10 * pow(10, GAMMA) / resistance, INVERSE_GAMMA);
  
  return analogValue / 2;
}

void taskCheckLux(void *params) {
    while (1) {
        currentLux = read_lux();

        if(abs(currentLux - lastLux) > SENSIBILIDADE_LUMINOSIDADE) {
            loginfo("Luminosity: %.2f lx", currentLux);
            setLed(currentLux < MIN_LUX);

            lastLux = currentLux;
        }

        vTaskDelay(pdMS_TO_TICKS(LUX_DELAY_MS));
    }
}

void resumeLuminosityTask() {
    vTaskResume(LUMINOSITY_HANDLE);
}

void suspendLuminosityTask() {
    vTaskSuspend(LUMINOSITY_HANDLE);
}

void initLux() {
    pinMode(LDR_PIN, INPUT);
    currentLux = read_lux();

    xTaskCreatePinnedToCore(taskCheckLux, "taskCheckLux", LUX_STACK_DEPTH, NULL, LUX_TASK_PRIORITY, &LUMINOSITY_HANDLE
, APP_CPU_NUM);
}