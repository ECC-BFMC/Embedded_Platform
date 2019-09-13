/**
  ******************************************************************************
  * @file    Echoer.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the serial echoer
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef ECHOER_HPP
#define ECHOER_HPP

/* The mbed library */
#include <mbed.h>
#include <TaskManager/taskmanager.hpp>


namespace examples{
   /**
    * @brief It is used for echoing messages over UART.
    * 
    */
    class CEchoer : public task::CTask
    {
        public:
            /* Construnctor */
            CEchoer(uint32_t f_period, Serial& f_serialPort);
        private:
            /* Run method */
            virtual void _run();

            /* Serial communication member*/
            Serial& m_serialPort; 
    };

}; // namespace examples

#endif