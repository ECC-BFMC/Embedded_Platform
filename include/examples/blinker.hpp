/**
  ******************************************************************************
  * @file    Blinker.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the blinker
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef BLINKER_HPP
#define BLINKER_HPP

/* The mbed library */
#include <mbed.h>
#include <TaskManager/taskmanager.hpp>


namespace examples{

   /**
    * @brief It is used for toggling an LED.
    * 
    */
    class CBlinker : public task::CTask
    {
        public:
            /* Construnctor */
            CBlinker(uint32_t f_period, DigitalOut f_led);
        private:
            /* Run method */
            virtual void _run();
            /* Digital output line connected to a LED */
            DigitalOut m_led;    
    };
}; // namespace examples

#endif