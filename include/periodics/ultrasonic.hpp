#ifndef HCSR04_HPP
#define HCSR04_HPP

#include "mbed.h"
#include "utils/task.hpp"

namespace periodics {

class CHCSR04 : public utils::CTask {
public:
    /**
     * @brief Constructor for HCSR04 ultrasonic sensor
     * @param period Task period in milliseconds
     * @param triggerPin Trigger pin (e.g., PA_0)
     * @param echoPin Echo pin (e.g., PA_1)
     * @param serial Serial interface for output
     * @param name Sensor name for identification
     */
    CHCSR04(std::chrono::milliseconds period, PinName triggerPin, PinName echoPin, 
            UnbufferedSerial& serial, const char* name = "ultrasonic");
    
    ~CHCSR04();
    
    /**
     * @brief Main callback function called periodically
     */
    void mainCallback() override;
    
    /**
     * @brief Get the latest distance reading in cm
     */
    float getDistance() const;
    
    /**
     * @brief Serial callback for command handling
     */
    void serialCallbackUltrasonicCommand(const char* message);
    
    std::string m_name;

private:
    DigitalOut m_trigger;
    InterruptIn m_echo;
    Timer m_timer;
    float m_distance;
    Mutex m_mutex;
    
    uint32_t m_pulseStart;
    uint32_t m_pulseEnd;
    bool m_measuring;
    
    /**
     * @brief ISR for echo pin rising edge
     */
    void echoRise();
    
    /**
     * @brief ISR for echo pin falling edge
     */
    void echoFall();
    
    /**
     * @brief Trigger a measurement
     */
    void triggerMeasurement();
    
    /**
     * @brief Calculate distance from pulse width
     */
    void calculateDistance();
};

} // namespace periodics

#endif // HCSR04_HPP