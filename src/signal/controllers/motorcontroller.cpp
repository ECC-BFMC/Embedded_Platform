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
  * @file    Controller.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the controller
  *          functionality.
  ******************************************************************************
 */

#include <signal/controllers/motorcontroller.hpp>

namespace signal{
    

namespace controllers{
    /**
     * @brief Construct a new CMotorController::CMotorController object
     * 
     * @param f_encoder Reference to the encoder getter interface.
     * @param f_pid     Reference to the controller interface.
     * @param f_converter [Optional] Pointer to the converter interface. 
     * @param f_inf_ref   [Optional] Inferior limit of reference signal.
     * @param f_sup_ref   [Optional] Superior limit of reference signal.
     */
    CMotorController::CMotorController(hardware::encoders::IEncoderGetter&          f_encoder
                            ,ControllerType<double>&                    f_pid
                            ,signal::controllers::IConverter*                   f_converter
                            ,float                                      f_inf_ref
                            ,float                                      f_sup_ref)
        :m_encoder(f_encoder)
        ,m_pid(f_pid)
        ,m_converter(f_converter)
        ,m_nrHighPwm(0)
        ,m_maxNrHighPwm(10)
        ,m_control_sup(0.5)
        ,m_control_inf(-0.5)
        ,m_ref_abs_inf(10.0)
        ,m_mes_abs_inf(30.0)
        ,m_mes_abs_sup(300.0)
        ,m_inf_ref(f_inf_ref)
        ,m_sup_ref(f_sup_ref)
    {
    }

    /**
     * @brief Set the reference signal value.
     * 
     * @param f_RefRps The value of the reference signal
     */
    void CMotorController::setRef(double f_RefRps)
    {
        m_RefRps=f_RefRps;
    }

    /** \brief  Get the value of reference signal.
     *
     */
    double CMotorController::getRef()
    {
        return m_RefRps;
    }

    /** @brief  Get the value of the control signal calculated last time.
     * 
     */
    double CMotorController::get()
    {
        return m_u;
    }

    /** @brief  Get the value of the error between the measured and reference signal. 
     *
     */
    double CMotorController::getError()
    {
        return m_error;
    }

    /** @brief  Clear the memory of the controller. 
     *
     */
    void CMotorController::clear()
    {
        m_pid.clear();
    }


    /**
     * @brief It calculates the next value of the control signal, by utilizing the given interfaces.
     * 
     * @return true control works fine
     * @return false appeared an error
     */
    int8_t CMotorController::control()
    {
        // Mesurment speed value
        float  l_MesRps = m_encoder.getSpeedRps();
        bool   l_isAbs = m_encoder.isAbs();
        float  l_ref;

        // Check the measured value and the superior limit for avoid over control state.
        // In this case deactivate the controller. 
        if(std::abs(l_MesRps) > m_mes_abs_sup){
            m_RefRps = 0.0;
            m_u = 0.0;
            return -1;
        }
        // Check the inferior limits of reference signal and measured signal for standing state.
        // Inactivate the controller to not brake the robot, when it stopped. 
        if(std::abs(m_RefRps) < m_ref_abs_inf && std::abs(l_MesRps) < m_mes_abs_inf ){
            m_u = 0.0;
            m_error = 0.0;
            return 1; 
        }

        // Check measured value is orientated or absolute
        if ( l_isAbs ){
            l_ref = std::abs(m_RefRps);
        } else{ 
            l_ref = m_RefRps;
        }
        float l_error=l_ref-l_MesRps;
        float l_v_control = m_pid.calculateControl(l_error);
        float l_pwm_control = converter(l_v_control);
        

        // Verify the number of high control signal and the measued rotation speed. When it's true, than the encoder doesn't measure the correct rotation speed,
        // so the calculated control signal has a too high value. 
        if(m_nrHighPwm>m_maxNrHighPwm && l_MesRps==0){
            m_pid.clear();
            m_RefRps = 0.0;
            m_u = 0.0;
            m_nrHighPwm = 0;
            return -2;
        }

        m_u=l_pwm_control;
        m_error=l_error;
        
        // Check measured value is oriantated or absolute.
        if(m_RefRps<0 && l_isAbs)
        {
            m_u=m_u*-1.0;
        }
        return 1;
    }

    /** @brief  
     *
     * Apply the converter interface to change the measurment unit.
     *
     * @param f_u                  Input control signal
     * @return                     Converted control signal
     */
    double CMotorController::converter(double f_u)
    {
        double l_pwm=f_u;
        // Convert the control signal from V to PWM
        if(m_converter!=NULL){
            l_pwm = (*m_converter)(f_u);
        }

        // Check the pwm control signal and limits
        if( l_pwm < m_control_inf ){
            l_pwm  = m_control_inf;
            ++m_nrHighPwm;
        } else if( l_pwm > m_control_sup ){
            l_pwm  = m_control_sup;
            ++m_nrHighPwm;
        }
        else{
            m_nrHighPwm = 0;
        }

        return l_pwm;
    }

    /**
     * @brief It verifies whether a number is in a given range
     * 
     * @param f_RefRps reference value for controller in rotation per second  
     * @return true means, that the value is in the range
     * @return false means, that the value isn't in the range
     */
    bool CMotorController::inRange(double f_RefRps){
        return m_inf_ref<=f_RefRps && f_RefRps<=m_sup_ref;
    }

}; //  namespace controllers
};// namespace signal