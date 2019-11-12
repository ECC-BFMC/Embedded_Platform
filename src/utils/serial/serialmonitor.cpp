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
  * @file    SerialMonitor.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the serial communication
  *          functionality.
  ******************************************************************************
 */

#include <utils/serial/serialmonitor.hpp>


namespace utils::serial{

    /** @brief  CSerialMonitor class constructor
     *
     *
     *  @param f_serialPort               reference to serial object
     *  @param f_serialSubscriberMap      map with the key and the callback functions
     */
    CSerialMonitor::CSerialMonitor(Serial& f_serialPort
                    ,CSerialSubscriberMap f_serialSubscriberMap)
            :utils::task::CTask(0)
            , m_serialPort(f_serialPort)
            , m_RxBuffer()
            , m_TxBuffer()
            , m_parseBuffer()
            , m_parseIt(m_parseBuffer.begin())
            , m_serialSubscriberMap(f_serialSubscriberMap) 
            {
                m_serialPort.attach(mbed::callback(this,&CSerialMonitor::serialRxCallback), Serial::RxIrq); 
                m_serialPort.attach(mbed::callback(this,&CSerialMonitor::serialTxCallback), Serial::TxIrq); 
            }

    /** @brief  Rx callback actions
     *  
     */
    void CSerialMonitor::serialRxCallback()
    {
        __disable_irq();
        while ((m_serialPort.readable()) && (!m_RxBuffer.isFull())) {
            char l_c = m_serialPort.getc();
            m_RxBuffer.push(l_c);
        }
        __enable_irq();
        return;
    }

    /** @brief  Tx callback actions
     *  
     */
    void CSerialMonitor::serialTxCallback()
    {
        __disable_irq();
        while ((m_serialPort.writeable()) && (!m_TxBuffer.isEmpty())) {
            m_serialPort.putc(m_TxBuffer.pop());
        }
        __enable_irq();
        return;
    }

    /** @brief  Monitoring function
     * 
     * It has role to monitor the received messaged, it applies periodically to read the buffer content and to decode it. 
     * Each validted messages are redirectionated to the callback function, by appling these. The callback function requires two input as pointers,
     *  one for message's content and one for response's content. After the appling the callback function, it will send the response to the other device.
     */
    void CSerialMonitor::_run()
    {
        if ((!m_RxBuffer.isEmpty()))
        {
            char l_c = m_RxBuffer.pop(); // Read the next character from buffer
            if ('#' == l_c) // Message starting special character
            {
                m_parseIt = m_parseBuffer.begin();
                m_parseIt[0] = l_c;
                m_parseIt++;
                return;
            }
            if (m_parseIt != m_parseBuffer.end())
            {
                if (l_c == '\n') // Message ending character
                {
                    if ((';' == m_parseIt[-3]) && (';' == m_parseIt[-2]) && ('\r' == m_parseIt[-1])) // Check the message ending
                    {
                        char l_msgID[5];
                        char l_msg[256];

                        uint32_t res = sscanf(m_parseBuffer.data(),"#%4s:%s;;",l_msgID,l_msg); //Parse the message to key and content
                        if (res == 2) // Check the parsing
                        {
                            auto l_pair = m_serialSubscriberMap.find(l_msgID); // Search the key and callback function pair
                            if (l_pair != m_serialSubscriberMap.end()) // Check the existence of key 
                            {
                                char l_resp[256] = "no response given"; // Initial response message
                                string s(l_resp);
                                l_pair->second(l_msg,l_resp); // Apply the attached function
                                m_serialPort.printf("@%s:%s\r\n",l_msgID,l_resp); // Create the response message
                            }
                        }
                        m_parseIt = m_parseBuffer.begin(); //Go to begining of parse buffer.
                    }
                }
                m_parseIt[0] = l_c;
                m_parseIt++;
                return;
            }
        }
    }

}; // namespace serial