#include "tofsensor.hpp"

namespace periodics 
{

CTofsensor::CTofsensor(std::chrono::milliseconds period,
                       PinName xshut_pin,
                       PinName sda,
                       PinName scl,
                       mbed::UnbufferedSerial& serial,
                       const std::string& name,
                       uint8_t address,
                       uint16_t threshold_mm)
    : utils::CTask(period),
      m_xshut(xshut_pin),
      m_i2c(sda, scl),
      m_sensor(&m_i2c),
      m_serial(serial),
      m_name(name),
      m_address(address),
      m_threshold_mm(threshold_mm)
    {
        m_i2c.frequency(400000);  // Set I2C to 400kHz
        m_xshut = 0;
    }

CTofsensor::~CTofsensor()
{
}

void CTofsensor::serialCallbackTofsensorCommand(char const * a, char * b)
{
    uint8_t l_isActivate = 0;
    uint8_t l_res = sscanf(a, "%hhu", &l_isActivate);

    if (l_res == 1) {
        m_isActive = (l_isActivate >= 1);

        if (m_isActive) {
            // Power on sensor
            m_xshut = 1;

            // VL53L0X needs time to boot after XSHUT release
            ThisThread::sleep_for(chrono::milliseconds(100));

            // Initialize VL53L0X
            if (m_sensor.init()) {
                // Optionally change address (m_address is 7-bit)
                if (m_address != 0x29) {
                    m_sensor.setAddress(m_address);
                }
                // Set timing budget (33ms default, 20ms min)
                m_sensor.setMeasurementTimingBudget(33000);
                // Set timeout for reads
                m_sensor.setTimeout(500);
                // Don't start continuous - we'll use single-shot
                sprintf(b, "%hhu", l_isActivate);
            } else {
                m_isActive = false;
                sprintf(b, "ERROR:init failed");
            }
        } else {
            // Deactivate
            m_xshut = 0;
            sprintf(b, "%hhu", l_isActivate);
        }
    } else {
        sprintf(b, "syntax error");
    }
}


void CTofsensor::_run()
{
    if (!m_isActive) return;

    // Use single-shot measurement (blocking, but reliable)
    uint16_t distance = m_sensor.readRangeSingleMillimeters();
    
    // Check for timeout/error
    if (m_sensor.timeoutOccurred()) {
        return;  // Skip silently
    }

    // Only print if under threshold
    if (distance > 0 && (distance-70) < m_threshold_mm) {
        char buf[64];
        snprintf(buf, sizeof(buf),
                 "@%s:%u;;\r\n",
                 m_name.c_str(),
                 distance-70); //fiksna greska 70mm
        m_serial.write(buf, strlen(buf));
    }
}


} // namespace periodics
