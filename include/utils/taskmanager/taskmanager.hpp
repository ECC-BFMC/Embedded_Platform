/**
 * Copyright (c) 2019, Bosch Engineering Center Cluj and BFMC organizers
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:

 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

/* Inclusion guard */
#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <mbed.h>

namespace utils::task{
   /**
    * @brief It aims to the task functionality. The tasks will be applied periodically by the task manager, the period is defined in the contructor. 
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
         /** @brief  Timer callback */
        void timerCallback()
        {
            m_ticks++;
            if (m_ticks >= m_period)
            {
                m_ticks = 0;
                Trigger();
            }
        }
         /** @brief  Trigger function to set the flag true state. */
        void Trigger()
        {
            m_triggered = true;
        }
    protected:
        /** @brief  main application logic - It's a pure function for application logic and has to override in the derivered class to implement the appl.*/
        virtual void _run() = 0;
        /** @brief period of the task */
        const uint32_t m_period;
        /** @brief  ticks */
        uint32_t m_ticks;
        /** @brief  trigger flag */
        bool m_triggered;
    };

   /**
    * @brief It aims to implement the task manager functionality. It controls and applies periodically each task. 
    * It has two main part, a ticker and the mainCallback method. The ticker method applies automatically 'timerCallback' method of each tasks, so
    * numerate separately the ticks from the functionalities of tasks. The mainCallback method aims to apply the application logic for each tasks, 
    * if the task's trigger flag has true state. 
    */
    class CTaskManager
    {
    public:
        /* Constructor */
        CTaskManager(CTask** f_taskList, uint32_t f_taskCount, float f_baseFreq);
        /* Destructor */
        virtual ~CTaskManager();
        /** @brief  The main callback method aims to apply the subtasks' run method. */
        inline void mainCallback()
        {
            for(uint32_t i = 0; i < m_taskCount; i++)
            {
                m_taskList[i]->run();
            }
        }
        /** @brief  Timer callback method applies the subtasks' callback function. */
        void timerCallback()
        {
            for(uint32_t i = 0; i < m_taskCount; i++)
            {
                m_taskList[i]->timerCallback();
            }
        }
    private:
        /** @brief  List of tasks  */
        CTask** m_taskList;
        /** @brief  number of tasks */
        uint32_t m_taskCount;
        /** @brief  Ticker for periodic applying the timer callback function  */
        Ticker m_ticker;
    };

}; // namespace utils::task

#endif