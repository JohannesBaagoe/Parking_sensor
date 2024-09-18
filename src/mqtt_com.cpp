#include "mqtt_com.hpp"

mqtt_com::mqtt_com(WiFiClient wifi_connection) : client(wifi_connection){
    
}

bool mqtt_com::connect(){
    client.setServer("192.168.163.180",1883);
    client.connect("Device1","parking_broker","123456");
    return true;
}

bool mqtt_com::send_sensor_information(){
    client.publish("Sensor2","true");
    return true;
}