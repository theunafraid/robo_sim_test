#include "Sensor.hpp"

#include <cstdint>
#include <memory>

using namespace std;

struct Gyroscope {
    using SensorId_t = uint32_t;

    SensorId_t id() const { return 0; }
};

struct GyroscopeSink {
    using SensorId_t = Gyroscope::SensorId_t;

    void OnData(SensorId_t sid) {

    }
};

using GyroscopeSensor = SensorController<GyroscopeSink, Gyroscope>;
using UniqueGyroscopeSensor = unique_ptr<GyroscopeSensor>;

template<>
std::int8_t GyroscopeSensor::subscribe(Gyroscope::SensorId_t sid);


