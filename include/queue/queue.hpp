/**
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

#include "queue.inl"

#endif
