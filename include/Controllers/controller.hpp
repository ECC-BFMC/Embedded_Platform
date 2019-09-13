/**
  ******************************************************************************
  * @file    Controller.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the controller
  *          functionality.
  ******************************************************************************
 */

/* Include guard */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include<cmath>
#include<Controllers/pidcontroller.hpp>

#include <Encoders/encoderinterface.hpp>
#include <Controllers/converters.hpp>

#include <mbed.h>

namespace controllers
{

   /** CControllerSiso class
    * @brief It implements a controller with a single input and a single output. It needs an encoder getter interface to get the measured values, a controller to calculate the control signal. It can be completed with a converter to convert the measaurment unit of the control signal. 
    * 
    */
    class CControllerSiso
    {
        /* PID controller declaration*/
        template <class T>
        using  ControllerType=siso::IController<T>;

        public:
            /* Construnctor */
            CControllerSiso(encoders::IEncoderGetter&               f_encoder
                            ,ControllerType<double>&                f_pid
                            ,controllers::IConverter*                   f_converter=NULL);
            /* Set controller reference value */
            void setRef(double                       f_RefRps);
            /* Get controller reference value */
            double getRef();
            /* Get control value */
            double get();
            /* Get error */
            double getError();
            /* Clear PID parameters */
            void clear();
            /* Control action */
            int8_t control();

        private:
            /* PWM onverter */
            double converter(double f_u);

            /* Enconder object reference */
            encoders::IEncoderGetter&               m_encoder;
            /* PID object reference */
            ControllerType<double>&                 m_pid;
            /* Controller reference */
            double                                  m_RefRps;
            /* Control value */
            double                                  m_u;
            /* Error */
            double                                  m_error;
            /* Converter */
            controllers::IConverter*                m_converter;
            uint8_t                                 m_nrHighPwm;
            /* Maximum High PWM Signal */
            const uint8_t                           m_maxNrHighPwm;


            /* Scaled PWM control signal limits */
            const float                                     m_control_sup;
            const float                                     m_control_inf;
            /* Inferior limit of absolut reference */
            const float                                     m_ref_abs_inf;
            /* Inferior and superior limits of absolute speed measurment for standing state */
            const float                                     m_mes_abs_inf;
            const float                                     m_mes_abs_sup;

    };
}; // namespace controllers

#endif