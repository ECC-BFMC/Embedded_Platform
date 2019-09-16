/**
  ******************************************************************************
  * @file    main.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   Application entry point.
  ******************************************************************************
 */
/* The mbed library */
#include <mbed.h>
/* Task manager */
#include<TaskManager/taskmanager.hpp>
/* Header file for the MOVE functionality */
#include <Move/move.hpp>
/* Header file for the blinker functionality */
#include <Examples/blinker.hpp>
/* Header file for the serial communication functionality */
#include <SerialMonitor/serialmonitor.hpp>
/* Header file for the motion controller functionality */
#include <MotionController/motioncontroller.hpp>
/* Header file for the sensor task functionality */
#include <Examples/Sensors/sensortask.hpp>
/* Header file  for the controller functionality */
#include <Controllers/controller.hpp>
/* Header file  for the sendor publisher functionality */
#include <Examples/Sensors/sensorpublisher.hpp>
/* Safety stop functionality */
#include <SafetyStop/safetystopfunction.hpp>
/* Quadrature encoder functionality */
#include <include/Encoders/quadratureencodertask.hpp>
/* Examples with the sensors publisher */
#include <include/Examples/Sensors/sensortask.hpp>

/// Serial interface with the another device(like single board computer). It's an built-in class of mbed based on the UART comunication, the inputs have to be transmiter and receiver pins. 
Serial          g_rpi(USBTX, USBRX);

Move            g_car(D9, D3, D2, D4, A0);
/// Base sample time for the task manager. The measurement unit of base sample time is second.
const float     g_baseTick = 0.0001; // seconds
/// It's a task for blinking periodically the built-in led on the Nucleo board.
examples::CBlinker        g_blinker       (0.5    / g_baseTick, LED1);

/// The sample time of the encoder, is measured in second. 
float           g_period_Encoder = 0.001;

/// Create a filter object for filtrating the noise appeared on the rotary encoder.
//! [Create encoder filter]
filter::lti::siso::CIIRFilter<float,1,2> g_encoderFilter(linalg::CRowVector<float,1>({ -0.77777778})
                                                        ,linalg::CRowVector<float,2>({0.11111111,0.11111111}));
//! [Create encoder filter]

/// Create a quadrature encoder object with a filter. It periodically measueres the rotary speed of the motor and applies the given filter. 
encoders::CQuadratureEncoderWithFilterTask g_quadratureEncoderTask(g_period_Encoder,encoders::CQuadratureEncoder_TIM4::Instance(),2048,g_encoderFilter);

///Create an encoder publisher object to transmite the rotary speed of the dc motor. 
examples::sensors::CEncoderSender         g_encoderPublisher(0.01/g_baseTick,g_quadratureEncoderTask,g_rpi);


/// Create a splines based converter object to convert the volt signal to pwm signal
controllers::CConverterSpline<2,1> l_volt2pwmConverter({-0.0007836798991808444,0.0007836798991808444},{std::array<float,2>({0.043799873976055455,-0.050627}),std::array<float,2>({0.30029741650913677,0.0}),std::array<float,2>({0.043799873976055455,0.050627})});
//Create the controller transfer function for the motor
/// Create a discrete transfer function, which respresents a discrete PID controller.

//! [Create PID controller]
systemmodels::lti::siso::CDiscreteTransferFucntion<double,3,3> g_motorPIDTF(linalg::CRowVector<double, 3>({ 0.12058,-0.23751,0.11695}).transpose(),linalg::CRowVector<double, 3>({ 1.00000, -1.97531  ,0.97531}).transpose());
controllers::siso::CPidController<double> l_pidController(g_motorPIDTF,g_period_Encoder);

controllers::siso::CPidController<double> l_pidController2( 0.1150,0.81000,0.000222,0.04,g_period_Encoder);

controllers::CControllerSiso g_controller(g_quadratureEncoderTask,l_pidController,&l_volt2pwmConverter);
//! [Create PID controller]

/// Create the motion controller, which controls the robot states and the robot's moves based on the transmitted command over the serial interface. 
CMotionController           g_motionController(g_period_Encoder, g_rpi, g_car,&g_controller);

/// Map for redirecting messages with the key and the callback functions. If the message key equals to one of the enumerated keys, than it will be applied the paired callback function.
serial::CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"MCTL",mbed::callback(CMotionController::staticSerialCallbackMove,&g_motionController)},
    {"BRAK",mbed::callback(CMotionController::staticSerialCallbackBrake,&g_motionController)},
    {"PIDA",mbed::callback(CMotionController::staticSerialCallbackPID,&g_motionController)},
    {"SPLN",mbed::callback(CMotionController::staticSerialCallbackSpline,&g_motionController)},
    {"ENPB",mbed::callback(examples::sensors::CEncoderSender::staticSerialCallback,&g_encoderPublisher)},
};

/// Create the serial monitor object, which decodes, redirects the messages and transmites the responses.
serial::CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);

/// List of the task, each task will be applied their own periodicity, defined by initializing the objects.
task::CTask* g_taskList[] = {
    &g_blinker,
    &g_serialMonitor,
    &g_encoderPublisher
}; 

/// Create the task manager, which applies periodically the tasks. It needs the list of task and the time base in seconds. 
task::CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(task::CTask*), g_baseTick);


/**
 * @brief Setup function for initializing some objects and transmiting a startup message through the serial. 
 * 
 * @return uint32_t Error level codes error's type.
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
    /// Start the Rtos timer for the quadrature encoder    
    g_quadratureEncoderTask.startTimer();
    /// Start the Rtos timer for the motion controller
    g_motionController.startRtosTimer();
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