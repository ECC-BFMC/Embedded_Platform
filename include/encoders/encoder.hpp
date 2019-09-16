/**
  ******************************************************************************
  * @file    Encoder.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the encoder
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef MAG_ENCODER_HPP
#define MAG_ENCODER_HPP

#include <mbed.h>
#include<TaskManager/taskmanager.hpp>
#include<Encoders/encoderinterface.hpp>

//!  CCounter class. 
/*!
 * It is used for counting encoder steps.
 */
class CCounter
{
    public:
        /* Constructor */
        CCounter(PinName f_pin);
        /* Reset method */
        void reset();
        /* Increment method */
        void increment();
        /* Get counts method */
        int32_t getCount();
        /* Increment */
        static void staticIncrement(void* obj);
    private:
        /* Interrupt object */
        InterruptIn m_interrupt;
        /* Counts member */
        volatile int32_t m_count;
};

//!  CEncoder class. 
/*!
 * It inherits class task::CTask. 
 * It is used for computing revolustions per second.
 */
class CEncoder: public task::CTask, public:: encoders::IEncoderGetter
{
    public:
        /* Constructor */
        CEncoder(uint32_t    f_period
                ,float      f_period_sec
                ,uint32_t   f_cpr
                ,PinName    f_pinName);
        /* Get rotations per second */
        virtual float getSpeedRps();
        virtual int16_t  getCount();
        virtual bool isAbs(){return true;}

    protected:
        /* Run method */
        virtual void _run();

        /* Counter values */
        CCounter        m_counter;
        /* Period in seconds */
        const float     m_period_sec;
        /* Value of counts per revolution */
        const uint32_t  m_cpr;//count per revolution (rise and fall edges)
        /* Value of rotation per second */
        float           m_rps;
};

//!  CMagEncoderTime class. 
/*!
 * It is used for computing high/low periods.
 */
class CMagEncoderTime
{
    public:
        /* Constructor */
        CMagEncoderTime(PinName f_pin);
        /* Callback for rising edge interrupt */
        void riseCallback();
        /* Callback for falling edge interrupt */
        void fallCallback();
        /* Callback for rising edge interrupt attached to pbject */
        static void staticRise(void* obj);
        /* Callback for falling edge interrupt attached to pbject */
        static void staticFall(void* obj);
        /* Get high time */
        float getHighTime();
        /* Get low time */
        float getLowTime();
    private:
        /* General purpose timer */
        Timer       m_Timer,m_fallTimer;
        /* Digital interrupt input */
        InterruptIn m_interrupt;
        /* High time */
        volatile float      m_highTime;
        /* Low time */
        volatile float      m_lowTime;
};
#endif