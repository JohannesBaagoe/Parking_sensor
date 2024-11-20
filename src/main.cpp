#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include <tof_sensor.hpp>
#include <Arduino.h>
#include "mqtt_client.hpp"
#include <driver/rtc_io.h>

RTC_DATA_ATTR bool lastState = false;
RTC_DATA_ATTR uint16_t num = 65535;
const int VL53LOX_InterruptPin = 13;
#define SENSOR_INTERRUPT_PIN 13  // Eksempel GPIO-pin
const gpio_num_t interruptPin = static_cast<gpio_num_t>(SENSOR_INTERRUPT_PIN);

const byte VL53LOX_ShutdownPin = 14;
volatile byte VL53LOX_State = LOW;
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

ToF sensor;

void startHiberNation(){
      //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
      //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
      esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC8M, ESP_PD_OPTION_OFF);
      esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF);
      esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
      
      esp_deep_sleep_start();
  }

void setup() {
  Serial.begin(115200);

  // Create timers
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));


  // Timeout-variabler
  // unsigned long startAttemptTime = millis();
  // const unsigned long timeout = 5000; // 5 sekunder

  // // Vent på Wi-Fi-forbindelse eller timeout
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.print('.');
  //   delay(100);

  //   // Tjek for timeout
  //   if (millis() - startAttemptTime > timeout) {
  //     Serial.println("\nWi-Fi-forbindelse fejlede. Sover igen");
  //     //ESP.restart(); // Genstart ESP32
  //     esp_sleep_enable_timer_wakeup(5000000);
  //     startHiberNation();
  //   }
  // }

    Serial.println("\n\rConnected");

    // Set up Wi-Fi and MQTT
    WiFi.onEvent(WiFiEvent);
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    connectToWifi();

  Wire.begin(21, 22, 400000); // Brug SDA=21 og SCL=22 med hastighed 400kHz

  pinMode(VL53LOX_ShutdownPin, OUTPUT);
  pinMode(VL53LOX_InterruptPin, INPUT_PULLUP);

  //init sensor
  lastState = sensor.init(lastState, VL53LOX_ShutdownPin);

  // num = mqtt_sender.testSend(num);

  // set thresholds
  sensor.SetInteruptThresholds(200, 300);

  // Aktivér kontinuerlig målingstilstand
  //Serial.println("Set Mode VL53L0X_DEVICEMODE_CONTINUOUS_RANGING... ");
  lox.setDeviceMode(VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING, false);

  // start mesurement
  sensor.startRangeContinuous(100, 100000);

  // Sæt ESP32 i deep sleep indtil interrupt
  esp_sleep_enable_ext0_wakeup(interruptPin, 0); // Wake up on low signal
  
  Serial.println("Going to sleep...\n\n");
  startHiberNation();
}

void loop() {
  // Loop-funktion er ikke nødvendig her, da ESP32 går i sleep
  // når der ikke er aktivitet. All logik håndteres i interrupt.
}
