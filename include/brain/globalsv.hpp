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

#ifndef GLOBALSV_HPP
#define GLOBALSV_HPP

#include <mbed.h>
#include <vector>
#include <algorithm>
#include <cstdint>

// TODO: Add your code here

extern uint64_t uint64_globalsV_instant_mAmpsH;

extern uint32_t uint32_globalsV_consumption_Total_mAmpsH;
extern uint32_t uint32_globalsV_numberOfMiliseconds_Total;
extern uint32_t uint32_globalsV_range_left_shutdown;
extern uint32_t currentEMA;

extern uint16_t uint16_globalsV_battery_totalVoltage;
extern uint16_t uint16_globalsV_battery_mAmps_user;
extern uint16_t readings[11]; 

extern uint8_t uint8_globalsV_value_of_kl;

extern uint8_t uint8_globalsV_alpha_scaled;
extern uint8_t uint8_globalsV_windowSize; 
extern uint8_t uint8_globalsV_index; 

extern bool bool_globalsV_imu_isActive;
extern bool bool_globalsV_instant_isActive;
extern bool bool_globalsV_battery_isActive;
extern bool bool_globalsV_resource_isActive;
extern bool bool_globalsV_ShuttedDown;
extern bool bool_globalsV_warningFlag;

#endif // GLOBALSV_HPP