#include "voltage.h"
#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"
#include "../TaskManagement/taskManagement.h"

float currentVoltage;
float lastVoltage = 0;
xTaskHandle VOLTAGE_HANDLE;

float readVoltage() {
    return 10 * analogRead(VOLTAGE_PIN) / 4095.;
}

void taskCheckVoltage(void *params) {
    while (1) {
        currentVoltage = readVoltage();

        if(abs(currentVoltage - lastVoltage) > VOLTAGE_SENSIBILITY) {
            lastVoltage = currentVoltage;
            loginfo("VOLTAGE: %.2fV", currentVoltage);

            if(currentVoltage > MAX_VOLTAGE) {
                setLed(LOW);
                suspendAllByVoltage();

                do {
                    logwarn("CRITICAL VOLTAGE: %.2fV", currentVoltage);
                    currentVoltage = readVoltage();
                    
                    vTaskDelay(pdMS_TO_TICKS(VOLTAGE_DELAY_MS));
                }while (currentVoltage > MAX_VOLTAGE);

                resumeAllByVoltage();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(VOLTAGE_DELAY_MS));
    }
    
}

void resumeVoltageTask() {
    vTaskResume(VOLTAGE_HANDLE);
}

void suspendVoltageTask() {
    vTaskSuspend(VOLTAGE_HANDLE);
}

void initVoltage() {
    pinMode(VOLTAGE_PIN, INPUT);
    currentVoltage = readVoltage();

    xTaskCreatePinnedToCore(taskCheckVoltage, "taskCheckVoltage",  VOLTAGE_STACK_DEPTH, NULL,  VOLTAGE_TASK_PRIORITY, &VOLTAGE_HANDLE,
 APP_CPU_NUM);
}