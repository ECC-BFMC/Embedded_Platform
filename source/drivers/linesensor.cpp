#include "periodics/alerts.hpp"
#include "drivers/linesensor.hpp"

// TODO: Add your code here
namespace periodics
{
   /**
    * @brief Class constructor linesensor
    *
    */
    CLineSensor::CLineSensor(std::chrono::milliseconds f_period, PinName f_sensor_pin, float f_threshold)
    : utils::CTask(f_period),
      m_sensor(f_sensor_pin),
      m_threshold(f_threshold)
    {
        /* constructor behaviour */
    }

    /** @brief  CLineSensor class destructor
     */
    CLineSensor::~CLineSensor()
    {
    };

    void CLineSensor::_run()
    {
        if (m_sensor.read() > m_threshold) {
            g_alerts.startBeep();
        } else {
            g_alerts.stopBeep();
        }
    }

}; // namespace periodics