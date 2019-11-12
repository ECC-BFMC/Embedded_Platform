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
  * @file    MotionController.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the motion controller
  *          functionality.
  ******************************************************************************
 */
#include <brain/robotstatemachine.hpp>

namespace brain{

    /**
     * @brief CRobotStateMachine Class constructor
     * 
     * @param f_period_sec          period for controller execution in seconds
     * @param f_serialPort          reference to serial communication object
     * @param f_motorControl        reference to dc motor control interface
     * @param f_steeringControl     reference to steering motor control interface
     * @param f_control             reference to controller object
     */
    CRobotStateMachine::CRobotStateMachine(
            float f_period_sec,
            Serial& f_serialPort,
            hardware::drivers::IMotorCommand&                 f_motorControl,
            hardware::drivers::ISteeringCommand&              f_steeringControl,
            signal::controllers::CMotorController*           f_control) 
        : m_serialPort(f_serialPort)
        , m_motorControl(f_motorControl)
        , m_steeringControl(f_steeringControl)
        , m_speed()
        , m_angle()
        , m_period_sec(f_period_sec)
        , m_ispidActivated(false)
        , m_hbTimeOut()
        , m_control(f_control)
        , m_timer(mbed::callback(this,&CRobotStateMachine::_run))
    {
    }

    /** \brief  Set the speed and the angle to zero value
     * 
     */
    void CRobotStateMachine::reset()
    {   
        m_speed = 0;
        m_angle = 0;
    }

    /** \brief  Get last speed command
     * 
     *  \return speed command value. (duty cycle of pwm or speed of robot in mps)
     */
    float CRobotStateMachine::getSpeed() 
    {
        return m_speed;
    }

    /** \brief  Get last steering angle command.
     * 
     *  
     *  \return steering angle in degree
     */
    float CRobotStateMachine::getAngle() 
    {
        return m_angle;
    }

    /** \brief  BrakeCallback method
     * 
     *  It changes the controller state to brake from default state. It's applied to deactivated hard braking.
     *  
     */
    void CRobotStateMachine::BrakeCallback(){
        if( m_state == 0){
            m_state=2;
        }
    }

    /** \brief  Set state of the controller
     * 
     *  @param  f_state state of controller express in integer value between 0 and 2
     *  
     */
    void CRobotStateMachine::setState(int f_state){
        m_state = f_state;
    }

    /** \brief  _Run method contains the main application logic, where it controls the lower lever drivers (dc motor and steering) based the given command and state.
     * It has three state: 
     *  - 0 - default state -> it doesn't apply any control signal on the drivers.
     *  - 1 - move state -> control the motor rotation speed by giving direct a PWM signal or by a pid controller
     *                   -> and control the steering angle
     *  - 2 - brake state -> apply a dynamic braking on the motor and control the steering angle.          
     */
    void CRobotStateMachine::_run()
    {   
        switch(m_state)
        {
            // Move state - control the dc motor rotation speed and the steering angle. 
            case 1:
                m_steeringControl.setAngle(m_angle); // control the steering angle 
                if(m_ispidActivated && m_control!=NULL) // Check the pid controller 
                {
                    m_control->setRef(CRobotStateMachine::Mps2Rps( m_speed )); // Set the reference of dc motor speed
                    // Calculate control signal and return the controller state. 
                    int8_t l_isCorrect = m_control->control(); 
                    // Check the state of the control method 
                    if( l_isCorrect == -1 ) // High consecutive control signal 
                    {
                        // In this case the encoder is working fine and measures too high speed rotation, than it changes to the braking state.  
                        m_serialPort.printf("@PIDA:Too high speed and the encoder working;;\r\n");
                        m_motorControl.brake();
                        m_control->clear();
                        m_state = 2;
                    }
                    else if (l_isCorrect == -2 ) // High consecutive control signal without observation value. 
                    {
                        // In this case the encoder fails and measures 0 rps, but the control signal had a series high values. 
                        // This part protects the robot to run with high speed, when the encoder doesn't measure correctly or it's broker.
                        m_serialPort.printf("@PIDA:Encoder error;;\r\n");
                        m_motorControl.brake();
                        m_control->clear();
                        m_state = 2;
                    }
                    else // It's all right and can control the robot. 
                    {
                        m_motorControl.setSpeed(m_control->get());
                    }
                    
                }
                else // The pid controller is deactivated and the dc motor is controlled by user control signal by giving duty cycle of PWM. 
                {
                    m_motorControl.setSpeed(m_speed);
                }
                break;

            // Brake state
            case 2:
                m_steeringControl.setAngle(m_angle); // Setting the steering angle
                m_motorControl.brake(); // dc motor dynamic braking. 
                if( m_control!=NULL){ 
                    m_control->clear();
                }
                break;
        }
        
    }

