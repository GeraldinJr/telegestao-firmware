#include "current.h"

float currentCurrent;
float lastCurrent = 0;

float readCurrent() {
    return 2 * analogRead(CURRENT_PIN) / 4095.;
}

void taskCheckCurrent(void *params) {
    while (1) {
       currentCurrent = readCurrent();

       if(abs(currentCurrent - lastCurrent) > CURRENT_SENSIBILITY) {
            lastCurrent = currentCurrent;
            loginfo("CURRENT: %.2fA", currentCurrent);

            if(currentCurrent > MAX_CURRENT) {
                setLed(LOW);
            }
       }

       vTaskDelay(pdMS_TO_TICKS(CURRENT_DELAY_MS));
    }
    
}

void initCurrent() {
    pinMode(CURRENT_PIN, INPUT);
    currentCurrent = readCurrent();

    xTaskCreatePinnedToCore(taskCheckCurrent, "taskCheckCurrent", STACK_DEPTH, NULL, TASK_PRIORITY, NULL, APP_CPU_NUM);
}