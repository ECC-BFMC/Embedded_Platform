/**
  ******************************************************************************
  * @file    Timer.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the timer functionality.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef TIMER_HPP
#define TIMER_HPP

#include <mbed.h>

/**
 * @brief It is used for implementing timers.
 * 
 * @tparam N 
 * @tparam 1 
 */
template <unsigned int N, unsigned int D = 1>
class CTimer
{
public:
    /* Constructor */
    CTimer();
    /* Destructor */
    virtual ~CTimer();
    /* Start timer */
    inline void start();
    /* Stop timer */
    inline void stop() ;
    /* Get timer */
    inline uint32_t get();
private:
    /* Timer callback */
    static void callback(void *thisPointer);
    /* Increase milliseconds value */
    void millisTicker();
    /* Ticker member */
    Ticker m_ticker;
    /* Milliseconds value */
    volatile uint32_t m_millisValue;
    /* Flag indicating timer started */
    bool m_started;
};

typedef CTimer<1,1000> CTimer_ms;
typedef CTimer<1,1000000> CTimer_us;
typedef CTimer<1,10000> CTimer_100us;
typedef CTimer<60> CTimer_min;

/* Inline implementation */
#include "timer.inl"

#endif