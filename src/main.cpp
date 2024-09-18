#include <Arduino.h>
#include "tof_sensor.hpp"
#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_com.hpp"


ToF sensor1;


void handleInterrupt() {
  // Hvad du vil gøre, når interrupten opstår
  Serial.println("Interrupt triggered!");
}

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);  // Bruger SDA pin 21 og SCL pin 22
    
    pinMode(2, INPUT);

  // Opsæt interrupt på pin 2: CHANGE betyder, at ISR udløses ved både HIGH->LOW og LOW->HIGH
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, RISING);
}

void loop() {
       
    sensor1 = ToF(1);

    if(sensor1.SetInteruptThresholds(100, 500) == VL53L0X_ERROR_NONE){
      Serial.println("Lykkedes pik");
    }else{
      Serial.print("Error pik");
    }
  
    while(1){
      
      Serial.print("Distance (mm): ");
      uint16_t distance = sensor1.getMeasurement();
      Serial.println(distance);

    }
}


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
    Serial.println("gay");
    delay(500);
  }
  Serial.println("efter connect");
  while(1){
    mqtt_sender.send_sensor_information();
    delay(100);
  }
}
