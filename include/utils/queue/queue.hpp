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
  * @file    Queue.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the queue
  *          functionality.
  ******************************************************************************
 */
/* Include guard */
#ifndef QUEUE_HPP
#define QUEUE_HPP


namespace utils{
    


/**
 * @brief It is used for executing queue container.
 * 
 * @tparam T The type of the varieble
 * @tparam N The size of the queue
 */
template <class T, unsigned int N>
class CQueue
{
public:
    /* Constructor */
    CQueue();
    /* Destructor */
    virtual ~CQueue();
    /* Is full method */
    inline bool isFull();
    /* Is full method */
    inline bool isEmpty();
    /* Peek */
    inline T peek();
    /* Pop */
    inline T pop();
    /* Get queue size */
    inline unsigned int getSize();
    /* Push single element */
    inline void push(T& f_char);
    /* Push multiple elements */
    inline void push(T *f_char, unsigned int f_len);
    /* Empty queue */ 
    inline void empty();
private:
    /* buffer */
    volatile T m_buffer[N];
    /* start */
    volatile unsigned int  m_start;
    /* end */
    volatile unsigned int  m_end;
    /* size */
    volatile unsigned int  m_size;
};

#include "queue.tpp"

}; // namespace utils

#endif
