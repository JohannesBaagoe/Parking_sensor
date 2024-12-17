#include "mqtt_com.hpp"
#include <WiFiClient.h>

mqtt_com::mqtt_com(WiFiClient wifi_connection) : client(wifi_connection){
    
}

bool mqtt_com::connect(){
    client.setServer("192.168.137.129", 1883);
    if (client.connect("Device2", "parking_broker", "123456")) {
        //Serial.println("Connected to MQTT broker");
        //Serial.println(client.state());
        return true;
    } else {
        Serial.println("MQTT connection failed");
        Serial.println(client.state());
        return false;
    }
}

bool mqtt_com::send_sensor_information(bool newState){

    bool result;

    for(size_t i = 0; i < 5; i++){
        if(newState == true){
            result = client.publish("Sensor2", "true", false);
        }else {
            result = client.publish("Sensor2","false", false);
        }
        delay(50);
    }

    if (result) {
        Serial.println("Message sent successfully");
    } else {
        Serial.println("Message failed to send");
    }

    return newState;
}

int mqtt_com::testSend(uint16_t num){

    bool result;
     result = client.publish("Sensor2", "true", false);

     if (result) {
        Serial.println("Message sent successfully");
        num++;
    } else {
        Serial.println("Message failed to send");
    }
     return num;
}