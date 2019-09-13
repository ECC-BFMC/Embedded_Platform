/**
  ******************************************************************************
  * @file    TaskManager.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for task manager.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <mbed.h>

namespace task{
   /**
    * @brief It is used for implementing the task functionality.
    * 
    */
    class CTask
    {
    public:
        /* Constructor */
        CTask(uint32_t f_period);
        /* Destructor */
        virtual ~CTask();
        /* Run method */
        virtual void run();
        /* Timer callback */
        void timerCallback()
        {
            m_ticks++;
            if (m_ticks >= m_period)
            {
                m_ticks = 0;
                Trigger();
            }
        }
        /* Trigger */
        void Trigger()
        {
            m_triggered = true;
        }
    protected:
        /* _run methof */
        virtual void _run() = 0;
        /* period */
        const uint32_t m_period;
        /* ticks */
        uint32_t m_ticks;
        /* trigger flag */
        bool m_triggered;
    };

   /**
    * @brief It is used for implementing the task manager functionality.
    * 
    */
    class CTaskManager
    {
    public:
        /* Constructor */
        CTaskManager(CTask** f_taskList, uint32_t f_taskCount, float f_baseFreq);
        /* Destructor */
        virtual ~CTaskManager();
        /* Main callback */
        inline void mainCallback()
        {
            for(uint32_t i = 0; i < m_taskCount; i++)
            {
                m_taskList[i]->run();
            }
        }
        /* Timer callback */
        inline void timerCallback()
        {
            for(uint32_t i = 0; i < m_taskCount; i++)
            {
                m_taskList[i]->timerCallback();
            }
        }
    private:
        /* Callback */
        static void callback(void *thisPointer);
        /* task list */
        CTask** m_taskList;
        /* task count */
        uint32_t m_taskCount;
        /* ticker */ /* A Ticker is used to call a function at a recurring interval */
        Ticker m_ticker;
    };

}; // namespace task

#endif