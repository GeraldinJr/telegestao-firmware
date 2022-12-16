#include "luminosity.h"
#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"

float currentLux;
float lastLux = 0;
xTaskHandle LUMINOSITY_HANDLE;

float read_lux() {
  float analogValue = (float) analogRead(LDR_PIN);
  //float voltage = analogValue * VOLTAGE_COEFFICIENT;
  //float resistance = SENSOR_RESISTANCE * voltage / (1 - voltage / VIN);
  //float lux = pow(RL10 * pow(10, GAMMA) / resistance, INVERSE_GAMMA);
  
  return analogValue / 2.0;
}

int getLedbyLux() {
    if (currentLux > MIN_LUX) {
        return 0;
    }

    return -70*currentLux/500+100;
}

void taskCheckLux(void *params) {
    while (1) {
        currentLux = read_lux();

        if(abs(currentLux - lastLux) > SENSIBILIDADE_LUMINOSIDADE) {
            int led = getLedbyLux();
            setLed(led);

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

float getLux() {
    return read_lux();
}

void initLux() {
    pinMode(LDR_PIN, INPUT);
    currentLux = read_lux();

    xTaskCreatePinnedToCore(taskCheckLux, "taskCheckLux", LUX_STACK_DEPTH, NULL, LUX_TASK_PRIORITY, &LUMINOSITY_HANDLE
, APP_CPU_NUM);
}