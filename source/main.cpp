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

/* Header file for the motion controller functionality */
#include <main.hpp>

#define dummy_value 15

/// Base sample time for the task manager. The measurement unit of base sample time is second.
const std::chrono::milliseconds g_baseTick = std::chrono::milliseconds(1); // microseconds

// Serial interface with the another device(like single board computer). It's an built-in class of mbed based on the UART communication, the inputs have to be transmitter and receiver pins. 
UnbufferedSerial g_rpi(USBTX, USBRX, 115200);

// It's a task for blinking periodically the built-in led on the Nucleo board, signaling the code is uploaded on the nucleo.
periodics::CBlinker g_blinker(g_baseTick * 500, LED1);

periodics::CAlerts g_alerts(g_baseTick * 5000);

// // It's a task for sending periodically the instant current consumption of the battery
periodics::CInstantConsumption g_instantconsumption(g_baseTick * 1000, A2, g_rpi);

// // It's a task for sending periodically the battery voltage, so to notice when discharging
periodics::CTotalVoltage g_totalvoltage(g_baseTick*3000, A4, g_rpi);

// It's a task for sending periodically the IMU values
periodics::CImu g_imu(g_baseTick*150, g_rpi, I2C_SDA, I2C_SCL);

//PIN for a motor speed in ms, inferior and superior limit
drivers::CSpeedingMotor g_speedingDriver(D3, -500, 500); //speed in cm/s

//PIN for angle in servo degrees, inferior and superior limit
drivers::CSteeringMotor g_steeringDriver(D4, -250, 250);

// Create the motion controller, which controls the robot states and the robot moves based on the transmitted command over the serial interface.
brain::CRobotStateMachine g_robotstatemachine(g_baseTick * 50, g_rpi, g_steeringDriver, g_speedingDriver);

periodics::CResourcemonitor g_resourceMonitor(g_baseTick * 5000, g_rpi);

brain::CKlmanager g_klmanager(g_alerts, g_imu, g_instantconsumption, g_totalvoltage, g_robotstatemachine, g_resourceMonitor);

periodics::CPowermanager g_powermanager(g_baseTick * 100, g_klmanager, g_rpi, g_totalvoltage, g_instantconsumption, g_alerts);

brain::CBatterymanager g_batteryManager(dummy_value);

/* USER NEW COMPONENT BEGIN */

/* USER NEW COMPONENT END */

// Map for redirecting messages with the key and the callback functions. If the message key equals to one of the enumerated keys, than it will be applied the paired callback function.
drivers::CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"speed",          mbed::callback(&g_robotstatemachine, &brain::CRobotStateMachine::serialCallbackSPEEDcommand)},
    {"steer",          mbed::callback(&g_robotstatemachine, &brain::CRobotStateMachine::serialCallbackSTEERcommand)},
    {"brake",          mbed::callback(&g_robotstatemachine, &brain::CRobotStateMachine::serialCallbackBRAKEcommand)},
    {"vcd",            mbed::callback(&g_robotstatemachine, &brain::CRobotStateMachine::serialCallbackVCDcommand)},
    {"battery",        mbed::callback(&g_totalvoltage,      &periodics::CTotalVoltage::serialCallbackTOTALVcommand)},
    {"instant",        mbed::callback(&g_instantconsumption,&periodics::CInstantConsumption::serialCallbackINSTANTcommand)},
    {"imu",            mbed::callback(&g_imu,               &periodics::CImu::serialCallbackIMUcommand)},
    {"kl",             mbed::callback(&g_klmanager,         &brain::CKlmanager::serialCallbackKLCommand)},
    {"batteryCapacity",mbed::callback(&g_batteryManager,    &brain::CBatterymanager::serialCallbackBATTERYCommand)},
    {"resourceMonitor",mbed::callback(&g_resourceMonitor,   &periodics::CResourcemonitor::serialCallbackRESMONCommand),}
};

// Create the serial monitor object, which decodes, redirects the messages and transmits the responses.
drivers::CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);

// List of the task, each task will be applied their own periodicity, defined by the initializing the objects.
utils::CTask* g_taskList[] = {
    &g_blinker,
    &g_instantconsumption,
    &g_totalvoltage,
    &g_imu,
    &g_robotstatemachine,
    &g_serialMonitor,
    &g_powermanager,
    &g_resourceMonitor,
    &g_alerts,
    // USER NEW PERIODICS BEGIN -
    
    // USER NEW PERIODICS END
}; 

// Create the task manager, which applies periodically the tasks, miming a parallelism. It needs the list of task and the time base in seconds. 
utils::CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(utils::CTask*), g_baseTick);

/**
 * @brief Setup function for initializing some objects and transmitting a startup message through the serial. 
 * 
 * @return uint32_t Error level codes error's type.
 */
uint8_t setup()
{
    // g_rpi.format(
    //     /* bits */ 8,
    //     /* parity */ SerialBase::None,
    //     /* stop bit */ 1
    // );
    g_rpi.write("\r\n\r\n", 4);
    g_rpi.write("#################\r\n", 19);
    g_rpi.write("#               #\r\n", 19);
    g_rpi.write("#   I'm alive   #\r\n", 19);
    g_rpi.write("#               #\r\n", 19);
    g_rpi.write("#################\r\n", 19);
    g_rpi.write("\r\n", 2);

    return 0;    
}

/**
 * @brief Loop function has aim to apply repeatedly task
 * 
 * @return uint32_t Error level codes error's type.
 */
uint8_t loop()
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
    uint8_t  l_errorLevel = setup();

    while(!l_errorLevel) 
    {
        if(bool_globalsV_ShuttedDown) {
            ThisThread::sleep_for(chrono::milliseconds(200));
            hal_deepsleep();
        }
        l_errorLevel = loop();
    }
    
    return l_errorLevel;
}