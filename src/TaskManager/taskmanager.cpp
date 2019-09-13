/**
  ******************************************************************************
  * @file    TaskManager.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for task manager.
  ******************************************************************************
 */

#include <TaskManager/taskmanager.hpp>

namespace task{

    /******************************************************************************/
    /** \brief  CTask class constructor
     *
     *  Constructor method
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
     *  Destructor method
     *
     *  
     */
    CTask::~CTask() 
    {
    }

    /** \brief  Run method
     *
     *  Destructor method
     *
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
     *  @param f_taskList      task list
     *  @param f_taskCount     task count
     *  @param f_baseFreq      base frequency
     */
    CTaskManager::CTaskManager(task::CTask** f_taskList, uint32_t f_taskCount, float f_baseFreq)
        : m_taskList(f_taskList)
        , m_taskCount(f_taskCount) 
    {
        m_ticker.attach(mbed::callback(&task::CTaskManager::callback, this), f_baseFreq);
    }

    /** \brief  CTaskManager class destructor
     *
     *  Destructor method
     *
     *  
     */
    CTaskManager::~CTaskManager() 
    {
        m_ticker.detach();
    }

    /** \brief  Callback method
     *
     *  @param thisPointer       The object pointer
     */
    void CTaskManager::callback(void *thisPointer)
    {
        task::CTaskManager* self = static_cast<task::CTaskManager*>(thisPointer);
        self->timerCallback(); 
    }

}; // namespace task