#include "tof_sensor.hpp"
#include <Wire.h>


    ToF::ToF() :lox(){}

    bool ToF::init(bool lastState, byte VL53LOX_ShutdownPin) {
       // Hvis lox.begin fejler, skyldes det muligvis, at sensoren er i
        // kontinuerlig målingstilstand. Vi nulstiller enheden.
        while (!lox.begin()) {
            Serial.println(F("Failed to boot VL53L0X"));
            digitalWrite(VL53LOX_ShutdownPin, LOW);
            delay(100);
            digitalWrite(VL53LOX_ShutdownPin, HIGH);
            Serial.println("Adafruit VL53L0X XShut set high to Allow Boot");
            delay(100);
        }
        // Indstil GPIO konfiguration for interrupt
        VL53L0X_Error status;
        if (lastState == true){
            //Serial.println("Set GPIO Config so if range is lower the LowThreshold trigger Gpio Pin ");
            status = lox.setGpioConfig(VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING,
                      VL53L0X_GPIOFUNCTIONALITY_THRESHOLD_CROSSED_LOW,
                      VL53L0X_INTERRUPTPOLARITY_LOW);
            lastState = false;
        } 
        else if(lastState == false){
            //Serial.println("Set GPIO Config so if range is higher than HighThreshold trigger Gpio Pin ");
            status = lox.setGpioConfig(VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING,
                        VL53L0X_GPIOFUNCTIONALITY_THRESHOLD_CROSSED_HIGH,
                        VL53L0X_INTERRUPTPOLARITY_LOW);
            lastState = true;
        }
        
        if (status != VL53L0X_ERROR_NONE) {
            Serial.println(F("Fejl ved indstilling af GPIO-konfiguration!"));
        }

        return lastState;

    } 
    
    
    uint16_t ToF::getMeasurement(){

        if (lox.isRangeComplete()) {
            return lox.readRange();
        } else {
            return 0;
        }
    };

    VL53L0X_Error ToF::SetInteruptThresholds(int low_thres, int high_thres){ // skal være i mm

        // Indstil interrupt thresholds
        FixPoint1616_t LowThreshold = (low_thres * 65536.0);
        FixPoint1616_t HighThreshold = (high_thres * 65536.0);
        Serial.println("Set Interrupt Thresholds... ");
        VL53L0X_Error status = lox.setInterruptThresholds(LowThreshold, HighThreshold, true);

        if (status != VL53L0X_ERROR_NONE) {
            Serial.println(F("Fejl ved indstilling af Thresholds"));
        } else{
            Serial.println("Det lykkedes at indstille Thresholds");
        }

        return status;

    }

    void ToF::clearInterrupt(){
        lox.clearInterruptMask(false);
    }

    void ToF::startRangeContinuous(int miliSec, int microSec){
        lox.setMeasurementTimingBudgetMicroSeconds(microSec);
        Serial.println("StartMeasurement... ");
        lox.startRangeContinuous(miliSec);
    }

    void ToF::startHiberNation(){

        //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
        esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC8M, ESP_PD_OPTION_OFF);
        esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF);
        esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);

        esp_deep_sleep_start();

    }