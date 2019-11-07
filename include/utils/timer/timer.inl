/**
Copyright 2019 Bosch Engineering Center Cluj and BFMC orginazers 

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
  * @file    Timer.inl
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the timer functionality.
  *          Inline implementation.
  ******************************************************************************
 */

/** @brief  CTimer class constructor
 *
 *  Constructor method
 *
 *  
 */
template <unsigned int N, unsigned int D>
CTimer<N,D>::CTimer()
    : m_ticker()
    , m_millisValue(0)
    , m_started(0)
{
}

/** @brief  CTimer class destructor
 *
 *  Destructor method
 *
 *  
 */
template <unsigned int N, unsigned int D>
CTimer<N,D>::~CTimer() 
{ 
    if (m_started) 
        stop ();
}

/** @brief  Start timer
 *
 *  
 *  
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::start () 
{
    m_started = 1;
    m_ticker.attach (mbed::callback(&CTimer::callback, this), 1.f*N/D);  
}
    
/** @brief  Stop timer
 *
 *  
 *  
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::stop() 
{
    m_started = 0;
    m_ticker.detach ();
}

/** @brief  Get timer
 *
 *  
 *  @return    Milliseconds value
 */
template <unsigned int N, unsigned int D>
uint32_t CTimer<N,D>::get()
{
    return m_millisValue;
}

/** @brief  Timer callback
 *
 *  @param thisPointer   The object pointer.
 *  
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::callback(void *thisPointer)
{
    CTimer<N,D>* self = static_cast<CTimer<N,D>*>(thisPointer);
    self->millisTicker(); 
}

/** @brief Increase milliseconds value
 *
 *  
 *  
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::millisTicker()
{
    m_millisValue ++;
}