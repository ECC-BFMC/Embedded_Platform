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
/* Distrance sensors read methods */
#include <include/SHARP_IR_distance\DistanceSensors\distancesensors.hpp>
/* SHARP IR sensor */
#include <include/SHARP_IR_distance\IR_DistanceSensorDriver\ir_distancesensordriver.hpp>

/// Serial interface with the another device(like single board computer). It's an built-in class of mbed based on the UART comunication, the inputs have to be transmiter and receiver pins. 
Serial          g_rpi(USBTX, USBRX,460800);
/** @brief 
 * This object is used to control the direction and the rotation speed of the wheel. The fist input respresents the pin for the servo motor, it must to generate a PWM signal. 
 * The second input  is the pin for generating PWM signal for the DC-Motor driver. The third and fourth inputs give the direction of the DC Motor, they are digital pins. The last input parameter represent an analog input pin, to measure the electric current.
 */
Move            g_car(D9, D3, D2, D4, A0);
/// Base sample time for the task manager. The measurement unit of base sample time is second.
const float     g_baseTick = 0.0001; // seconds
/// It's a task for blinking periodically the built-in led on the Nucleo board.
examples::CBlinker        g_blinker       (0.5    / g_baseTick, LED1);

/// The safetystop base period
const float     g_safetyStop_period = 0.25; // seconds

/// Creating the infra-red proximity sensors drivers. In this example, they're put on the front of the vehicle.
C_IR_DistanceSensorDriver g_ir_sensor1(A1, D12);// left front sensor
C_IR_DistanceSensorDriver g_ir_sensor2(A3, D8); // mid left front sensor
C_IR_DistanceSensorDriver g_ir_sensor3(A4, D5); // center frint sensor
C_IR_DistanceSensorDriver g_ir_sensor4(A5, D6); // mid right front sensor
C_IR_DistanceSensorDriver g_ir_sensor5(D11, D7); // right front

/// The list of the proximity sensors on the front of the robot
std::array<C_IR_DistanceSensorDriver*, 5> g_front_sersor_array = {&g_ir_sensor1,&g_ir_sensor2, &g_ir_sensor3, &g_ir_sensor4, &g_ir_sensor5};
/// The object for control the sequential activating of the front sensors.
CDistanceSensors<std::array<C_IR_DistanceSensorDriver*, 5>> g_frontSensors(g_safetyStop_period/g_baseTick, g_baseTick, g_front_sersor_array);
/// The safety stop object verifies the robot state and the generate a flag in the case of emergency braking
CSafetyStopFunction g_safetyStop(g_ir_sensor1, g_ir_sensor2, g_ir_sensor3, g_ir_sensor4, g_ir_sensor5);


/// The sample time of the encoder, is measured in second. 
float           g_period_Encoder = 0.001;

/// Create a filter object for filtrating the noise appeared on the rotary encoder.
filter::lti::siso::CIIRFilter<float,1,2> g_encoderFilter(linalg::CRowVector<float,1>({ -0.77777778})
                                                        ,linalg::CRowVector<float,2>({0.11111111,0.11111111}));
/// Create a quadrature encoder object with a filter. It periodically measueres the rotary speed of the motor and applies the given filter. 
encoders::CQuadratureEncoderWithFilterTask g_quadratureEncoderTask(g_period_Encoder,encoders::CQuadratureEncoder_TIM4::Instance(),2048,g_encoderFilter);

///Create an encoder publisher object to transmite the rotary speed of the dc motor. 
examples::sensors::CEncoderSender         g_encoderPublisher(0.01/g_baseTick,g_quadratureEncoderTask,g_rpi);

//Create an object to convert volt to pwm for motor driver
/// Create a splines based converter object to convert the volt signal to pwm signal
controllers::CConverterSpline<2,1> l_volt2pwmConverter({-0.22166,0.22166},{std::array<float,2>({0.1041568079746662,-0.08952760561569219}),std::array<float,2>({0.50805,0.0}),std::array<float,2>({0.1041568079746662,0.08952760561569219})});
//  controllers::siso::CPidController<double> l_pidController(g_motorPIDTF,g_period_Encoder);
controllers::siso::CPidController<double> l_pidController( 0.1150,0.81000,0.000222,0.04,g_period_Encoder);
/// Create a controller object based on the predefined PID controller and the quadrature encoder
controllers::CControllerSiso g_controller(g_quadratureEncoderTask,l_pidController,&l_volt2pwmConverter);
/// Create the motion controller, which controls the robot states and the robot moves based on the transmitted command over the serial interface. 
CMotionController           g_motionController(g_period_Encoder, g_rpi, g_car,&g_safetyStop,&g_controller);

/// Map with the key and the callback functions.If the message key equals to one of the enumerated keys, than it will be applied the corresponsive function. 
serial::CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"MCTL",mbed::callback(CMotionController::staticSerialCallbackMove,&g_motionController)},
    {"BRAK",mbed::callback(CMotionController::staticSerialCallbackBrake,&g_motionController)},
    {"HBRA",mbed::callback(CMotionController::staticSerialCallbackHardBrake,&g_motionController)},
    {"PIDA",mbed::callback(CMotionController::staticSerialCallbackPID,&g_motionController)},
    {"SPLN",mbed::callback(CMotionController::staticSerialCallbackSpline,&g_motionController)},
    {"ENPB",mbed::callback(examples::sensors::CEncoderSender::staticSerialCallback,&g_encoderPublisher)},
};

/// Create the serial monitor object, which decodes the messages and transmites the responses.
serial::CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);

//! [Adding a resource]
/// List of the task, each task will be applied their own periodicity, defined by initializing the objects.
task::CTask* g_taskList[] = {
    &g_blinker,
    &g_serialMonitor,
    &g_encoderPublisher
}; 
//! [Adding a resource]

/// Create the task manager, which applies periodically the tasks.
task::CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(task::CTask*), g_baseTick);

/**
 * @brief Setup function for initializing the objects
 * 
 * @return uint32_t 
 */
uint32_t setup()
{
    // g_rpi.baud(460800);  
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