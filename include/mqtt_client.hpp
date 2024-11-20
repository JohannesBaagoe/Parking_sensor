#ifndef MQTT_WIFI_H
#define MQTT_WIFI_H

#include <WiFi.h>
#include <AsyncMqttClient.h>
extern "C" {
    #include "freertos/FreeRTOS.h"
    #include "freertos/timers.h"
}

// Wi-Fi and MQTT Configuration
#define WIFI_SSID "Not_Russian_spies"
#define WIFI_PASSWORD "Diller123"
#define MQTT_HOST IPAddress(192, 168, 50, 180)
#define MQTT_PORT 1883

// Function Prototypes
void connectToWifi();
void connectToMqtt();
void WiFiEvent(WiFiEvent_t event);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttPublish(uint16_t packetId);

// Extern variables for use in other files
extern AsyncMqttClient mqttClient;
extern TimerHandle_t mqttReconnectTimer;
extern TimerHandle_t wifiReconnectTimer;

#endif
