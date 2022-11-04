#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"

//PIN
#define TEMPERATURE_PIN 33

#define SENSIBILITY 2.0

#define BETA 3950
#define MAX_ANALOG 4095
#define MAX_TEMPERATURE 50.
#define MIN_TEMPERATURE -5.0
#define TEMPERATURE_DELAY_MS 2000
#define STACK_DEPTH 4096
#define TASK_PRIORITY 1

void initTemperature();