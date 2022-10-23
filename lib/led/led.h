#include <Arduino.h>

#define LED 2

void initLed();
void invertLed();
void acendeLed();
void apagaLed();
void setLed(uint8_t state);
uint8_t getLedState();
