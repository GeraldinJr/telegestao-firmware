#include "main.h"

// WiFi
const char *ssid = "telegestao";   

const char *password = "telegestao@password"; 

// MQTT Broker
const char *mqtt_broker = "192.168.1.4";
const char *topic = "teste";
const char *mqtt_username = "esp";
const char *mqtt_password = "esp";
const int mqtt_port = 1883;

typedef struct sensorData {
  float temperature;
  float luminosity;
  float current;
  float voltage;
  int Led;
} SensorData;

SensorData* dataToSend;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    
    loginfo("Message arrived in topic sensor/callback ");
    loginfo("Message:");

    for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
    }
    loginfo("-----------------------");
    
}

void initMqttClient()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        loginfo("Connecting to WiFi..");
    }

    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    String host = WiFi.getHostname();
    while (!client.connected())
    {

        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());

        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("Mqtt broker connected");
            client.publish("sensor/conections", "esp conectado");
            if(client.subscribe("callback")) {
              loginfo("Inscrito no topico");
              client.publish("sensor/conections", "inscrito no topic");
            }
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

String serializeData(SensorData data) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = data.temperature;
  doc["luminosity"] = data.luminosity;
  doc["current"] = data.current;
  doc["voltage"] = data.voltage;
  doc["led"] = data.Led;

  String dados;

  serializeJson(doc, dados);

  return dados;
}

void taskSendData(void *params) {
  while (1) {
    dataToSend->current = getCurrent();
    dataToSend->Led = getLedState();
    dataToSend->luminosity = getLux();
    dataToSend->temperature = getTemperature();
    dataToSend->voltage = getVoltage();

    client.publish("sensor/data", serializeData(*dataToSend).begin());
    
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void initSendData() {
  dataToSend = (SensorData*) malloc(sizeof(SensorData));

  xTaskCreatePinnedToCore(taskSendData, "taskSendData", 4096, NULL, 1, NULL
, APP_CPU_NUM);
}

void setup() {
  Serial.begin(115200);
  log_setserial(&Serial);
  log_setlevel(0);
  delay(100);

  initMqttClient();

  initLed();
  initButton();
  initLux();
  initTemperature();
  initCurrent();
  initVoltage();
  initSendData();
  vTaskDelete(NULL);
}

void loop() {

  client.loop();

}