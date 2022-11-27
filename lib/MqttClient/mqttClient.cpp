#include "mqttClient.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../Log/logs.h"

// WiFi
const char *ssid = "GalaxyS20";    // Enter your WiFi name
const char *password = "afak1247"; // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.89.180";
const char *topic = "teste";
const char *mqtt_username = "esp";
const char *mqtt_password = "esp";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    loginfo("Message arrived in topic: ");
    loginfo(topic);
    loginfo("Message:");

    for (int i = 0; i < length; i++)
    {
        loginfo((char *)payload[i]);
    }

    loginfo("");
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
            Serial.println("Public emqx mqtt broker connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}