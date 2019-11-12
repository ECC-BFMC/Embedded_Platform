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
#include<utils/taskmanager/taskmanager.hpp>
#include <utils/queue/queue.hpp>


namespace utils::serial{

   /**
    * @brief Class Serial Monitor
    * 
    * It aims to decode the messages received from the other device and redirectionate to other functions the content of message. 
    * For decode it has a predefined structure with a header (key) part and a content part. The key has to be four character, the content is defined by user.
    * The message received has to start with '#' special character, the responses have the same key and start with "@" character. The special characters notice the direction of the message.
    * Examples of messages:
    * 
    *   "#KEY1:MESSAGECONTENT;;\r\n"
    * 
    *   "@KEY1:RESPONSECONTANT;;\r\n"
    * 
    * The key differs for each functionalities, so for each callback function.
    */
    class CSerialMonitor : public utils::task::CTask
    {
    public:
        typedef mbed::Callback<void(char const *, char *)> FCallback;
        typedef std::map<string,FCallback> CSerialSubscriberMap;

        /* Constructor */
        CSerialMonitor(Serial& f_serialPort
                    ,CSerialSubscriberMap f_serialSubscriberMap);
    private:
        /* Rx callback actions */
        void serialRxCallback();
        /* Tx callback actions */
        void serialTxCallback();
        /* Run method */
        virtual void _run();

        /** @brief Serial communication port */
        Serial& m_serialPort;
        /** @brief Rx buffer */
        utils::CQueue<char,255> m_RxBuffer;
        /** @brief Tx buffer */
        utils::CQueue<char,255> m_TxBuffer;
        /** @brief Data buffer */
        array<char,256> m_parseBuffer;
        /** @brief Parse iterator */
        array<char,256>::iterator m_parseIt;
        /** @brief Serial subscriber */
        CSerialSubscriberMap m_serialSubscriberMap;
    };

}; // namespace utils::serial

#endif // SERIAL_MONITOR_HPP
