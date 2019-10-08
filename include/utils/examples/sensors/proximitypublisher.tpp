
#ifndef PROXIMITY_PUBLISHER_TPP
#define PROXIMITY_PUBLISHER_TPP

#ifndef PROXIMITY_PUBLISHER_HPP
#error __FILE__ should only be included from proximitypublisher.hpp
#endif //PROXIMITY_PUBLISHER_HPP

namespace examples{
    namespace sensors{
        /** \brief  Constructor for the CEncoderFilter class
         *
         *  It initializes the member parameters. The publisher initially is deactivated. 
         *
         *  @param f_period       period for sending the measured values
         *  @param f_sensors      array of proximity sensors  
         *  @param f_serial       the serial object
         */
        template <class C_Sensor,uint Nr_Senrsor> 
        ProximityPublisher<C_Sensor,Nr_Senrsor>::ProximityPublisher(uint32_t        f_period
                                                                ,SensorArrayT    f_sensors
                                                                ,Serial&         f_serial)
            :CTask(f_period)
            ,m_sensors(f_sensors)
            ,m_serial(f_serial)
            ,m_isActivate(false)
        {
        }

        /** \brief  Serial callback method to activate or deactivate the publisher.
         * 
         * If the message contains a a number higher or equal to 1 then it activates the publisher, 
         * otherwise it deactivated the publisher and doesn't send the message. 
         *
         * @param a                   input received string 
         * @param b                   output  string 
         * 
         */
        template <class C_Sensor,uint Nr_Senrsor> 
        void ProximityPublisher<C_Sensor,Nr_Senrsor>::serialCallback(char const * a, char * b){
            int l_isActivate=0;
            uint32_t l_res = sscanf(a,"%d",&l_isActivate);
            if(l_res==1){
                m_isActivate=(l_isActivate>=1);
                sprintf(b,"ack;;");
            }else{
                sprintf(b,"sintax error;;");
            }
        }

        /** \brief  Periodically applied sending method.
         * 
         *  It the case of deactivated publisher, it does nothing. 
         *  
         */
        template <class C_Sensor,uint Nr_Senrsor> 
        void ProximityPublisher<C_Sensor,Nr_Senrsor>::_run(){
            if(!m_isActivate) return;
            char l_buf[100];
            sprintf(l_buf,"@DSPB:");
            for(uint8_t i=0;i<Nr_Senrsor;++i){
                
                sprintf(l_buf,"%s%2.2f;",l_buf,m_sensors[i]->getValue());
            }
            sprintf(l_buf,"%s;\n\r",l_buf);
            m_serial.printf("%s",l_buf);

        }
    }; // namespace sensors
}; // namespace examples

#endif // PROXIMITY_PUBLISHER