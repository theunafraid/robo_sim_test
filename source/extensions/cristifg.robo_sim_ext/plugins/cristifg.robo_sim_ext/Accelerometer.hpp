#include "Sensor.hpp"

#include <cstdint>
#include <memory>

using namespace std;

struct Accelerometer {
    using SensorId_t = uint32_t;

    SensorId_t id() const { return 0; }
};

struct AccelerometerSink {
    using SensorId_t = Accelerometer::SensorId_t;

    void OnData(SensorId_t sid) {

    }
};

using AccelerometerSensor = SensorController<AccelerometerSink, Accelerometer>;
using UniqueAccelerometerSensor = unique_ptr<AccelerometerSensor>;
