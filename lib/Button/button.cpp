#include "button.h"
#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"

TaskHandle_t ISR = NULL;

IRAM_ATTR void resumeInterrupt() { xTaskResumeFromISR(ISR); }

void buttonTask(void *arg) {
  while (1) {
    vTaskSuspend(NULL);
    loginfo("[INTERRUPCAO] - BOTAO ACIONADO");
    apagaLed();

    vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE));
  }
}

void initButton() {
    pinMode(BUTTON, INPUT);

    interrupts();
    attachInterrupt(BUTTON, resumeInterrupt, RISING);
    xTaskCreate(buttonTask, "buttonTask", STACK_DEPTH, NULL, 10, &ISR);
}