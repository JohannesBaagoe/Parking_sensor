// mqtt_client.h

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <WiFi.h>            
#include <PubSubClient.h>
#include <WiFiClientSecure.h>


// Function declarations
void setup_wifi(const char* ssid, const char *password);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void mqtt_setup();
void mqtt_loop();
uint8_t *mqtt_connect_packet(const char* clientID);
void mqtt_connect(WiFiClient espClient, const char* brokerIP, const uint16_t brokerPort);


// WiFi and MQTT configurations
extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern WiFiClient espClient;
extern PubSubClient client;

#endif  // MQTT_CLIENT_H