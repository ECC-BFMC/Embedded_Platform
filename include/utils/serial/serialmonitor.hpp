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
#include<utils/taskmanager/taskmanager.hpp>
#include <utils/queue/queue.hpp>


namespace utils::serial{

   /**
    * @brief Class Serial Monitor
    * 
    * It has aim to decode the messages received from the other device and redirectionate to other functions the content of message. 
    * For decode it has a predefined structure with a header (key) part and a content part. The key has to be four character, the content is defined by user.
    * The message received has to start with '#' special character, the responses have the same key and start with "@" character. The special characters notice the direction of the message.
    * Examples of messages:
    *   '#KEY1:MESSAGECONTENT;;\r\n'
    *   '@KEY1:RESPONSECONTANT;;\r\n'
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

        /* Reference to serial object */
        Serial& m_serialPort;
        /* Rx buffer */
        utils::CQueue<char,255> m_RxBuffer;
        /* Tx buffer */
        utils::CQueue<char,255> m_TxBuffer;
        /* Data buffer */
        array<char,256> m_parseBuffer;
        /* Parse iterator */
        array<char,256>::iterator m_parseIt;
        /* Serial subscriber */
        CSerialSubscriberMap m_serialSubscriberMap;
    };

}; // namespace utils::serial

#endif // SERIAL_MONITOR_HPP
