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
    * @brief It has aim to the task functionality. The tasks will be applied periodically by the task manager, the period is defined in the contructor. 
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
        /* main app. logic - It's a pure function for application logic and has to override in the derivered class to implement the appl. */
        virtual void _run() = 0;
        /* period */
        const uint32_t m_period;
        /* ticks */
        uint32_t m_ticks;
        /* trigger flag */
        bool m_triggered;
    };

   /**
    * @brief It has aim to implement the task manager functionality. It controls and applies periodically each task. 
    * It has two main part, a ticker and the mainCallback method. The ticker method applies automatically 'timerCallback' method of each Task, as 
    * numerate se
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
        void timerCallback()
        {
            for(uint32_t i = 0; i < m_taskCount; i++)
            {
                m_taskList[i]->timerCallback();
            }
        }
    private:
        /* task list */
        CTask** m_taskList;
        /* task count */
        uint32_t m_taskCount;
        /* ticker */ 
        Ticker m_ticker;
    };

}; // namespace task

#endif