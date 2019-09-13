/**
  ******************************************************************************
  * @file    Encoder.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementations for the encoder
  *          functionality.
  ******************************************************************************
 */

#include <Encoders/encoder.hpp>

/** \brief  Constructor for the CCounter class
 *
 *  Constructor method
 *
 *  @param f_pin       digital pin connected to encoder output    
 */
CCounter::CCounter(PinName f_pin)
    :m_interrupt(f_pin)
    ,m_count(0)
{
    m_interrupt.rise(mbed::callback(CCounter::staticIncrement,this));
    m_interrupt.fall(mbed::callback(CCounter::staticIncrement,this));
}

/** \brief  Counter reset method
 * 
 *  
 *  
 */
void CCounter::reset(){
    m_count=0;
}

/** \brief  Counter increment method
 * 
 *  
 *  
 */
void CCounter::increment(){
    m_count++;
}

/** \brief  Get counts method
 * 
 *  
 *  @return    counts
 */
int32_t CCounter::getCount(){
    return m_count;
}

/** \brief  Get counts method
 * 
 *  Method attaches the increment method to an object.
 * 
 *  @param obj            object
 *  
 */
void CCounter::staticIncrement(void* obj){
    CCounter* self=static_cast<CCounter*>(obj);
    self->increment();
}

/** \brief  Constructor for the CEncoder class
 *
 *  Constructor method
 *
 *  @param f_period       period value 
 *  @param f_period_sec   period value in seconds   
 *  @param f_cpr          counts per revolution (rise and fall edges)
 *  @param f_pinName      digital pin connected to encoder output
 */
CEncoder::CEncoder(uint32_t   f_period
                  ,float      f_period_sec
                  ,uint32_t   f_cpr
                  ,PinName    f_pinName)
    :task::CTask(f_period)
    ,m_counter(f_pinName)
    ,m_period_sec(f_period_sec)
    ,m_cpr(f_cpr)
    ,m_rps(0)
{
}

/** \brief  Get rps method
 *
 *  Returns the value of the rotations per second value
 *
 *  
 *  @return     rps   
 */
float CEncoder::getSpeedRps()
{
    return m_rps;
}

/** \brief  Get count
 *
 *  Returns the value of the count
 *
 *  
 *  @return     rps   
 */
int16_t CEncoder::getCount()
{
    return m_rps*m_period_sec*m_cpr;
}



/** \brief  Run method
 *
 *  Method executed at predefined time intervals. 
 *  Method called each f_period
 *
 *  
 */
void CEncoder::_run()
{
    float l_count=m_counter.getCount();
    m_counter.reset();
    m_rps=l_count/m_period_sec/m_cpr;
}

/** \brief  Constructor for the CMagEncoderTime class
 *
 *  Constructor method
 *
 *  @param f_pin       digital pin connected to encoder output    
 */
CMagEncoderTime::CMagEncoderTime(PinName f_pin)
    :m_interrupt(f_pin)
{
    m_interrupt.rise(mbed::callback(CMagEncoderTime::staticRise,this));
    // m_interrupt.fall(mbed::callback(CMagEncoderTime::staticFall,this));
    m_Timer.start();
}

/** \brief  Callback method  for rising edge interrupt
 *
 *  
 */
void CMagEncoderTime::riseCallback()
{
    m_Timer.stop();
    m_lowTime=m_Timer.read();
    m_Timer.reset();
    m_Timer.start();
}

/** \brief  Callback method for rising edge interrupt
 *
 *  
 */
void CMagEncoderTime::fallCallback()
{
    m_Timer.stop();
    m_highTime=m_Timer.read();
    m_Timer.reset();
    m_Timer.start();
}

/** \brief  Callback method for rising edge interrupt attached to pbject
 *
 *  @param obj        object
 */
void CMagEncoderTime::staticRise(void* obj)
{
    CMagEncoderTime* self=static_cast<CMagEncoderTime*>(obj);
    self->riseCallback();
}

/** \brief  Callback method for rising edge interrupt attached to pbject
 *
 *  @param obj        object
 *  @return     None   
 */
void CMagEncoderTime::staticFall(void* obj)
{
    CMagEncoderTime* self=static_cast<CMagEncoderTime*>(obj);
    self->fallCallback();
}

/** \brief  Callback method for rising edge interrupt
 *
 *  
 *  @return     High period value   
 */
float CMagEncoderTime::getHighTime()
{
    return m_highTime;
}

/** \brief  Callback method for rising edge interrupt
 *
 *  @return     Low period value   
 */
float CMagEncoderTime::getLowTime()
{
    return m_lowTime;
}