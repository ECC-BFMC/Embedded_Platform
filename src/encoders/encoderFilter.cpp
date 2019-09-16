/**
  ******************************************************************************
  * @file    EncoderFilter.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the encoder filter
  *          functionality.
  ******************************************************************************
 */

#include <Encoders/encoderFilter.hpp>

/** \brief  Constructor for the CEncoderFilter class
 *
 *  Constructor method
 *
 *  \param[in] f_period       period value 
 *  \param[in] f_period_sec   period value in seconds   
 *  \param[in] f_cpr          counts per revolution (rise and fall edges)
 *  \param[in] f_pinName      digital pin connected to encoder output
 *  \param[in] f_filter       reference to filter object
 */
CEncoderFilter::CEncoderFilter(uint32_t                       f_period
                              ,float                          f_period_sec
                              ,uint32_t                       f_cpr
                              ,PinName                        f_pinName
                              ,CFilterFunction<float>&        f_filter)
    :CEncoder(f_period,f_period_sec,f_cpr,f_pinName)
    ,m_filter(f_filter)
{
}

/** \brief  Get filtered rps value
 *
 *  Method for getting filtererd rotations per second value
 *
 *  \return filtered rps value
 */
float CEncoderFilter::getSpeedRps(){
    return m_filteredRps;
}

/**
 * @brief Getter filtered count value
 * 
 * @return int16_t 
 */
int16_t  CEncoderFilter::getCount(){
     return m_filteredRps*m_period_sec*m_cpr;
}

/** \brief  Run method
 *
 *  Method executed at predefined time intervals. 
 *  Method called each f_period
 * 
 */
void CEncoderFilter::_run()
{
    float l_count=m_counter.getCount();
    m_counter.reset();
    m_rps=static_cast<float>(l_count)/m_period_sec/m_cpr;
    m_filteredRps=m_filter(m_rps);
}
