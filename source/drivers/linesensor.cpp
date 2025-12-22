#include "periodics/alerts.hpp"
#include "drivers/linesensor.hpp"

// TODO: Add your code here
namespace periodics
{
   /**
    * @brief Class constructor linesensor
    *
    */
    CLineSensor::CLineSensor(std::chrono::milliseconds f_period, PinName f_sensor_pin)
    : utils::CTask(f_period),
      m_sensor(f_sensor_pin)
    {
        /* configure internal pull to avoid floating reads */
        m_sensor.mode(PullUp);
    }

    /** @brief  CLineSensor class destructor
     */
    CLineSensor::~CLineSensor()
    {
    };

    void CLineSensor::_run()
    {
        if (m_sensor.read() == 0) {
            g_alerts.startBeep();
        } else {
            g_alerts.stopBeep();
        }
    }

}; // namespace periodics