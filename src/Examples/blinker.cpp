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

#include <Examples/blinker.hpp>


namespace examples{
    /** \brief  Class constructor
     *
     *  Constructor method
     *
     *  \param f_period       LED toggling reading period
     *  \param f_led          Digital output line to which the LED is connected     
     */
    CBlinker::CBlinker(uint32_t f_period, DigitalOut f_led) 
        : task::CTask(f_period)
        , m_led(f_led) 
    {
        m_led = 1;
    }

    /** \brief  Method called each f_period
     * 
     */
    void CBlinker::_run()
    {
        m_led = !m_led;
    }

}; // namespace examples