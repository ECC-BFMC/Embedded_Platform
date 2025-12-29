#ifndef LINESENSOR_HPP
#define LINESENSOR_HPP

// TODO: Add your code here

#include <mbed.h>
#include <chrono>
#include <string>
#include <utils/task.hpp>

namespace periodics { class CAlerts; }
extern periodics::CAlerts g_alerts;

namespace periodics
{
   /**
    * @brief Class linesensor
    *
    */
    class CLineSensor: public utils::CTask
    {
        public:
            /* Constructor */
            CLineSensor(
                std::chrono::milliseconds f_period,
                PinName f_sensor_pin,
                UnbufferedSerial& f_serial,
                const std::string& f_name
            );
            /* Destructor */
            ~CLineSensor();

            /* Serial control callback: enable/disable periodic printing */
            void serialCallbackLINEcommand(char const * a, char * b);
            std::string m_name;
            
        private:
            /* private variables & method member */
            DigitalIn m_sensor;
            UnbufferedSerial& m_serial;
            /** @brief Active flag for serial publishing */
            bool m_isActive;

            /* Run method */
            virtual void _run();

    }; // class CLineSensor
} // namespace periodics

#endif // LINESENSOR_HPP
