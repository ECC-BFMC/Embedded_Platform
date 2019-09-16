/**
  ******************************************************************************
  * @file    SensorTask.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the sensor task
  *          functionality.
  ******************************************************************************
 */
#include <Examples/Sensors/sensortask.hpp>

namespace examples
{

    namespace sensors{
        

        /** \brief  Constructor for the CCounter class
         *
         *  Constructor method
         *
         *  @param f_period       period value
         *  @param f_serial       reference to the serial object
         */
        CDISTFTest::CDISTFTest(uint32_t        f_period
                                        ,Serial&         f_serial)
            :task::CTask(f_period)
            ,m_serial(f_serial)
            ,m_tf()
        {
            std::array<std::array<float,1>,3> l_num({std::array<float,1>({3}),std::array<float,1>({-5.897}),std::array<float,1>({2.9})});
            std::array<std::array<float,1>,3> l_den({std::array<float,1>({1}),std::array<float,1>({-1.949}),std::array<float,1>({0.9512})});
            m_tf.setNum(l_num);
            m_tf.setDen(l_den);
        }

        /** \brief  Method called each f_period
         * 
         *  
         *  
         */
        void CDISTFTest::_run()
        {
            float l_input=1.0;
            float l_output=m_tf(l_input);
            m_serial.printf("%.4f\n",l_output);
        }

        /** \brief  Constructor for the CCounter class
         *
         *  Constructor method
         *
         *  @param f_period       period value
         *  @param f_encoder      reference to encoder object
         *  @param f_serial       reference to the serial object
         */
        CEncoderSender::CEncoderSender(uint32_t            f_period
                                                ,encoders::IEncoderGetter&     f_encoder
                                                ,Serial&             f_serial)
            :task::CTask(f_period)
            ,m_isActived(false)
            ,m_encoder(f_encoder)
            ,m_serial(f_serial)
        {
        }

        /** \brief  Serial callback method
         *
         * Serial callback attaching serial callback to controller object
         *
         * @param  obj                 PID controller object
         * @param  a                   string to read data from
         * @param b                   string to write data to
         * 
         */
        void CEncoderSender::staticSerialCallback(void* obj,char const * a, char * b){
            examples::sensors::CEncoderSender* self = static_cast<CEncoderSender*>(obj);
            self->serialCallback(a,b);
        }

        /** \brief  Serial callback actions
         *
         * Serial callback method setting controller to values received
         *
         * @param  a                   string to read data from
         * @param b                   string to write data to
         * 
         */
        void CEncoderSender::serialCallback(char const * a, char * b){
            int l_isActivate=0;
            uint32_t l_res = sscanf(a,"%d",&l_isActivate);
            if(l_res==1){
                m_isActived=(l_isActivate>=1);
                sprintf(b,"ack;;");
            }else{
                sprintf(b,"sintax error;;");
            }
        }

        /** \brief  Method called each f_period
         * 
         *  
         *  
         */
        void CEncoderSender::_run()
        {
            if(!m_isActived) return;
            float l_filtRps=m_encoder.getSpeedRps();
            m_serial.printf("@ENPB:%.2f;;\r\n",l_filtRps);  
        }                        

    }; // namespace sensors
}; // namespace examples
