/**
  ******************************************************************************
  * @file    SerialMonitor.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the serial communication
  *          functionality.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef SERIAL_MONITOR_HPP
#define SERIAL_MONITOR_HPP

/* The mbed library */
#include <mbed.h>
#include <map>
#include <array>
/* Function objects */
#include <functional>
#include<TaskManager/taskmanager.hpp>
#include <Queue/queue.hpp>


namespace serial{

   /**
    * @brief It is used for implementing serial communciation.
    * 
    */
    class CSerialMonitor : public task::CTask
    {
    public:
        typedef mbed::Callback<void(char const *, char *)> FCallback;
        typedef std::map<string,FCallback> CSerialSubscriberMap;

        /* Constructor */
        CSerialMonitor(Serial& f_serialPort
                    ,CSerialSubscriberMap f_serialSubscriberMap);
    private:
        /* Rx callback */
        static void RxCallback(void *thisPointer);
        /* Tx callback */
        static void TxCallback(void *thisPointer);
        /* Rx callback actions */
        void serialRxCallback();
        /* Tx callback actions */
        void serialTxCallback();
        /* Run method */
        virtual void _run();

        /* Reference to serial object */
        Serial& m_serialPort;
        /* Rx buffer */
        CQueue<char,255> m_RxBuffer;
        /* Tx buffer */
        CQueue<char,255> m_TxBuffer;
        /* Data buffer */
        array<char,256> m_parseBuffer;
        /* Parse iterator */
        array<char,256>::iterator m_parseIt;
        /* Serial subscriber */
        CSerialSubscriberMap m_serialSubscriberMap;
    };

}; // namespace serial

#endif // SERIAL_MONITOR_HPP
