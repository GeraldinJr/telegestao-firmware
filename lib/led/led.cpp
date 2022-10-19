#include "led.h"

uint8_t ledState;

void initLed() {
    pinMode(LED, OUTPUT);
    ledState = LOW;
    digitalWrite(LED, ledState);
}

void invertLed() {
    ledState = !ledState;
    digitalWrite(LED, ledState);
}

void acendeLed() {
    ledState = HIGH;
    digitalWrite(LED, ledState);
}

void apagaLed() {
    ledState = LOW;
    digitalWrite(LED, ledState);
}

void setLed(uint8_t state) {
    ledState = state;
    digitalWrite(LED, ledState);
}

uint8_t getLedState() {
    return ledState;
}

