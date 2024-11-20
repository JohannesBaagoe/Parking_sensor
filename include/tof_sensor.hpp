#include "Adafruit_VL53L0X.h"

class ToF{
    private:
        Adafruit_VL53L0X lox;
        VL53L0X_RangingMeasurementData_t measure;

    public:
        ToF();
        bool init(bool lastState, byte VL53LOX_ShutdownPin);
        uint16_t getMeasurement();
        VL53L0X_Error SetInteruptThresholds(int low_thres, int high_thres); // skal være i mm
        void clearInterrupt();
        void startRangeContinuous(int ms, int microSec);
};