#include <Arduino.h>
#include "../led/led.h"
#include "../Log/logs.h"

#define BUTTON 4
#define BUTTON_DEBOUNCE 1000 //ms

void initButton();
void buttonTask(void *arg);