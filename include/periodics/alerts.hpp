#ifndef ALERTS_HPP
#define ALERTS_HPP

// TODO: Add your code here
#include <mbed.h>
#include <chrono>
#include <utils/task.hpp>

namespace periodics
{
   /**
    * @brief Class alerts
    *
    */
    class CAlerts: public utils::CTask
    {
        public:
            /* Construnctor */
            CAlerts(
                std::chrono::milliseconds f_period
            );
            /* Destructor */
            ~CAlerts();

            void alertsCommand(char const * a, char * b);
        private:
            /* private variables & method member */

            /* Run method */
            virtual void        _run();

            /** @brief Active flag  */
            bool            m_isActive;

            uint16_t m_period;

            PwmOut buzzer;

            uint8_t current_step;
            uint8_t step_counter;
            bool tone_active;
            bool warning_active;

            uint8_t alert_id;

    }; // class CAlerts
}; // namespace periodics

#endif // ALERTS_HPP
