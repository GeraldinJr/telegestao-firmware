#include "main.h"

void setup() {
  Serial.begin(115200);
  log_setserial(&Serial);
  log_setlevel(0);
  delay(100);

  initLed();
  initButton();
  initLux();
  initTemperature();
  
  vTaskDelete(NULL);
}

void loop() {

  //NADA A SER EXECUTADO

}