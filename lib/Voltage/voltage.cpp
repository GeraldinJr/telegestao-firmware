#include "voltage.h"

float currentVoltage;
float lastVoltage = 0;

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
            }
        }

        vTaskDelay(pdMS_TO_TICKS(VOLTAGE_DELAY_MS));
    }
    
}

void initVoltage() {
    pinMode(VOLTAGE_PIN, INPUT);
    currentVoltage = readVoltage();

    xTaskCreatePinnedToCore(taskCheckVoltage, "taskCheckVoltage", STACK_DEPTH, NULL, TASK_PRIORITY, NULL, APP_CPU_NUM);
}