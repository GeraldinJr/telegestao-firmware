#include "button.h"

TaskHandle_t ISR = NULL;

IRAM_ATTR void resumeInterrupt() { xTaskResumeFromISR(ISR); }

void buttonTask(void *arg) {
  while (1) {
    vTaskSuspend(NULL);
    loginfo("[INTERRUPCAO] - BOTAO ACIONADO");
    invertLed();

    vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE));
  }
}

void initButton() {
    pinMode(BUTTON, INPUT);

    interrupts();
    attachInterrupt(BUTTON, resumeInterrupt, RISING);
    xTaskCreate(buttonTask, "buttonTask", 4096, NULL, 10, &ISR);
}