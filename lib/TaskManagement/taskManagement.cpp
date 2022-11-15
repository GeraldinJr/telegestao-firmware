#include "taskManagement.h"
#include "../Current/current.h"
#include "../Voltage/voltage.h"
#include "../Luminosity/luminosity.h"
#include "../Temperature/temperature.h"

void suspendAllByCurrent() {
    suspendVoltageTask();
    suspendTemperatudeTask();
    suspendLuminosityTask();
}

void resumeAllByCurrent() {
    resumeVoltageTask();
    resumeTemperatureTask();
    resumeLuminosityTask();
}

void suspendAllByVoltage() {
    suspendCurrentTask();
    suspendTemperatudeTask();
    suspendLuminosityTask();
}

void resumeAllByVoltage() {
    resumeCurrentTask();
    resumeTemperatureTask();
    resumeLuminosityTask();
}

void suspendAllByTemperature() {
    suspendCurrentTask();
    suspendVoltageTask();
    suspendLuminosityTask();
}

void resumeAllByTemperature() {
    resumeCurrentTask();
    resumeVoltageTask();
    resumeLuminosityTask();
}