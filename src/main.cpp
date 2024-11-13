#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include <tof_sensor.hpp>


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

void setup() {
  Serial.begin(115200);

  // Vent indtil serial porten åbner for native USB-enheder
  while (!Serial) {
    delay(1);
  }

  Serial.println("Awake");

  Wire.begin(21, 22, 100000); // Brug SDA=21 og SCL=22 med hastighed 100kHz

  pinMode(VL53LOX_ShutdownPin, OUTPUT);
  pinMode(VL53LOX_InterruptPin, INPUT_PULLUP);

  //init sensor
  lastState = sensor.init(lastState, VL53LOX_ShutdownPin);

  if(lastState == false){
    Serial.println("Occupied");
  } else {
    Serial.println("Free");
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
