#include <Arduino.h>
// #include "tof_sensor.hpp"
#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_com.hpp"
#include <driver/rtc_io.h>
#include <WiFiClient.h>

RTC_DATA_ATTR bool lastState = false;
#include <queue>

#define NUM_OF_SENSORS 1

// ToF sensor1;
const char* ssid = "Not_Russian_spies";
const char* password = "Diller123";
const char* mqtt_port = "1883";
// const char* ca_cert = "C:/Users/chris/Documents/IoT/IoT_School/Git/Parking_sensor/cert/ca_cert.pem";
// const char* client_cert = "C:/Users/chris/Documents/IoT/IoT_School/Git/Parking_sensor/cert/client_mqtt_cert.pem";
// const char* client_key = "C:/Users/chris/Documents/IoT/IoT_School/Git/Parking_sensor/cert/client_mqtt_key.pem";
const char* mqtt_server = "192.168.254.180";  // e.g., "broker.hivemq.com" or your local broker

const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIBtTCCAVugAwIBAgIUJ2z8SUJUZklRusoL4NvMh34LSHswCgYIKoZIzj0EAwIwMDELMAkGA1UEBhMCREsxDzANBgNVBAcMBkFhcmh1czEQMA4GA1UEAwwHc2Vuc29yMTAeFw0yNDEwMDIxMDU0MzJaFw0yNTEwMDIxMDU0MzJaMDAxCzAJBgNVBAYTAkRLMQ8wDQYDVQQHDAZBYXJodXMxEDAOBgNVBAMMB3NlbnNvcjEwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAShuLVEhm4ZUaNqPUQIF34M9tyr824BYVbhJqZfciC585iM61Nu4dZ/WS+YmYzcHMKBL2XCG4Rg3I0lHVarnTHuo1MwUTAdBgNVHQ4EFgQU3jj3Ket6k7/a9xQ3bZV0gSp9V2EwHwYDVR0jBBgwFoAU3jj3Ket6k7/a9xQ3bZV0gSp9V2EwDwYDVR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAgNIADBFAiB4qQaK6o5UlN9F5OrhkVdpSDvQzQgjjx6Su4LhQ4Uc7QIhAOKiJs/DOGs54A2fvRAQkB7W0dbK5X+5gX53azMU3AFZ\n" \
"-----END CERTIFICATE-----\n";

const char* client_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIBWzCCAQECFDKy/r0Ktd8d6EVq1doJ1wrtgW2DMAoGCCqGSM49BAMCMDAxCzAJBgNVBAYTAkRLMQ8wDQYDVQQHDAZBYXJodXMxEDAOBgNVBAMMB3NlbnNvcjEwHhcNMjQxMDAyMTA1ODM3WhcNMjcwNjI5MTA1ODM3WjAwMQswCQYDVQQGEwJESzEPMA0GA1UEBwwGQWFyaHVzMRAwDgYDVQQDDAdzZW5zb3IyMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEBDigMtV2LSJqoN9zJp+BqpEiuLDovwFtz9sUiAX+9Ig0NcWXKSiI8qeOB2Nl+okHBHsUuLN1EXJYa/A45EnhRTAKBggqhkjOPQQDAgNIADBFAiEAg9As29iErtbngM+PoZy+n81ehVXOHQS92Hy3Sr+Ma+YCIEW5QQVNmAwNOLlKxEcFZkzWwcgPmF+SnLlu4BN9A/H1\n" \
"-----END CERTIFICATE-----\n";

const char* client_key = \
"-----BEGIN PRIVATE KEY-----\n" \
"MHcCAQEEICIB1sLKnJUCZdo05LDXGB9IIBgXIgV0XcRJXH0l6atMoAoGCCqGSM49AwEHoUQDQgAEBDigMtV2LSJqoN9zJp+BqpEiuLDovwFtz9sUiAX+9Ig0NcWXKSiI8qeOB2Nl+okHBHsUuLN1EXJYa/A45EnhRQ==\n" \
"-----END PRIVATE KEY-----\n";




