#include <Arduino.h>
#include "tof_sensor.hpp"
#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_com.hpp"




const char* ssid = "Not_Russian_spies";
const char* password = "Diller123";

void setup() {


}

void loop() {
  WiFiClient espClient;
  mqtt_com mqtt_sender(espClient);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  while(!mqtt_sender.connect()){
    Serial.println("Trying to connect to server");
    delay(500);
  }
  while(1){
    mqtt_sender.send_sensor_information();
    delay(100);
  }
}
