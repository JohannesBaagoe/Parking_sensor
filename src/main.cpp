#include <Arduino.h>
#include "tof_sensor.hpp"
#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_com.hpp"
#include <driver/rtc_io.h>

RTC_DATA_ATTR bool lastState = false;
#include <queue>

#define NUM_OF_SENSORS 1

ToF sensor1;
const char* ssid = "Not_Russian_spies";
const char* password = "Diller123";

void setup() {
    Serial.begin(115200);

    WiFiClient espClient;
    mqtt_com mqtt_sender(espClient);
    //Serial.println(WiFi.localIP()); // Print the local IP address
    sensor1 = ToF(1);
      
    Serial.print("Distance (mm): ");
    uint16_t distance = sensor1.getMeasurement();
    Serial.println(distance);

    uint16_t threshold = 500;

    if(distance <= threshold && lastState != true){
      
       WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(100);
    }

    while(!mqtt_sender.connect()){
      Serial.println("Trying to connect to server");
      delay(100);
    }

      mqtt_sender.send_sensor_information(true, ssid, password, mqtt_sender);
      lastState = true;
      Serial.println("Sends true");
    }
    else if(distance > threshold && lastState != false){
       WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(100);
      }

    while(!mqtt_sender.connect()){
      Serial.println("Trying to connect to server");
      delay(100);
    }
      mqtt_sender.send_sensor_information(false, ssid, password, mqtt_sender);
      lastState = false;
      Serial.println("sends false");
    }else{
      Serial.println("sends nothing");
    }
    
    // Angiv en timer til at vågne op efter 10 sekunder
    esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 sekunder


    Serial.println("Sleep");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    // Hvis Bluetooth er aktiveret, sluk for det
    btStop();  
    // Deaktiver RTC komponenter
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC8M, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    
    // Gå i deep sleep
    esp_deep_sleep_start();
}

void loop(){}

/*void loop() {


    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    while(!mqtt_sender.connect()){
      Serial.println("Trying to connect to server");
      delay(500);
    }

    sensor1 = ToF(1);

    if(sensor1.SetInteruptThresholds(400, 500) == VL53L0X_ERROR_NONE){
      Serial.println("Lykkedes pik");
    }else{
      Serial.print("Error pik");
    }
  
    while(1){
      
      Serial.print("Distance (mm): ");
      uint16_t distance = sensor1.getMeasurement();
      Serial.println(distance);
      mqtt_sender.send_sensor_information();
      delay(100);

    }
}*/
