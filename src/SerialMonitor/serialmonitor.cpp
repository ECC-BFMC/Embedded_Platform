/**
  ******************************************************************************
  * @file    SerialMonitor.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the serial communication
  *          functionality.
  ******************************************************************************
 */

#include <SerialMonitor/serialmonitor.hpp>


namespace serial{

    /** @brief  CSerialMonitor class constructor
     *
     *
     *  @param f_serialPort               reference to serial object
     *  @param f_serialSubscriberMap      sensor mounteed on left front part
     */
    CSerialMonitor::CSerialMonitor(Serial& f_serialPort
                    ,CSerialSubscriberMap f_serialSubscriberMap)
            :task::CTask(0)
            , m_serialPort(f_serialPort)
            , m_RxBuffer()
            , m_TxBuffer()
            , m_parseBuffer()
            , m_parseIt(m_parseBuffer.begin())
            , m_serialSubscriberMap(f_serialSubscriberMap) 
            {
                m_serialPort.attach(mbed::callback(&CSerialMonitor::RxCallback, this), Serial::RxIrq);
                m_serialPort.attach(mbed::callback(&CSerialMonitor::TxCallback, this), Serial::TxIrq);
            }

    /** @brief  Rx callback
     *
     *  @param thisPointer         the object pointer
     *  
     */
    void CSerialMonitor::RxCallback(void *thisPointer)
    {
        CSerialMonitor* self = static_cast<CSerialMonitor*>(thisPointer);
        self->serialRxCallback(); 
    }

    /** @brief  Tx callback
     *
     *  @param thisPointer         the object pointer
     *  
     */
    void CSerialMonitor::TxCallback(void *thisPointer)
    {
        CSerialMonitor* self = static_cast<CSerialMonitor*>(thisPointer);
        self->serialTxCallback(); 
    }

    /** @brief  Rx callback actions
     *
     *  @param None
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
     *  @param None
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

    /** @brief  Run method
     * 
     *  @param  None
     *  @param None
     */
    void CSerialMonitor::_run()
    {
        if ((!m_RxBuffer.isEmpty()))
        {
            char l_c = m_RxBuffer.pop();
            // m_serialPort.printf("%c",l_c);
            if ('#' == l_c)
            {
                m_parseIt = m_parseBuffer.begin();
                m_parseIt[0] = l_c;
                m_parseIt++;
                return;
            }
            if (m_parseIt != m_parseBuffer.end())
            {
                if (l_c == '\n')
                {
                    if ((';' == m_parseIt[-3]) && (';' == m_parseIt[-2]) && ('\r' == m_parseIt[-1]))
                    {
                        char l_msgID[5];
                        char l_msg[256];

                        uint32_t res = sscanf(m_parseBuffer.data(),"#%4s:%s;;",l_msgID,l_msg);
                        if (res == 2)
                        {
                            auto l_pair = m_serialSubscriberMap.find(l_msgID);
                            if (l_pair != m_serialSubscriberMap.end())
                            {
                                char l_resp[256] = "no response given";
                                string s(l_resp);
                                l_pair->second(l_msg,l_resp);
                                m_serialPort.printf("@%s:%s\r\n",l_msgID,l_resp);
                            }
                        }
                        m_parseIt = m_parseBuffer.begin();
                    }
                }
                m_parseIt[0] = l_c;
                m_parseIt++;
                return;
            }
        }
    }

}; // namespace serial