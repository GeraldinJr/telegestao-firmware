#include "luminosidade.h"

float currentLux;
float lastLux = 0;

float read_lux() {
  int analogValue = analogRead(LDR);
  float voltage = analogValue / 4096. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  
  return lux;
}

void taskCheckLux(void *params) {
    while (1) {
        currentLux = read_lux();

        if(abs(currentLux - lastLux) > SENSIBILIDADE_LUMINOSIDADE) {
            Serial.printf("%.2f lx\n", currentLux);
            setLed(currentLux < 500);

            lastLux = currentLux;
        }

        vTaskDelay(pdMS_TO_TICKS(LUX_DELAY_MS));
    }
}

void initLux() {
    pinMode(LDR, INPUT);
    currentLux = read_lux();

    xTaskCreatePinnedToCore(taskCheckLux, "taskCheckLux", 2048, NULL, 1, NULL, APP_CPU_NUM);
}