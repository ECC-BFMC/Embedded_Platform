#ifndef DRIVER_TASK_H
#define DRIVER_TASK_H

#include <TaskManager.h>
#include <DistanceDriver.h>
#include <filter.h>




template<class T>
class CDriverTask: public task::CTask
{
    public:
        using FilterFunc=filter::CFilterFunction<T>;
        CDriverTask( uint32_t f_period
                    ,PinName f_pin
                    ,FilterFunc& f_filter
                    ,Serial& f_serial)
                    :CTask(f_period)
                    ,m_filter(f_filter)
                    ,m_distanceDriver(f_pin)
                    ,m_val_brut(0.0)
                    ,m_val_filtrat(0.0)
                    ,m_serial(f_serial)
                    ,idx(0)
        {
        }
    
        T getDistanceFiltered(){
            return m_val_filtrat;
        }

        T getDistanceMeasured(){
            return m_val_brut;
        }

        
    
    private:
        virtual void _run(){
            m_val_brut=m_distanceDriver.ReadDistance();
            m_val_filtrat=m_filter(m_val_brut);
        }

        FilterFunc&     m_filter;
        DistanceDriver  m_distanceDriver;
        T               m_val_brut;
        T               m_val_filtrat;
        Serial& m_serial;
        int idx;
};
#endif