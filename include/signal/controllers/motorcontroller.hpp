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
#include<signal/controllers/sisocontrollers.hpp>

#include <hardware/encoders/encoderinterfaces.hpp>
#include <signal/controllers/converters.hpp>

#include <mbed.h>

namespace signal
{
namespace controllers
{

   /**
    * @brief It implements a controller with a single input and a single output. It needs an encoder getter interface to get the measured values, a controller to calculate the control signal. It can be completed with a converter to convert the measaurment unit of the control signal. 
    * 
    */
    class CMotorController
    {
        /* PID controller declaration*/
        template <class T>
        using  ControllerType=siso::IController<T>;

        public:
            /* Construnctor */
            CMotorController(hardware::encoders::IEncoderGetter&               f_encoder
                            ,ControllerType<double>&                f_pid
                            ,signal::controllers::IConverter*               f_converter=NULL
                            ,float                                  f_inf_ref = -225
                            ,float                                  f_sup_ref = 225);
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

            bool inRange(double f_RefRps);

        private:
            /* PWM onverter */
            double converter(double f_u);

            /* Enconder object reference */
            hardware::encoders::IEncoderGetter&               m_encoder;
            /* PID object reference */
            ControllerType<double>&                 m_pid;
            /* Controller reference */
            double                                  m_RefRps;
            /* Control value */
            double                                  m_u;
            /* Error */
            double                                  m_error;
            /* Converter */
            signal::controllers::IConverter*                m_converter;
            uint8_t                                 m_nrHighPwm;
            /* Maximum High PWM Signal */
            const uint8_t                           m_maxNrHighPwm;


            /* Scaled PWM control signal limits */
            const float                                     m_control_sup;
            const float                                     m_control_inf;
            /* Absolute inferior limit of  reference to inactivate the controller in the case low reference and observation value. */
            const float                                     m_ref_abs_inf;
            /* Absolute inferior limits of measured speed to inactivate the controller in the case low reference and observation value. */
            const float                                     m_mes_abs_inf;
            /* Superior limits of measured speed to deactivate the controller for avoid over accelerating. */
            const float                                     m_mes_abs_sup;
            /* Reference allowed limits */
            const float                                     m_inf_ref;
            const float                                     m_sup_ref;

    };
}; // namespace controllers

}; // namespace signal

#endif