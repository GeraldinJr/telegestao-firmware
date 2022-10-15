#include <Arduino.h>

// PINAGEM
#define LED 2
#define BUTTON 4
#define LDR 14
#define TEMPERATURE 26

// PARAMETROS GERAIS UTEIS
#define TEMPO_DEBOUNCE 0
#define GAMMA 0.7
#define RL10 50
#define BETA 3950

// VARIAVEIS
uint8_t ledState;
float currentLux;
float lastLux = 0;
float currentTemperature;
float lastTemperature = 25.0;

unsigned long timestamp_ultimo_acionamento = 0;

void IRAM_ATTR muda_status_led(){
  if( (millis() - timestamp_ultimo_acionamento) >= TEMPO_DEBOUNCE ) {

    ledState = !ledState;

    digitalWrite(LED, ledState);

    timestamp_ultimo_acionamento = millis();

    Serial.println("[INTERRUPCAO] - BOTAO ACIONADO");
  }
}

float read_lux(uint8_t pin) {
  int analogValue = analogRead(pin);
  float voltage = analogValue / 4096. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  
  return lux;
}

float read_temperature(uint8_t pin) {
  int analogValue = analogRead(pin);
  float celsius = 1 / (log(1 / (4095. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  return celsius;
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

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(TEMPERATURE, INPUT);
  
  digitalWrite(ledState, LOW);

  attachInterrupt(BUTTON, muda_status_led, RISING);
}

void loop() {
  currentLux = read_lux(LDR);
  currentTemperature = read_temperature(TEMPERATURE);

  if(abs(currentLux - lastLux) > 100.) {
    Serial.printf("%.2f lx\n", currentLux);
    ledState = led_by_lux(currentLux);

    digitalWrite(LED, ledState);

    lastLux = currentLux;
  }
                                                                                       
  if(abs(currentTemperature - lastTemperature) > 2.) {
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

  delay(100);
  
}