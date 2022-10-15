#include <Arduino.h>
#include "../Led/led.h"
#include "../Log/logs.h"

#define BUTTON 4
#define BUTTON_DEBOUNCE 1000 //ms

void initButton();
void buttonTask(void *arg);