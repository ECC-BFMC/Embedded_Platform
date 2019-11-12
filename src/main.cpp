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
 * @file main.cpp
 * @author RBRO/PJ-IU
 * @brief Application entry point.
 * @version 0.1
 * @date 2019-11-07
 */
/* The mbed library */
#include <mbed.h>
/* Task manager */
#include <utils/taskmanager/taskmanager.hpp>
/* Header file for the blinker functionality */
#include <examples/blinker.hpp>
/* Header file for the serial communication functionality */
#include <utils/serial/serialmonitor.hpp>
/* Header file for the motion controller functionality */
#include <brain/robotstatemachine.hpp>
/* Header file for the sensor task functionality */
#include <examples/sensors/encoderpublisher.hpp>
/* Header file  for the controller functionality */
#include <signal/controllers/motorcontroller.hpp>
/* Quadrature encoder functionality */
#include <hardware/encoders/quadratureencoder.hpp>


/// Serial interface with the another device(like single board computer). It's an built-in class of mbed based on the UART comunication, the inputs have to be transmiter and receiver pins. 
Serial          g_rpi(USBTX, USBRX);
/** @brief 
 * This object is used to control the direction and the rotation speed of the wheel. The fist input respresents the pin for the servo motor, it must to generate a PWM signal. 
 * The second input  is the pin for generating PWM signal for the DC-Motor driver. The third and fourth inputs give the direction of the DC Motor, they are digital pins. The last input parameter represent an analog input pin, to measure the electric current.
 */
hardware::drivers::CMotorDriverVnh g_motorVnhDriver(D3, D2, D4, A0);
hardware::drivers::CSteeringMotor g_steeringDriver(D9);

/// Base sample time for the task manager. The measurement unit of base sample time is second.
const float     g_baseTick = 0.0001; // seconds
/// It's a task for blinking periodically the built-in led on the Nucleo board.
examples::CBlinker        g_blinker       (0.5    / g_baseTick, LED1);

/// The sample time of the encoder, is measured in second. 
float           g_period_Encoder = 0.001;

/// Create a filter object for filtrating the noise appeared on the rotary encoder.
signal::filter::lti::siso::CIIRFilter<float,1,2> g_encoderFilter(utils::linalg::CRowVector<float,1>({ -0.77777778})
                                                        ,utils::linalg::CRowVector<float,2>({0.11111111,0.11111111}));
/// Create a quadrature encoder object with a filter. It periodically measueres the rotary speed of the motor and applies the given filter. 
hardware::encoders::CQuadratureEncoderWithFilter g_quadratureEncoderTask(g_period_Encoder,hardware::drivers::CQuadratureCounter_TIM4::Instance(),2048,g_encoderFilter);

///Create an encoder publisher object to transmite the rotary speed of the dc motor. 
examples::sensors::CEncoderPublisher   g_encoderPublisher(0.01/g_baseTick,g_quadratureEncoderTask,g_rpi);

//Create an object to convert volt to pwm for motor driver
/// Create a splines based converter object to convert the volt signal to pwm signal
signal::controllers::CConverterSpline<2,1> l_volt2pwmConverter({-0.22166,0.22166},{std::array<float,2>({0.1041568079746662,-0.08952760561569219}),std::array<float,2>({0.50805,0.0}),std::array<float,2>({0.1041568079746662,0.08952760561569219})});
//  signal::controllers::siso::CMotorController<double> l_pidController(g_motorPIDTF,g_period_Encoder);
signal::controllers::siso::CPidController<double> l_pidController( 0.1150,0.81000,0.000222,0.04,g_period_Encoder);
/// Create a controller object based on the predefined PID controller and the quadrature encoder
signal::controllers::CMotorController g_controller(g_quadratureEncoderTask,l_pidController,&l_volt2pwmConverter);
/// Create the motion controller, which controls the robot states and the robot moves based on the transmitted command over the serial interface. 
brain::CRobotStateMachine           g_robotstatemachine(g_period_Encoder, g_rpi, g_motorVnhDriver,g_steeringDriver,&g_controller);

/// Map for redirecting messages with the key and the callback functions. If the message key equals to one of the enumerated keys, than it will be applied the paired callback function.
utils::serial::CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"MCTL",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackMove)},
    {"BRAK",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackBrake)},
    {"PIDA",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackPID)},
    {"ENPB",mbed::callback(&g_encoderPublisher,&examples::sensors::CEncoderPublisher::serialCallback)},
};

/// Create the serial monitor object, which decodes, redirects the messages and transmites the responses.
utils::serial::CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);

//! [Adding a resource]
/// List of the task, each task will be applied their own periodicity, defined by initializing the objects.
utils::task::CTask* g_taskList[] = {
    &g_blinker,
    &g_serialMonitor,
    &g_encoderPublisher
}; 
//! [Adding a resource]

/// Create the task manager, which applies periodically the tasks. It needs the list of task and the time base in seconds. 
utils::task::CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(utils::task::CTask*), g_baseTick);

/**
 * @brief Setup function for initializing some objects and transmiting a startup message through the serial. 
 * 
 * @return uint32_t Error level codes error's type.
 */
uint32_t setup()
{
    g_rpi.baud(256000);  
    g_rpi.printf("\r\n\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#   I'm alive   #\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("\r\n");
    /// Start the Rtos timer for the quadrature encoder    
    g_quadratureEncoderTask.startTimer();
    /// Start the Rtos timer for the motion controller
    g_robotstatemachine.startRtosTimer();
    return 0;    
}

/**
 * @brief Loop function has aim to apply repeatedly task
 * 
 * @return uint32_t Error level codes error's type.
 */
uint32_t loop()
{
    g_taskManager.mainCallback();
    return 0;
}

/**
 * @brief Main function applies the setup function and the loop function periodically. It runs automatically after the board started.
 * 
 * @return int Error level codes error's type.  
 */
int main() 
{
    uint32_t  l_errorLevel = setup(); 
    while(!l_errorLevel) 
    {
        l_errorLevel = loop();
    }
    g_rpi.printf("exiting with code: %d",l_errorLevel);
    return l_errorLevel;
}