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
  * @file    main.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   Application entry point.
  ******************************************************************************
 */
//!  [Include mbed]
/* The mbed library */
#include <mbed.h>
//!  [Include mbed]
//!  [List of included files]
/* Task manager */
#include<utils/taskmanager/taskmanager.hpp>
/* Header file for the MOVE functionality */
#include <move/move.hpp>
/* Header file for the blinker functionality */
#include <examples/blinker.hpp>
/* Header file for the serial communication functionality */
#include <utils/serial/serialmonitor.hpp>
/* Header file for the motion controller functionality */
#include <motioncontroller/motioncontroller.hpp>
/* Header file for the sensor task functionality */
#include <examples/sensors/sensortask.hpp>
/* Header file  for the controller functionality */
#include <signal/controllers/controller.hpp>
/* Header file  for the sendor publisher functionality */
#include <examples/sensors/sensorpublisher.hpp>
/* Safety stop functionality */
#include <SafetyStop/safetystopfunction.hpp>
/* Quadrature encoder functionality */
#include <include/encoders/quadratureencodertask.hpp>
/* Examples with the sensors publisher */
#include <include/examples/sensors/sensortask.hpp>
//!  [List of included files]

/// Serial interface with the another device(like single board computer). It's an built-in class of mbed based on the UART comunication, the inputs have to be transmiter and receiver pins. 
Serial          g_rpi(USBTX, USBRX);
/** @brief 
 * This object is used to control the direction and the rotation speed of the wheel. The fist input respresents the pin for the servo motor, it must to generate a PWM signal. 
 * The second input  is the pin for generating PWM signal for the DC-Motor driver. The third and fourth inputs give the direction of the DC Motor, they are digital pins. The last input parameter represent an analog input pin, to measure the electric current.
 */
Move            g_car(D9, D3, D2, D4, A0);
/// Base sample time for the task manager. The measurement unit of base sample time is second.
const float     g_baseTick = 0.0001; // seconds
/// It's a task for blinking periodically the built-in led on the Nucleo board.
examples::CBlinker        g_blinker       (0.5    / g_baseTick, LED1);

/// The sample time of the encoder, is measured in second. 
float           g_period_Encoder = 0.001;

/// Create the motion controller, which controls the robot states and the robot moves based on the transmitted command over the serial interface. 
CMotionController           g_motionController(0.001, g_rpi, g_car);

/// Map with the key and the callback functions.If the message key equals to one of the enumerated keys, than it will be applied the corresponsive function. 
//! [List of messages]
utils::serial::CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"MCTL",mbed::callback(CMotionController::staticSerialCallbackMove,&g_motionController)},
    {"BRAK",mbed::callback(CMotionController::staticSerialCallbackBrake,&g_motionController)},
    {"HBRA",mbed::callback(CMotionController::staticSerialCallbackHardBrake,&g_motionController)},
    {"PIDA",mbed::callback(CMotionController::staticSerialCallbackPID,&g_motionController)},
    {"SPLN",mbed::callback(CMotionController::staticSerialCallbackSpline,&g_motionController)}
};

/// Create the serial monitor object, which decodes the messages and transmites the responses.
utils::serial::CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);
//! [List of messages]

/// List of the task, each task will be applied their own periodicity, defined by initializing the objects.
//! [List of task]
utils::task::CTask* g_taskList[] = {
    &g_blinker,
    &g_serialMonitor
}; 

/// Create the task manager, which applies periodically the tasks.
utils::task::CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(utils::task::CTask*), g_baseTick);
//! [List of task]

/**
 * @brief Setup function for initializing the objects
 * 
 * @return uint32_t 
 */
uint32_t setup()
{
    g_rpi.baud(460800);  
    g_rpi.printf("\r\n\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#   I'm alive   #\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("\r\n");
    /// Start the Rtos timer for the motion controller
    g_motionController.startRtosTimer();
    return 0;    
}

/**
 * @brief Loop function
 * 
 * @return uint32_t 
 */
uint32_t loop()
{
    g_taskManager.mainCallback();
    return 0;
}

/**
 * @brief Main function
 * 
 * @return int 
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