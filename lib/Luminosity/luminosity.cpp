#include "luminosity.h"

float currentLux;
float lastLux = 0;

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

void initLux() {
    pinMode(LDR_PIN, INPUT);
    currentLux = read_lux();

    xTaskCreatePinnedToCore(taskCheckLux, "taskCheckLux", STACK_DEPTH, NULL, TASK_PRIORITY, NULL, APP_CPU_NUM);
}