#include "current.h"
#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"
#include "../TaskManagement/taskManagement.h"

float currentCurrent;
float lastCurrent = 0;
xTaskHandle CURRENT_HANDLE;

float readCurrent() {
    return 2.0 * (float) analogRead(CURRENT_PIN) / 4095.0;
}

void taskCheckCurrent(void *params) {
    while (1) {
       currentCurrent = readCurrent();

       if(abs(currentCurrent - lastCurrent) > CURRENT_SENSIBILITY) {
            lastCurrent = currentCurrent;

            if(currentCurrent > MAX_CURRENT) {
                apagaLed();
                suspendAllByCurrent();

                do {
                    logwarn("CRITICAL CURRENT: %.2fA", currentCurrent);
                    currentCurrent = readCurrent();

                    vTaskDelay(pdMS_TO_TICKS(CURRENT_DELAY_MS));
                    
                } while(currentCurrent > MAX_CURRENT);

                resumeAllByCurrent();
            }
       }

       

       vTaskDelay(pdMS_TO_TICKS(CURRENT_DELAY_MS));
    }
    
}

void resumeCurrentTask() {
    vTaskResume(CURRENT_HANDLE);
}

void suspendCurrentTask() {
    vTaskSuspend(CURRENT_HANDLE);
}

float getCurrent() {
    return readCurrent();
}

void initCurrent() {
    pinMode(CURRENT_PIN, INPUT);
    currentCurrent = readCurrent();
    xTaskCreatePinnedToCore(taskCheckCurrent, "taskCheckCurrent", CURRENT_STACK_DEPTH, NULL, CURRENT_TASK_PRIORITY, &CURRENT_HANDLE, APP_CPU_NUM);
}