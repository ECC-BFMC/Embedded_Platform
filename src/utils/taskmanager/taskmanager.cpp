/**
 * Copyright 2019 Bosch Engineering Center Cluj and BFMC organizers 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
  ******************************************************************************
  * @file    TaskManager.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for task manager.
  ******************************************************************************
 */
#include <utils/taskmanager/taskmanager.hpp>

namespace utils::task{

    /******************************************************************************/
    /** \brief  CTask class constructor
     *
     *  It initializes the period and other private value of the task. 
     *
     *  @param f_period      execution period
     */
    CTask::CTask(uint32_t f_period) 
        : m_period(f_period)
        , m_ticks(0)
        , m_triggered(false) 
    {
    }

    /** \brief  CTask class destructor
     *
     */
    CTask::~CTask() 
    {
    }

    /** \brief  Run method
     *
     *  It applies the '_run' method, which implements the task's functionality. It has to override in the derived class.  
     *  
     *  
     */
    void CTask::run()
    {
        if (m_triggered)
        {
            m_triggered = false;
            _run();
        }
    }

    /******************************************************************************/
    /** \brief  CTaskManager class constructor
     *
     *  Constructor method
     *
     *  @param f_taskList      list of tasks
     *  @param f_taskCount     number of tasks
     *  @param f_baseFreq      base frequency
     */
    CTaskManager::CTaskManager(utils::task::CTask** f_taskList, uint32_t f_taskCount, float f_baseFreq)
        : m_taskList(f_taskList)
        , m_taskCount(f_taskCount) 
    {
        m_ticker.attach(mbed::callback(this,&utils::task::CTaskManager::timerCallback), f_baseFreq);
    }

    /** \brief  CTaskManager class destructor
     *  
     */
    CTaskManager::~CTaskManager() 
    {
        m_ticker.detach();
    }

}; // namespace utils::task