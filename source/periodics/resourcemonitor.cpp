#include "periodics/resourcemonitor.hpp"

#define _50_chars 50
#define max_percent_int 10000 // 100.00%

// TODO: Add your code here
namespace periodics
{
   /**
    * @brief Class constructorresourcemonitor
    *
    */
    CResourcemonitor::CResourcemonitor(std::chrono::milliseconds f_period, UnbufferedSerial& f_serial)
    : utils::CTask(f_period)
    , m_serial(f_serial)
    , m_isActive(false)
    {
        /* constructor behaviour */
    }

    /** @brief  CResourcemonitor class destructor
     */
    CResourcemonitor::~CResourcemonitor()
    {
    };

    void CResourcemonitor::serialCallbackRESMONCommand(char const * a, char * b){
        uint8_t l_isActivate=0;
        uint8_t l_res = sscanf(a,"%hhu",&l_isActivate);

        if(1 == l_res){
            if(uint8_globalsV_value_of_kl == 15 || uint8_globalsV_value_of_kl == 30)
            {
                m_isActive=(l_isActivate>=1);
                bool_globalsV_resource_isActive = (l_isActivate>=1);
                sprintf(b,"1");
            }
            else{
                sprintf(b,"kl 15/30 is required!!");
            }
            
        }else{
            sprintf(b,"syntax error");
        }
    }

    void CResourcemonitor::_run(){
        if(!m_isActive) return;
        
        char buffer[_50_chars];

        mbed_stats_heap_t heap_stats;
        mbed_stats_stack_t stack_stats;

        mbed_stats_heap_get(&heap_stats);
        mbed_stats_stack_get(&stack_stats);

        // Compute the usage percentage
        uint16_t heap_usage_percentage = (heap_stats.current_size * max_percent_int) / heap_stats.reserved_size;
        uint16_t stack_usage_percentage = (stack_stats.max_size * max_percent_int) / stack_stats.reserved_size;

        snprintf(buffer, sizeof(buffer), "@resourceMonitor:Heap (%d.%d);Stack (%d.%d);;\r\n", heap_usage_percentage/100, heap_usage_percentage%100,
                                            stack_usage_percentage/100, stack_usage_percentage%100);
        m_serial.write(buffer,strlen(buffer));
    }

}; // namespace periodics