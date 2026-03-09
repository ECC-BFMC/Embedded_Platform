#include "utils/headlight.hpp"
#include <cstdio>

namespace utils
{

CHeadlight::CHeadlight(std::chrono::milliseconds period, PinName pin, const std::string &name)
    : CTask(period),
      m_pin(pin, 1),  // Initialize as HIGH (LED OFF) since LOW = ON
      m_name(name),
      m_isActive(false)
{
    m_pin = 1; // Headlight initially OFF (HIGH state)
}

CHeadlight::~CHeadlight()
{
}

void CHeadlight::set(bool state)
{
    m_isActive = state;
    // Invert logic: LOW(0) = ON, HIGH(1) = OFF
    m_pin = !m_isActive;
}

bool CHeadlight::get() const
{
    return m_isActive;
}

const std::string &CHeadlight::getName() const
{
    return m_name;
}

void CHeadlight::_run()
{
    // No periodic task needed - all state changes go through set()
}

void CHeadlight::serialCallbackHeadlightCommand(const char *message, char *response)
{
    uint8_t val = 0;
    int res = sscanf(message, "%hhu", &val);

    if (res == 1)
    {
        set(val >= 1);
        sprintf(response, "OK");
    }
    else
    {
        sprintf(response, "syntax error");
    }
}

} // namespace utils