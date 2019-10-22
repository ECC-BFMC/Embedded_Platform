/**
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