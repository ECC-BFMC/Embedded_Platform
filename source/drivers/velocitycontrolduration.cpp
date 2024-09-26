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

#include "drivers/velocitycontrolduration.hpp"

#define scale_ds_to_ms 100

namespace drivers{

    CVelocityControlDuration::CVelocityControlDuration(
        std::chrono::milliseconds f_period,
        drivers::ISteeringCommand& f_steeringControl,
        drivers::ISpeedingCommand& f_speedingControl,
        UnbufferedSerial& f_serial
    )
    : utils::CTask(f_period)
    , m_steeringControl(f_steeringControl)
    , m_speedingControl(f_speedingControl)
    , m_isActive(false)
    , m_ticksRun(0)
    , m_period((uint16_t)(f_period.count()))
    , m_serial(f_serial)
    {
    };

    CVelocityControlDuration::~CVelocityControlDuration()
    {
    };

    // Command handler for setting speed and steering based on a received message.
    void CVelocityControlDuration::serialCallbackVCDCommand(const char* message, char* response)
    {
        int speed, steer;
        uint8_t time_deciseconds;

        uint8_t parsed = sscanf(message, "%d;%d;%hhu", &speed, &steer, &time_deciseconds);

        m_targetTime = time_deciseconds;

        if(parsed == 3 && speed <= 50 && speed >= -50 && steer <= 232 && steer >= -232)
        {
            sprintf(response, "%d;%d;%d", speed, steer, time_deciseconds);

            m_isActive = true;
            m_ticksRun = 0;

            m_targetTime = time_deciseconds * scale_ds_to_ms;

            m_steeringControl.setAngle(steer);
            m_speedingControl.setSpeed(-speed);
        }
        else
        {
            sprintf(response, "something went wrong");
        }
    };

    // Main run function for the task. Controls the speed and steering based on set parameters.
    void CVelocityControlDuration::_run()
    {
        if(!m_isActive) return;

        // If the accumulated ticks exceed the target time, stop the movement and deactivate the task.
        if(m_ticksRun >= m_targetTime+m_period)
        {
            m_speedingControl.setSpeed(0);
            m_steeringControl.setAngle(0);
            m_isActive = false;
            m_serial.write("@vcd:0;0;0;;\r\n", 15);
        }
        else
        {
            // Otherwise, increment the tick counter.
            m_ticksRun += m_period;
        }
    };
}

// TODO: Add your code here
