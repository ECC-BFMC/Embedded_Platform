#ifndef UTILS_HEADLIGHT_HPP
#define UTILS_HEADLIGHT_HPP

#include "mbed.h"
#include "task.hpp"
#include <string>

namespace utils
{

/**
 * @brief Headlight task class for BFMC robot
 * 
 * Inherits from CTask so it can be scheduled periodically.
 * Supports serial commands via serialCallbackHeadlightCommand.
 */
class CHeadlight : public CTask
{
public:
    /**
     * @brief Construct a new CHeadlight object
     * 
     * @param period Task period in milliseconds
     * @param pin GPIO pin for the LED
     * @param name Name of the headlight (used for serial commands)
     */
    CHeadlight(std::chrono::milliseconds period, PinName pin, const std::string &name);

    ~CHeadlight() override;

    /**
     * @brief Serial command callback for controlling headlight
     * 
     * Expects message "0" or "1".
     */
    void serialCallbackHeadlightCommand(const char *message, char *response);

    /**
     * @brief Turn the headlight ON/OFF programmatically
     */
    void set(bool state);

    /**
     * @brief Get current headlight state
     */
    bool get() const;

    /**
     * @brief Get the headlight name
     */
    const std::string &getName() const;

protected:
    /**
     * @brief Task execution function, called by CTask scheduler
     */
    void _run() override;

private:
    DigitalOut m_pin;  // Active LOW - LOW state turns LED on
    std::string m_name;
    bool m_isActive;
};

} // namespace utils

#endif // UTILS_HEADLIGHT_HPP