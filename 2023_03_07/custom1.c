#include "IfxPort.h"
#include "Bsp.h"

#define A1         &MODULE_P00,6 
#define A2         &MODULE_P00,8 
#define A3         &MODULE_P00,10

#define WAIT_TIME     30

int a=0;

void port_cycle(void)
{
    IfxPort_setPinMode(A1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(A2, IfxPort_Mode_outputPushPullGeneral); 
    IfxPort_setPinMode(A3, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinHigh(A1);
    IfxPort_setPinHigh(A2);
    IfxPort_setPinHigh(A3);
}


void blinkLED(void)
{
    a++;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));
    if(a%1==0){IfxPort_togglePin(A1);}
    if(a%2==0){IfxPort_togglePin(A2);}
    if(a%4==0){IfxPort_togglePin(A3);}                                                    /* Toggle the state of the LED      */
    
}