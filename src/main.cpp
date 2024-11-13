#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include <tof_sensor.hpp>
#include <Arduino.h>
// #include "tof_sensor.hpp"
#include <PubSubClient.h>
#include <WiFi.h>
#include "mqtt_com.hpp"
#include <driver/rtc_io.h>
#include <WiFiClient.h>

RTC_DATA_ATTR bool lastState = false;
const int VL53LOX_InterruptPin = 13;
#define SENSOR_INTERRUPT_PIN 13  // Eksempel GPIO-pin
const gpio_num_t interruptPin = static_cast<gpio_num_t>(SENSOR_INTERRUPT_PIN);

const byte VL53LOX_ShutdownPin = 14;
volatile byte VL53LOX_State = LOW;
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

ToF sensor;

void IRAM_ATTR VL53LOXISR() {
  // Læs om vi er høj eller lav
  VL53LOX_State = digitalRead(VL53LOX_InterruptPin);
  esp_sleep_enable_ext0_wakeup(interruptPin, 0); // Wake up on low signal
}

const char* ssid = "Not_Russian_spies";
const char* password = "Diller123";
const char* mqtt_port = "1883";
// const char* ca_cert = "cert/ca_cert.pem";
// const char* client_cert = "cert/client_mqtt_cert.pem";
// const char* client_key = "cert/client_mqtt_key.pem";
const char* mqtt_server = "192.168.241.180";  // e.g., "broker.hivemq.com" or your local broker

// const char* ca_cert = \
// "-----BEGIN CERTIFICATE-----\n" \
// "MIIBtTCCAVugAwIBAgIUJ2z8SUJUZklRusoL4NvMh34LSHswCgYIKoZIzj0EAwIw\n" \
// "MDELMAkGA1UEBhMCREsxDzANBgNVBAcMBkFhcmh1czEQMA4GA1UEAwwHc2Vuc29y\n" \
// "MTAeFw0yNDEwMDIxMDU0MzJaFw0yNTEwMDIxMDU0MzJaMDAxCzAJBgNVBAYTAkRL\n" \
// "MQ8wDQYDVQQHDAZBYXJodXMxEDAOBgNVBAMMB3NlbnNvcjEwWTATBgcqhkjOPQIB\n" \
// "BggqhkjOPQMBBwNCAAShuLVEhm4ZUaNqPUQIF34M9tyr824BYVbhJqZfciC585iM\n" \
// "61Nu4dZ/WS+YmYzcHMKBL2XCG4Rg3I0lHVarnTHuo1MwUTAdBgNVHQ4EFgQU3jj3\n" \
// "Ket6k7/a9xQ3bZV0gSp9V2EwHwYDVR0jBBgwFoAU3jj3Ket6k7/a9xQ3bZV0gSp9\n" \
// "V2EwDwYDVR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAgNIADBFAiB4qQaK6o5UlN9F\n" \
// "5OrhkVdpSDvQzQgjjx6Su4LhQ4Uc7QIhAOKiJs/DOGs54A2fvRAQkB7W0dbK5X+5\n" \
// "gX53azMU3AFZ\n" \
// "-----END CERTIFICATE-----\n";

// const char* client_cert = \
// "-----BEGIN CERTIFICATE-----\n" \
// "MIIBWzCCAQECFDKy/r0Ktd8d6EVq1doJ1wrtgW2DMAoGCCqGSM49BAMCMDAxCzAJ\n" \
// "BgNVBAYTAkRLMQ8wDQYDVQQHDAZBYXJodXMxEDAOBgNVBAMMB3NlbnNvcjEwHhcN\n" \
// "MjQxMDAyMTA1ODM3WhcNMjcwNjI5MTA1ODM3WjAwMQswCQYDVQQGEwJESzEPMA0G\n" \
// "A1UEBwwGQWFyaHVzMRAwDgYDVQQDDAdzZW5zb3IyMFkwEwYHKoZIzj0CAQYIKoZI\n" \
// "zj0DAQcDQgAEBDigMtV2LSJqoN9zJp+BqpEiuLDovwFtz9sUiAX+9Ig0NcWXKSiI\n" \
// "8qeOB2Nl+okHBHsUuLN1EXJYa/A45EnhRTAKBggqhkjOPQQDAgNIADBFAiEAg9As\n" \
// "29iErtbngM+PoZy+n81ehVXOHQS92Hy3Sr+Ma+YCIEW5QQVNmAwNOLlKxEcFZkzW\n" \
// "wcgPmF+SnLlu4BN9A/H1\n" \
// "-----END CERTIFICATE-----\n";

// const char* client_key = \
// "-----BEGIN EC PRIVATE KEY-----\n" \
// "MHcCAQEEICIB1sLKnJUCZdo05LDXGB9IIBgXIgV0XcRJXH0l6atMoAoGCCqGSM49\n" \
// "AwEHoUQDQgAEBDigMtV2LSJqoN9zJp+BqpEiuLDovwFtz9sUiAX+9Ig0NcWXKSiI\n" \
// "8qeOB2Nl+okHBHsUuLN1EXJYa/A45EnhRQ==\n" \
// "-----END EC PRIVATE KEY-----\n";

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
  // espClient.setCACert(ca_cert);
  // espClient.setCertificate(client_cert);
  // espClient.setPrivateKey(client_key);
  
  mqtt_com mqtt_sender(espClient);

  // Attempt to connect to the MQTT server
  while (!mqtt_sender.connect()) {
    Serial.println("Trying to connect to server");
    delay(100);
  }

  Wire.begin(21, 22, 100000); // Brug SDA=21 og SCL=22 med hastighed 100kHz

  pinMode(VL53LOX_ShutdownPin, OUTPUT);
  pinMode(VL53LOX_InterruptPin, INPUT_PULLUP);

  //init sensor
  lastState = sensor.init(lastState, VL53LOX_ShutdownPin);

  // Send sensor information
  mqtt_sender.send_sensor_information(lastState);
  
  if (lastState == false){
      Serial.println("Sends false");
  }else {
    Serial.println("Sends true");
  }
  
  // set thresholds
  sensor.SetInteruptThresholds(200, 300);

  // Aktivér kontinuerlig målingstilstand
  Serial.println("Set Mode VL53L0X_DEVICEMODE_CONTINUOUS_RANGING... ");
  lox.setDeviceMode(VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING, false);

  // start mesurement
  sensor.startRangeContinuous(100, 10000);

  //Serial.println("Attach interrupt");
  //attachInterrupt(VL53LOX_InterruptPin, VL53LOXISR, CHANGE);

  // Sæt ESP32 i deep sleep indtil interrupt
  esp_sleep_enable_ext0_wakeup(interruptPin, 0); // Wake up on low signal
  
  Serial.println("Going to sleep...\n\n");
  sensor.startHiberNation();
}

void loop() {
  // Loop-funktion er ikke nødvendig her, da ESP32 går i sleep
  // når der ikke er aktivitet. All logik håndteres i interrupt.
}
