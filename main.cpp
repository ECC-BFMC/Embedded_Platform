#include "mbed.h"
#include "rtos.h"


void test(){
    printf("Test");
    wait(1.0);
}

int main()
{
    RtosTimer         m_timer(mbed::callback(test));
}
