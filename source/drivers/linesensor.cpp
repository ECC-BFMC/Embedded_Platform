#include "periodics/alerts.hpp"
#include "drivers/linesensor.hpp"

// TODO: Add your code here
namespace periodics
{
   /**
    * @brief Class constructor linesensor
    *
    */
        CLineSensor::CLineSensor(std::chrono::milliseconds f_period, PinName f_sensor_pin, UnbufferedSerial& f_serial, const std::string& f_name)
        : utils::CTask(f_period),
            m_sensor(f_sensor_pin),
            m_serial(f_serial),
            m_isActive(false),
            m_name(f_name)
    {
        /* configure internal pull to avoid floating reads */
        m_sensor.mode(PullUp);
    }

    /** @brief  CLineSensor class destructor
     */
    CLineSensor::~CLineSensor()
    {
    };

    void CLineSensor::serialCallbackLINEcommand(char const * a, char * b)
    {
        uint8_t l_isActivate = 0;
        uint8_t l_res = sscanf(a, "%hhu", &l_isActivate);
        if (1 == l_res) {
            m_isActive = (l_isActivate >= 1);
            sprintf(b, "%hhu", l_isActivate);
        } else {
            sprintf(b, "syntax error");
        }
    }

    void CLineSensor::_run()
    {
        int val = !m_sensor.read();
        // if (val == 1) {
        //     g_alerts.startBeep();
        // } else {
        //     g_alerts.stopBeep();
        // }

        if (!m_isActive) return;

        /* Print the current line sensor value over serial in the project's message format */
        char buf[64];
        snprintf(buf, sizeof(buf), "@%s:%d;;\r\n", m_name.c_str(), val);
        m_serial.write(buf, strlen(buf));
    }

}; // namespace periodics