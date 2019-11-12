/**
Copyright 2019 Bosch Engineering Center Cluj and BFMC organizers 

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
  
  
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
#include <utils/taskmanager/taskmanager.hpp>


namespace examples{

   /**
    * @brief It is used for toggling a LED.
    * 
    */
    class CBlinker : public utils::task::CTask
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