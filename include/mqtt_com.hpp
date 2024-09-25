#ifndef MQTT_COM_H
#define MQTT_COM_H

#include <stdint.h>
#include <PubSubClient.h>
#include <WiFi.h>

class mqtt_com{
    public:
        mqtt_com(WiFiClient);
        uint16_t get_sensor_id();
        bool send_sensor_information(bool newState, const char* ssid, const char* password, mqtt_com mqtt_sender);
        bool connect();
        PubSubClient client;

    private:
        uint16_t sensor_id;

};


#endif