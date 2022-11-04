#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"


#define VOLTAGE_PIN 32
#define VOLTAGE_SENSIBILITY 0.5
#define MAX_VOLTAGE 5
#define VOLTAGE_DELAY_MS 1000
#define STACK_DEPTH 4096
#define TASK_PRIORITY 1

void initVoltage();