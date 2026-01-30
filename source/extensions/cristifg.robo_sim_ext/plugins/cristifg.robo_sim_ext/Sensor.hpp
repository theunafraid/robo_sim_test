#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

#include <concepts>
#include <cstdint>
#include <type_traits>
#include <string_view>

#include <carb/PluginUtils.h>
#include <pxr/usd/usd/stage.h>
#include <omni/physx/IPhysx.h>
#include <carb/InterfaceUtils.h>
#include <PxPhysicsAPI.h>

#include <isaacsim/sensors/physics/ImuSensor.h>

enum SENSOR_TYPE {
    UNINITIALIZED,
    ACCELEROMETER,
    GYROSCOPE,
    UNKNOWN,
};

template<typename T>
concept IdentifiableSensor = requires(const T& sensor) {
    typename T::SensorId_t;
    { sensor.id() } -> std::convertible_to<typename T::SensorId_t>;
};

template<typename SS>
concept SinkableSensor = requires(SS& s, typename SS::SensorId_t sid) {
    { s.OnData(sid) };
};

template<typename S>
concept SubscriberSensor = IdentifiableSensor<S> && requires(S& s, typename S::SensorId_t sid) {
    { s.subscribe(sid) } -> std::same_as<std::int8_t>;
    { s.unsubscribe(sid) } -> std::same_as<std::int8_t>;
};

struct ISensor {
    virtual void syncSensorData() = 0;
    virtual const std::string& getSensorPath() = 0;
};

struct Accelerometer;
struct Gyroscope;

using namespace carb;

template<typename SS, typename SI>
requires SinkableSensor<SS> && IdentifiableSensor<SI>
struct SensorController : public ISensor {
    std::int8_t subscribe(typename SI::SensorId_t sid) { return -1; }
    std::int8_t unsubscribe(typename SI::SensorId_t) { return -1; }

    SensorController(SENSOR_TYPE type,
                     const std::string& sensorPath,
                     omni::physx::IPhysx* physx)
        :m_sensorType(type),
        m_sensorPath(sensorPath),
        m_physx(physx) {}

    SensorController(const std::string& sensorPath,
                     omni::physx::IPhysx* physx):
        m_sensorType(determineSensorType()),
        m_sensorPath(sensorPath),
        m_physx(physx) {}

    void syncSensorData() {}

    const std::string& getSensorPath() {
        return m_sensorPath;
    }

private:
    SENSOR_TYPE                                 m_sensorType;
    SS                                          m_sensorSink;
    const std::string&                          m_sensorPath;

    omni::physx::IPhysx*                        m_physx;

    static constexpr SENSOR_TYPE determineSensorType() {
        if constexpr (std::is_same_v<SS, Accelerometer>) {
            return SENSOR_TYPE::ACCELEROMETER;
        }

        return SENSOR_TYPE::GYROSCOPE;
    }
};

#endif