void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(100);
  }
  Serial.println("\nConnected to WiFi");

  WiFiClient espClient;
  mqtt_com mqtt_sender(espClient);

  // Attempt to connect to the MQTT server
  while (!mqtt_sender.connect()) {
    Serial.println("Trying to connect to server");
    delay(100);
  }

  // Send sensor information
  mqtt_sender.send_sensor_information(true);
  Serial.println("Sends true");

  // Put ESP32 into deep sleep for 20 seconds
  Serial.println("Entering deep sleep for 20 seconds...");
  esp_sleep_enable_timer_wakeup(30 * 1000000);
  esp_deep_sleep_start();
}










// void setup() {



//     Serial.begin(115200);
//     // Set the duration for hibernation in microseconds (e.g., 10 seconds)

//     WiFi.mode(WIFI_STA);
//     WiFi.begin(ssid, password);

//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print('.');
//         Serial.println(WiFi.status());
//         delay(100);
//     }



    // } //else if (distance > threshold && lastState != false) {
    //   else if (lastState == true) {
    //     WiFi.mode(WIFI_STA);
    //     WiFi.begin(ssid, password);

    //     while (WiFi.status() != WL_CONNECTED) {
    //         Serial.print('.');
    //         Serial.println(WiFi.status());
    //         delay(100);
    //     }

    //     while (!mqtt_sender.connect()) {
    //         Serial.println("Trying to connect to server");
    //         delay(100);
    //     }

    //     mqtt_sender.send_sensor_information(false);
    //     lastState = false;
    //     Serial.println("Sends false");

    // } else {
    //     Serial.println("Sends nothing");
    // }
    
    // // // Setup timer to wake up after 10 seconds
    // esp_sleep_enable_timer_wakeup(20 * 1000000); // 10 seconds

    // Serial.println("Sleep");
    // WiFi.disconnect();
    // WiFi.mode(WIFI_OFF);
    
    // // // If Bluetooth is enabled, stop it
    // btStop();  
    
    // // // Disable RTC components
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC8M, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);

    // // // Enter deep sleep
    // esp_deep_sleep_start();
// }

void loop(){

    // WiFiClient espClient;
    // // espClient.setCACert(ca_cert);
    // // espClient.setCertificate(client_cert);
    // // espClient.setPrivateKey(client_key);
    // mqtt_com mqtt_sender(espClient);

    // while (!mqtt_sender.connect()) {
    //     Serial.println("Trying to connect to server");
    //     delay(100);
    // }

    // while (1) {
    //   mqtt_sender.send_sensor_information(true);
    //   lastState = true;
    //   Serial.println("Sends true");
    // }

    // // sensor1 = ToF(1);
    // uint16_t threshold = 500;

    // // Measure distance
    // Serial.print("Distance (mm): ");
    // // uint16_t distance = sensor1.getMeasurement();
    // // Serial.println(distance);
    // lastState = !lastState;
    // // Check if the distance is less than the threshold
    // // if (distance <= threshold && lastState != true) {
    // if (lastState == false){
    // WiFiClientSecure espClient;
    // // espClient.setCACert(ca_cert);
    // // espClient.setCertificate(client_cert);
    // // espClient.setPrivateKey(client_key);
    // mqtt_com mqtt_sender(espClient);
    // // sensor1 = ToF(1);
    // uint16_t threshold = 500;

    // // Measure distance
    // Serial.print("Distance (mm): ");
    // // uint16_t distance = sensor1.getMeasurement();
    // // Serial.println(distance);¨
    // lastState = true;
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password);

    // while (WiFi.status() != WL_CONNECTED) {
    //     Serial.print('.');
    //     delay(100);
    // }
    // espClient.setTimeout(5000);  // Timeout of 5 seconds
    // while (!mqtt_sender.connect()) {
    //   Serial.println("Trying to connect to server");
    //   delay(100);
    // } 

    // while(1) {

    //   // lastState = !lastState;
    //   // Check if the distance is less than the threshold
    //   // if (distance <= threshold && lastState != true) {
    //   if (lastState == false){
          
    //       mqtt_sender.send_sensor_information(true);
    //       lastState = true;
    //       Serial.println("Sends true");

    //   } //else if (distance > threshold && lastState != false) {
    //     else if (lastState == true) {

    //       mqtt_sender.send_sensor_information(false);
    //       lastState = false;
    //       Serial.println("Sends false");

    //   } else {
    //       Serial.println("Sends nothing");
    //   }
      
    //   // Setup timer to wake up after 10 seconds
    //   // esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 seconds

    //   Serial.println("Sleep");
    //   delay(1000);
    // }
}

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
