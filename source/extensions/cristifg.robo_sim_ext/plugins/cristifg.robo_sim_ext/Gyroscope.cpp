#include "Gyroscope.hpp"

#include <carb/logging/Log.h>
#include <carb/Framework.h>
#include <carb/InterfaceUtils.h>
#include <omni/physx/IPhysx.h>

template<>
std::int8_t GyroscopeSensor::subscribe(Gyroscope::SensorId_t sid) {
    return 0;
}
