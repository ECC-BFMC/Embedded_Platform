#ifndef LINESENSOR_HPP
#define LINESENSOR_HPP

// TODO: Add your code here

#include <mbed.h>
#include <chrono>
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
                PinName f_sensor_pin
            );
            /* Destructor */
            ~CLineSensor();

        private:
            /* private variables & method member */
            DigitalIn m_sensor;

            /* Run method */
            virtual void _run();

    }; // class CLineSensor
} // namespace periodics

#endif // LINESENSOR_HPP
