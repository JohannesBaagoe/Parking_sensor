#include "tof_sensor.hpp"
#include <Wire.h>

    ToF::ToF() {
    };

    ToF::ToF(int num) : lox() {
        if (!lox.begin()) {
            Serial.println(F("Failed to boot VL53L0X"));
        }


        VL53L0X_Error status = lox.setGpioConfig(VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, VL53L0X_GPIOFUNCTIONALITY_THRESHOLD_CROSSED_HIGH, VL53L0X_INTERRUPTPOLARITY_HIGH);
        //FixPoint1616_t low = (int)100*(1<<16);
        //FixPoint1616_t high = (int)1000*(1<<16);
       /*VL53L0X_Error status =  lox.setInterruptThresholds(low, high, true);

        if(status == VL53L0X_ERROR_NONE){
            Serial.println("Lykkedes");
        }else{
            Serial.println("fejl");
        }*/ 

        //lox.setDeviceMode(VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, true);

        //lox.startRangeContinuous();

        //lox.startMeasurement(true);
    }
    
    uint16_t ToF::getMeasurement(){

        //lox.rangingTest(&measure, false);

        if (lox.isRangeComplete()) {
            return lox.readRange();
        } else {
            return 0;
        }
    };


    VL53L0X_Error ToF::SetInteruptThresholds(int low_thres, int high_thres){ // skal være i mm

        FixPoint1616_t low = (int)low_thres*(1<<16);
        FixPoint1616_t high = (int)high_thres*(1<<16);

        VL53L0X_Error status = lox.setInterruptThresholds(low, high, false);

        return status;

    }

    void ToF::clearInterrupt(){
        lox.clearInterruptMask(false);
    }