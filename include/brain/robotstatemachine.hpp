/**
 * Copyright (c) 2019, Bosch Engineering Center Cluj and BFMC organizers
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:

 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

#ifndef MOTION_CONTROLLER_HPP
#define MOTION_CONTROLLER_HPP

/* The mbed library */
#include <mbed.h>
#include <rtos.h>

#include <utils/taskmanager/taskmanager.hpp>
#include <hardware/drivers/dcmotor.hpp>
#include <hardware/drivers/steeringmotor.hpp>

#include <signal/controllers/motorcontroller.hpp>


namespace brain{



    /**
     * @brief CRobotStateMachine targets to implement the main state machine to control
     *  movement of robot and provide the interfaces to control functionality, like braking and moving.
     *  The state of robot can change by external signal received from a higher level controller.   
     * 
     */
    class CRobotStateMachine 
    {
    public:

        CRobotStateMachine(
            float                                               f_period_sec, 
            RawSerial&                                          f_serialPort, 
            hardware::drivers::IMotorCommand&                   f_dcMotor,
            hardware::drivers::ISteeringCommand&                f_steeringControl,
            signal::controllers::CMotorController*              f_dcMotorControl = NULL);
        
        /* Start the Rtos timer for applying "_run" method  */
        void startTimer();
        /* Serial callback method for Speed */ 
        void serialCallbackSPEEDcommand(char const * a, char * b);
        /* Serial callback method for Steering */ 
        void serialCallbackSTEERcommand(char const * a, char * b);
        /* Serial callback method for braking */
        void serialCallbackBRAKEcommand(char const * a, char * b);
        /* Serial callback method for activating pid */
        void serialCallbackACTIVPIDcommand(char const * a, char * b);
        /* Serial callback method for Movement */ 
        void serialCallbackMOVEcommand(char const * a, char * b);

    private:
        /* Contains the state machine, which control the lower level drivers (motor and steering) based the current state. */
        virtual void _run();
        /* Static function to convert from linear velocity ( centimeter per second ) of robot to angular velocity ( rotation per second ) of motor */
        static float Mps2Rps(float f_vel_cmps);
        /* Static function to convert from meters to int */
        static float m2imp(float f_meters);

    private:
        /* reference to Serial object */
        RawSerial&                                      m_serialPort;
        /* Motor control interface */
        hardware::drivers::IMotorCommand&               m_dcMotor;
        /* Steering wheel control interface */
        hardware::drivers::ISteeringCommand&            m_steeringControl;
        /* PEriod i nseconds */
        float                                           m_period_sec;
        /* State machine state */
        uint8_t                                         m_state;
        /* PID activation state */
        bool                                            m_ispidActivated;       
        /* Speed Control for dc motor */
        signal::controllers::CMotorController*          m_dcMotorControl;
        /* Rtos  timer for periodically applying */
        LowPowerTicker                                  m_timer;
    }; // class CRobotStateMachine
}; // namespace brain

#endif // MOTION_CONTROLLER_H 