    /** \brief  Serial callback method for move command
     *
     * Serial callback method setting controller to values received like steering angle and dc motor control values. 
     * In the case of pid activated,  the dc motor control values has to be express in meter per second, otherwise represent the duty cycle of PWM signal in percent. 
     * The steering angle has to express in degree, where the positive values marks the right direction and the negative values noticed the left turning direction.
     *
     * @param a                   string to read data 
     * @param b                   string to write data 
     * 
     */
    void CRobotStateMachine::serialCallbackMove(char const * a, char * b)
    {
        float l_speed;
        float l_angle;
        uint32_t l_res = sscanf(a,"%f;%f",&l_speed,&l_angle);
        if (2 == l_res)
        {
            if( !m_ispidActivated && !m_motorControl.inRange(l_speed)){ // Check the received control value
                sprintf(b,"The speed command is too high;;");
                return;
            }
            if( m_ispidActivated && !m_control->inRange(CRobotStateMachine::Mps2Rps(l_speed))){ //Check the received reference value
                sprintf(b,"The speed reference is too high;;");
                return;
            }
            if( !m_steeringControl.inRange(l_angle)){ // Check the received steering angle
                sprintf(b,"The steering angle command is too high;;");
                return;
            }

            m_speed = l_speed;
            m_angle = l_angle; 
            m_state=1;
            sprintf(b,"ack;;");
        }
        else
        {
            sprintf(b,"sintax error;;");
        }
    }

    /** \brief  Serial callback actions for brake command
     *
     * This method aims to change the state of controller to brake and sets the steering angle to the received value. 
     *
     * @param a                   string to read data 
     * @param b                   string to write data
     * 
     */
    void CRobotStateMachine::serialCallbackBrake(char const * a, char * b)
    {
        float l_angle;
        uint32_t l_res = sscanf(a,"%f",&l_angle);
        if(1 == l_res)
        {
            if( !m_steeringControl.inRange(l_angle)){
                sprintf(b,"The steering angle command is too high;;");
                return;
            }
            m_speed = 0;
            m_angle = l_angle;
            // Brake state 
            m_state = 2;

            if( m_control!=NULL){
                m_control->setRef(0);
            }
            sprintf(b,"ack;;");           
        }
        else
        {
            sprintf(b,"sintax error;;");
        }
    }

    /** \brief  Serial callback actions for hard brake command
     *
     * It can be used to activate a inverse current braking mechanism, the inverse current applies a short period of time. 
     * The period is defined through attaching the "BrakeCallback" method. 
     *
     * @param a                   string to read data 
     * @param b                   string to write data
     * 
     */
    void CRobotStateMachine::serialCallbackHardBrake(char const * a, char * b)
    {
        float l_brake,l_angle;
        uint32_t l_res = sscanf(a,"%f;%f",&l_brake,&l_angle);
        if(2 == l_res && m_state!=0)
        {
            if( !m_steeringControl.inRange(l_angle)){
                sprintf(b,"The steering angle command is too high;;");
                return;
            }
            m_speed=0;
            m_angle = l_angle; 
            m_motorControl.inverseDirection(l_brake);
            m_hbTimeOut.attach(callback(this,&CRobotStateMachine::BrakeCallback),0.04); // Attaching a callback function for changing state to brake. 
            m_state = 0;

            sprintf(b,"ack;;");           
        }
        else
        {
            sprintf(b,"sintax error;;");
        }
    }

    /** \brief  Serial callback actions for hard PID activation command
     *
     * This function provides an interface to activate or deactivate the Pid controller. When the input string contains non-zero value, then it activates the pid functionality and the robot's linear velocity will be controlled in meter per second.
     * When the value is zero, the user directly transmite the duty cycle of pwm signal to control the motor rotation speed. If the controller wasn't deffined for the motioncontroller object, this functionality cannot be activated. 
     *
     * @param a                   string to read data 
     * @param b                   string to write data
     * 
     */
    void CRobotStateMachine::serialCallbackPID(char const * a, char * b)
    {
        int l_isActivate=0;
        uint32_t l_res = sscanf(a,"%d",&l_isActivate);
        if(l_res==1)
        {   
            if(m_control==NULL){
                sprintf(b,"Control object wans't instances. Cannot be activate pid controller;;");
            }else{
                m_speed = 0;
                m_ispidActivated=(l_isActivate>=1);
                // Change to brake state
                m_state = 2;
                sprintf(b,"ack;;");    
            }
            
        }else
        {
            sprintf(b,"sintax error;;");
        }
    }

    /**
     * @brief Function to convert from linear velocity ( meter per second ) of robot to angular velocity ( rotation per second ) of motor.
     * 
     * @param f_vel_mps linear velocity of robot
     * @return float angular velocity of motor
     */
    float CRobotStateMachine::Mps2Rps(float f_vel_mps){
        return f_vel_mps * 150.0;
    }

    /**
     * @brief Start RtosTimer, which periodically apply the run method. The period of the task is defined in the contructor.
     * 
     */
    void CRobotStateMachine::startRtosTimer(){
        this->m_timer.start(static_cast<int>(m_period_sec*1000));
    }

}; // namespace brain