#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"

#define CURRENT_PIN 34
#define CURRENT_SENSIBILITY 0.2
#define MAX_CURRENT 1.0
#define CURRENT_DELAY_MS 100
#define STACK_DEPTH 4096
#define TASK_PRIORITY 1

void initCurrent();