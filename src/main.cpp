#include <Arduino.h>
#include "main.h"

// VARIAVEIS
uint8_t ledState;
float currentLux;
float lastLux = 0;
float currentTemperature;
float lastTemperature = 25.0;
float currentCurrent;
float lastCurrent = 0;
float currentVoltage;
float lastVoltage =0;

typedef float (*read_value) (uint8_t);
typedef uint8_t (*led_by_value) (float);

unsigned long timestamp_ultimo_acionamento = 0;

TaskHandle_t ISR = NULL;

void button_task(void *arg) {
  while (1) {
    vTaskSuspend(NULL);
    Serial.println("[INTERRUPCAO] - BOTAO ACIONADO");
    ledState = !ledState;
    digitalWrite(LED, ledState);

    vTaskDelay(pdMS_TO_TICKS(TEMPO_DEBOUNCE));
  }
}

IRAM_ATTR void resumeInterrupt() { xTaskResumeFromISR(ISR); }

float read_lux() {
  int analogValue = analogRead(LDR);
  float voltage = analogValue / 4096. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  
  return lux;
}

float read_temperature(uint8_t pin) {
  int analogValue = analogRead(pin);
  float celsius = 1 / (log(1 / (4096. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  return celsius;
}

float read_current(uint8_t pin) {
  int analogValue = analogRead(pin);
  float amperes = 5 * analogValue / 4096.;
  return amperes;
}

float read_voltage(uint8_t pin) {
  int analogValue = analogRead(pin);
  float voltage = 10 * analogValue / 4096.0;
  return voltage;
}

uint8_t led_by_lux(float lux) {
  if(lux < 500.) {
    return HIGH;
  }
    
  return LOW;
}

uint8_t led_by_temperature(float temperature) {
  if( temperature > 50. || temperature < -5.) {
    return LOW;
  }

  return HIGH;
}

uint8_t led_by_current(float current) {
  if(current > 2.0) {
    return LOW;
  }

  return HIGH;
}

uint8_t led_by_voltage(float voltage) {
  if(voltage < 2.5 || voltage > 10) {
    return LOW;
  }

  return HIGH;
}

void taskCheckLux(void *params) {
  while (1) {
    currentLux = read_lux();

    if(abs(currentLux - lastLux) > SENSIBILIDADE_LUMINOSIDADE) {
      Serial.printf("%.2f lx\n", currentLux);
      ledState = currentLux < 500;

      digitalWrite(LED, ledState);

      lastLux = currentLux;
    }

    vTaskDelay(pdMS_TO_TICKS(DEFAULT_DELAY_MS));
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(TEMPERATURE, INPUT);
  //pinMode(CURRENT, INPUT);
  //pinMode(VOLTAGE, INPUT);
  
  digitalWrite(ledState, LOW);

  // Interrupção
  interrupts();
  attachInterrupt(BUTTON, resumeInterrupt, RISING);
  xTaskCreate(button_task, "button_task", 4096, NULL, 10, &ISR);

  // Task dos sensores
  xTaskCreatePinnedToCore(taskCheckLux, "taskCheckLux", 2048, NULL, 1, NULL, APP_CPU_NUM);
  vTaskDelete(NULL);
}

void loop() {

  currentTemperature = read_temperature(TEMPERATURE);
  // CHECK TEMPERATURE                                                                                     
  if(abs(currentTemperature - lastTemperature) >  SENSIBILIDADE_TEMPERATURA) {
    Serial.printf("%.2f °C\n", currentTemperature);

    lastTemperature = currentTemperature;

    if(!led_by_temperature(currentTemperature)) {
      uint8_t lastLedState = ledState;
      ledState = LOW;
      digitalWrite(LED, ledState);

      float temp = read_temperature(TEMPERATURE);

      while (!led_by_temperature(temp)){
        temp = read_temperature(TEMPERATURE);
        Serial.printf("TEMPERATURA CRITICA: %.2f °C\n", temp);
        delay(500);
      }

      ledState = lastLedState;
      digitalWrite(LED, ledState);
    }
  }

  //CHECK CURRENT
  /*if(abs(currentCurrent - lastCurrent) > SENSIBILIDADE_CORRENTE) {
    Serial.printf("%.2f A\n", currentCurrent);

    lastCurrent = currentCurrent;

    if(!led_by_current(currentCurrent)){
      uint8_t lastLedState = ledState;
      ledState = LOW;
      digitalWrite(LED, ledState);
      float current = read_current(CURRENT);

      while (!led_by_current(current)) {
        current = read_current(CURRENT);
        Serial.printf("CORRENTE CRITICA: %.2F A\n", current);
        delay(500);
      }

      ledState = lastLedState;
      digitalWrite(LED, ledState);
    }
  }

  //CHECK VOLTAGE
  if(abs(currentVoltage - lastVoltage) > SENSIBILIDADE_TENSAO) {
    Serial.printf("%.2f V", currentVoltage);

    lastVoltage = currentVoltage;

    if(!led_by_voltage(currentVoltage)) {

      uint8_t lastLedState = ledState;
      ledState = LOW;
      digitalWrite(LED, ledState);
      
      float voltage = read_voltage(VOLTAGE);

      while (!led_by_voltage(voltage)) {
        voltage = read_voltage(VOLTAGE);
        Serial.printf("TENSAO CRITICA: %.2f V\n", voltage);
        delay(500);
      }

       ledState = lastLedState;
      digitalWrite(LED, ledState);
      
    }
  }*/

}