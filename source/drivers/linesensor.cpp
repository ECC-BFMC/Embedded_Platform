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
            m_name(f_name),
            m_isActive(false),
            m_lastState(0)
    {
        /* configure internal pull to avoid floating reads */
        m_sensor.mode(PullUp);
    }

    /** @brief  CLineSensor class destructor
     */
    CLineSensor::~CLineSensor()
    {
    };

   void CLineSensor::serialCallbackLINEcommand(char const * a, char * b) {
       uint8_t l_isActivate = 0;
       uint8_t l_res = sscanf(a, "%hhu", &l_isActivate);
       
       if(1 == l_res) {
           m_isActive = (l_isActivate >= 1);
           sprintf(b, "1");
       } else {
           sprintf(b, "syntax error");
       }
   }

     void CLineSensor::_run() {
       if (!m_isActive) return;
       
       bool currentState = (m_sensor.read() == 0);  // true = line detected
       
       if (currentState != m_lastState) {
           char buffer[32];
           snprintf(buffer, sizeof(buffer), "@stopLine:%d;;\r\n", currentState ? 1 : 0);
           m_serial.write(buffer, strlen(buffer));
           
           m_lastState = currentState;
       }
   }

}; // namespace periodics