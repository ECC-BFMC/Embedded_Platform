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


/* The mbed library */
#include <mbed.h>
/* Task manager */
#include <utils/taskmanager/taskmanager.hpp>
/* Header file for the blinker functionality */
#include <periodics/blinker.hpp>
/* Header file for the serial communication functionality */
#include <utils/serial/serialmonitor.hpp>
/* Header file for the motion controller functionality */
#include <brain/robotstatemachine.hpp>
/* Header file for the sensor task functionality */
#include <periodics/encoderpublisher.hpp>
/* Header file  for the controller functionality */
#include <signal/controllers/motorcontroller.hpp>
/* Quadrature encoder functionality */
#include <hardware/encoders/quadratureencoder.hpp>


/// Serial interface with the another device(like single board computer). It's an built-in class of mbed based on the UART comunication, the inputs have to be transmiter and receiver pins. 
RawSerial          g_rpi(USBTX, USBRX);
/** @brief 
 * This object is used to control the direction and the rotation speed of the wheel. 
 * The D3 output pin is for generating PWM signal for the DC-Motor driver. 
 * The D2 and D4 give the direction of the DC Motor, they are output digital pins. 
 * The A0 parameter represent an analog input pin, to measure the electric current used by the motor driver.
 * D9 reppresents the output pin for the servo motor, it must to generate a PWM signal. 
 */
hardware::drivers::CMotorDriverVnh g_motorVnhDriver(D3, D2, D4, -0.30, 0.30);
hardware::drivers::CSteeringMotor g_steeringDriver(D9, -23.0, 23.0);

/// Base sample time for the task manager. The measurement unit of base sample time is second.
const float g_baseTick = 0.0001; // seconds

/// It's a task for blinking periodically the built-in led on the Nucleo board, signaling the code is uploaded on the nucleo.
periodics::CBlinker g_blinker(0.5 / g_baseTick, LED1);

/// The sample time of the encoder, it is measured in second. 
float g_period_Encoder = 0.001;

/// Create a filter object for filtrating the noise appeared on the encoder.
signal::filter::lti::siso::CIIRFilter<float,1,2> g_encoderFilter(utils::linalg::CRowVector<float,1>({ -0.77777778}), utils::linalg::CRowVector<float,2>({0.11111111,0.11111111}));

/// Create a quadrature encoder object. It periodically measueres the rotary speed of the motor and applies the given filter. 
hardware::encoders::CQuadratureEncoderWithFilter g_quadratureEncoderTask(g_period_Encoder,hardware::encoders::CQuadratureCounter_TIM4::Instance(),2048,g_encoderFilter);

///Create an encoder publisher object to transmite the rotary speed of the dc motor to the RPi (if enabled)
periodics::CEncoderPublisher   g_encoderPublisher(0.01/g_baseTick,g_quadratureEncoderTask,g_rpi);

/// Create a spline based converter object to convert the volt signal to pwm signal for the motor driver.
signal::controllers::CConverterSpline<2,1> l_volt2pwmConverter({-0.22166,0.22166},{std::array<float,2>({0.1041568079746662,-0.08952760561569219}),std::array<float,2>({0.50805,0.0}),std::array<float,2>({0.1041568079746662,0.08952760561569219})});

/// Create a PID controller object, with the sampling time calculation equal to the one of the readings of the encoder;
signal::controllers::siso::CPidController<double> l_pidController( 0.115000,0.810000,0.000222,0.040000,g_period_Encoder);

/// Create a controller object based on the predefined PID controller, the quadrature encoder and the spline object;
signal::controllers::CMotorController g_controller(g_quadratureEncoderTask, l_pidController, &l_volt2pwmConverter, -310299.0, 310299.0);

/// Create the motion controller, which controls the robot states and the robot moves based on the transmitted command over the serial interface. 
brain::CRobotStateMachine g_robotstatemachine(g_period_Encoder, g_rpi, g_motorVnhDriver,g_steeringDriver, &g_controller);

/// Map for redirecting messages with the key and the callback functions. If the message key equals to one of the enumerated keys, than it will be applied the paired callback function.
utils::serial::CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"1",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackSPEEDcommand)},
    {"2",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackSTEERcommand)},
    {"3",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackBRAKEcommand)},
    {"4",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackACTIVPIDcommand)},
    {"5",mbed::callback(&g_encoderPublisher,&periodics::CEncoderPublisher::serialCallbackENCODERPUBcommand)},
    {"6",mbed::callback(&l_pidController,&signal::controllers::siso::CPidController<double>::serialCallbackTUNEPIDcommand)},
    {"7",mbed::callback(&g_robotstatemachine,&brain::CRobotStateMachine::serialCallbackMOVEcommand)}
};

/// Create the serial monitor object, which decodes, redirects the messages and transmites the responses.
utils::serial::CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);

/// List of the task, each task will be applied their own periodicity, defined by the initializing the objects.
utils::task::CTask* g_taskList[] = {
    &g_blinker,
    &g_serialMonitor,
    &g_encoderPublisher
}; 

/// Create the task manager, which applies periodically the tasks, miming a parallelism. It needs the list of task and the time base in seconds. 
utils::task::CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(utils::task::CTask*), g_baseTick);

/**
 * @brief Setup function for initializing some objects and transmiting a startup message through the serial. 
 * 
 * @return uint32_t Error level codes error's type.
 */
uint32_t setup()
{
    g_rpi.baud(19200); 
    g_rpi.printf("\r\n\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#   I'm alive   #\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("\r\n");
    /// Start periodic task for the quadrature encoder    
    g_quadratureEncoderTask.startTimer();
    /// Start tperiodic task for the motion controller
    g_robotstatemachine.startTimer();
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
    g_rpi.printf("exiting with code: %ld",l_errorLevel);
    return l_errorLevel;
}