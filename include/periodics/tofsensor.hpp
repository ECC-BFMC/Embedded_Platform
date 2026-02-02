#pragma once

#include "mbed.h"
#include "VL53L0X.h"
#include "task.hpp"   // make sure this is the correct path

namespace periodics {

// Ensure namespace is correct
class CTofsensor : public utils::CTask {
    public:
        // Constructor
        CTofsensor(
                std::chrono::milliseconds period,
                PinName xshut_pin,
                PinName sda,
                PinName scl,
                mbed::UnbufferedSerial& serial,
                const std::string& name,
                uint8_t address,
                uint16_t threshold_mm = 200
        );
        // Destructor
        ~CTofsensor();
        // Initialize the sensor
        void serialCallbackTofsensorCommand(char const * a, char * b);
        std::string m_name;

    private:
        DigitalOut m_xshut;
        I2C m_i2c;
        VL53L0X m_sensor;

        mbed::UnbufferedSerial& m_serial;
        bool m_isActive = false;
        uint8_t m_address;
        uint16_t m_threshold_mm;

        void _run() override;

    };

} // namespace periodics
