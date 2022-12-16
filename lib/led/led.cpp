#include "led.h"

int ledState;

int getDutyCycle() {
    double fator = ledState/100.0;
    return fator * 255;
}

void initLed() {
    pinMode(LED, OUTPUT);
    ledState = 0;
    analogWrite(LED, getDutyCycle());
}

void apagaLed() {
    ledState = 0;
    analogWrite(LED, getDutyCycle());
}

void setLed(int state) {
    ledState = state;
    analogWrite(LED, getDutyCycle());
}

int getLedState() {
    return ledState;
}

