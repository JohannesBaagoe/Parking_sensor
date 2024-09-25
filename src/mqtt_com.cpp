#include "mqtt_com.hpp"

mqtt_com::mqtt_com(WiFiClient wifi_connection) : client(wifi_connection){
    
}

bool mqtt_com::connect(){
    client.setServer("192.168.130.180",1883);
    client.connect("Device1","parking_broker","123456");
    return true;
}

bool mqtt_com::send_sensor_information(bool newState, const char* ssid, const char* password, mqtt_com mqtt_sender){

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