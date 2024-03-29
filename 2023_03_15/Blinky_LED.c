/**********************************************************************************************************************
 * \file Blinky_led.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "IfxPort.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define led1 &MODULE_P00,0
#define led2 &MODULE_P00,1
#define led3 &MODULE_P00,2
#define led4 &MODULE_P00,3
#define led5 &MODULE_P00,5
#define led6 &MODULE_P00,6
#define led7 &MODULE_P00,7
#define led8 &MODULE_P00,8

#define WAIT_TIME   400

#define IOport1 &MODULE_P23,0
#define IOport2 &MODULE_P23,1
#define IOport3 &MODULE_P23,2
#define IOport4 &MODULE_P23,3
#define IOport5 &MODULE_P23,4
#define IOport6 &MODULE_P23,5
#define IOport7 &MODULE_P22,0
#define IOport8 &MODULE_P22,2

uint8 rxtx_buff[8] = {0,};
void setdata();
/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* This function initializes the port pin which drives the led */
void initOUTPUT(void)
{
    IfxPort_setPinModeOutput(led1, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led2, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led3, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led4, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led5, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led6, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led7, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(led8, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
}

void initINPUT(void)
{
    IfxPort_setPinMode(IOport1, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport2, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport3, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport4, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport5, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport6, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport7, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(IOport8, IfxPort_Mode_inputPullUp);
}

//////////////////////////// 16 TO 10 TO 2 //////////////////////////

void setdata(int count, int rxtx)     //int 형을 받아서 10진수로
{
    for(int i = 0;i < count;i++)
    {
        rxtx_buff[i] = rxtx % 2;
        rxtx /= 2;
    }
}

//////////////////////////// 16 TO 10 TO 2 //////////////////////////

void val(int rxtx)
{
    if(rxtx >= 64)   setdata(7,rxtx);
    else if(rxtx >= 32)   setdata(6,rxtx);
    else if(rxtx >= 16)   setdata(5,rxtx);
    else if(rxtx >= 8)    setdata(4,rxtx);
    else if(rxtx >= 4)    setdata(3,rxtx);
    else if(rxtx >= 2)    setdata(2,rxtx);
    else if(rxtx == 1)     rxtx_buff[0] = 1;

    if(rxtx_buff[0] == 1)    IfxPort_setPinHigh(led1);
    else IfxPort_setPinLow(led1);
    if(rxtx_buff[1] == 1)    IfxPort_setPinHigh(led2);
    else IfxPort_setPinLow(led2);
    if(rxtx_buff[2] == 1)    IfxPort_setPinHigh(led3);
    else IfxPort_setPinLow(led3);
    if(rxtx_buff[3] == 1)    IfxPort_setPinHigh(led4);
    else IfxPort_setPinLow(led4);
    if(rxtx_buff[4] == 1)    IfxPort_setPinHigh(led5);
    else IfxPort_setPinLow(led5);
    if(rxtx_buff[5] == 1)    IfxPort_setPinHigh(led6);
    else IfxPort_setPinLow(led6);
    if(rxtx_buff[6] == 1)    IfxPort_setPinHigh(led7);
    else IfxPort_setPinLow(led7);
    if(rxtx_buff[7] == 1)    IfxPort_setPinHigh(led8);
    else IfxPort_setPinLow(led8);
 }




/////////////////////////////reset/////////////////////////////

void resetdata(void)
{
    for(int i = 0; i<=7; i++)
    {
        rxtx_buff[i] = 0;
    }
}

//////////////////////////////////////////////////////////////



/*int INPUT_control(void)
{

}*/
