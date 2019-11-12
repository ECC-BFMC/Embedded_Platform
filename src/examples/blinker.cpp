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
  * @file    Blinker.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the blinker
  *          functionality.
  ******************************************************************************
 */

#include <examples/blinker.hpp>


namespace examples{
    /** \brief  Class constructor
     *
     *  It initializes the task and the state of the led. 
     *
     *  \param f_period       Toggling period of LED
     *  \param f_led          Digital output line to LED
     */
    CBlinker::CBlinker(uint32_t f_period, DigitalOut f_led) 
        : utils::task::CTask(f_period)
        , m_led(f_led) 
    {
        m_led = 1;
    }

    /** \brief  Periodically applied method to change the LED's state
     * 
     */
    void CBlinker::_run()
    {
        m_led = !m_led;
    }

}; // namespace examples