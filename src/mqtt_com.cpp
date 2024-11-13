#include "mqtt_com.hpp"
#include <WiFiClient.h>

mqtt_com::mqtt_com(WiFiClient wifi_connection) : client(wifi_connection){
    
}

bool mqtt_com::connect(){
    client.setServer("192.168.241.180", 1883);
    if (client.connect("Device1", "parking_broker", "123456")) {
        Serial.println("Connected to MQTT broker");
        Serial.println(client.state());
        return true;
    } else {
        Serial.println("MQTT connection failed");
        Serial.println(client.state());
        return false;
    }
}

bool mqtt_com::send_sensor_information(bool newState){

    bool result;
    if(newState == true){
        result = client.publish("Sensor2","true");
    }else {
        result = client.publish("Sensor2","false");
    }

    if (result) {
        Serial.println("Message sent successfully");
    } else {
        Serial.println("Message failed to send");
    }

    return newState;
}