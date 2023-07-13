/**********************************************************************************************************************
 * \file GPT12_PWM_Generation.c
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
#include "GPT12_PWM_Generation.h"
#include "ERU_Interrupt.h"
#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_GPT12_TIMER    6                       /* Define the GPT12 Timer interrupt priority            */
#define ISR_PROVIDER_GPT12_TIMER    IfxSrc_Tos_cpu0         /* Interrupt provider                                   */
#define PWM_FREQUENCY               50.0f                    /* Frequency of the PWM signal in Hz                    */
#define GPT1_BLOCK_PRESCALER        32                      /* GPT1 block prescaler value                           */
#define TIMER_T3_INPUT_PRESCALER    32                      /* Timer input prescaler value                          */
#define TIMER_CYCLE                 10                      //ms
#define PWM_PIN                     &MODULE_P00, 5
#define MOTOR_ENA                   &MODULE_P00, 6
#define MOTOR_ENB                   &MODULE_P00, 9
#define Kp 0.35
#define Ki 0.05
#define Kd 0.8

uint16 PWM_DUTY_CYCLE=0;                                   /* Duty cycle of the PWM signal in percentage           */
uint16 PWM_output=0;

uint32 target_encoder=0;
uint32 g_previousCntVal = 0;
uint32 timer_cnt=0;

uint64 finalCntVal = 0;

long encoderPos_read=0;

float32 encoder_error=0;
float32 encoder_error_d=0;
float32 encoder_error_s=0;
float32 encoder_error_old=0;

long Debug_encoder=0;
int Debug_speed=0;

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/


void motor_contor(float32 speed){
    if(speed < 0){
        IfxPort_setPinState(MOTOR_ENA,IfxPort_State_high);
        IfxPort_setPinState(MOTOR_ENB,IfxPort_State_low);
        PWM_DUTY_CYCLE = -speed;
    }
    else{
        IfxPort_setPinState(MOTOR_ENA,IfxPort_State_low);
        IfxPort_setPinState(MOTOR_ENB,IfxPort_State_high);
        PWM_DUTY_CYCLE = speed;
    }
}

void update_encoderPos(void){
    encoderPos_read=return_encoderPos();
    Debug_encoder = encoderPos_read;
}

void Encoder_PID_Contor(void){

    encoder_error=target_encoder - encoderPos_read;
    encoder_error_s += encoder_error;
    encoder_error_d = encoder_error - encoder_error_old;

    encoder_error_s = (encoder_error_s >=  16) ?  16 : encoder_error_s;
    encoder_error_s = (encoder_error_s <= -16) ? -16 : encoder_error_s;

    PWM_output = Kp*encoder_error + Kd*encoder_error_d + Ki*encoder_error_s;

    PWM_output = (PWM_output >=  100) ?  100 : PWM_output;
    PWM_output = (PWM_output <= -100) ? -100 : PWM_output;

    Debug_speed=PWM_output;

    if(fabs(encoder_error) <= 2){
        motor_contor(0);
        encoder_error_s = 0;
    }
    else{
        motor_contor(PWM_output);
    }

    encoder_error_old=encoder_error;
}


/* Macro to define Interrupt Service Routine */
IFX_INTERRUPT(interruptGpt12, 0, ISR_PRIORITY_GPT12_TIMER);

/* Interrupt Service Routine of the GPT12 */
void interruptGpt12(void)
{
    /* Toggle the state of the LED */
    IfxPort_togglePin(PWM_PIN);
    timer_cnt++;
    target_encoder = 300;
    Encoder_PID_Contor();
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER,0.1*PWM_DUTY_CYCLE));
    IfxPort_togglePin(PWM_PIN);
}

/* Function to initialize the GPT12 module and the LED */
void initGpt12PWM(void)
{
    /* Enable the GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120);
    /* Set GPT1 block prescaler to divide the module frequency */
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_32);
    /* Set the GPT12 timer T3 in timer mode */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    /* Set the Timer T3 direction   */
    IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
    /* Set timer T3 input prescaler to divide the timer frequency */
    IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_32);

    /* Calculate dutyUpTime and dutyDownTime for reloading timer T3 */
    float32 moduleFreq = IfxGpt12_getModuleFrequency(&MODULE_GPT120);
    float32 fTimer = (moduleFreq / (GPT1_BLOCK_PRESCALER * TIMER_T3_INPUT_PRESCALER));
    uint16 dutyUpTime = (fTimer * (PWM_DUTY_CYCLE / 100.0f)) / PWM_FREQUENCY;
    uint16 dutyDownTime = (fTimer * (1 - (PWM_DUTY_CYCLE / 100.0f))) / PWM_FREQUENCY;

    /* Set timer T3 value */
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, dutyDownTime);

    /* Timer T2: reloads the value dutyDownTime in timer T3 */
    /* Set the timer T2 in reload mode */
    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);
    /* Set the T2 input parameter: Negative transition (falling edge) of T3 toggle latch T3OTL */
    IfxGpt12_T2_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_fallingEdgeTxOTL);
    /* Set timer T2 value (the value that is reloaded in T3 on negative transition) */
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, dutyDownTime);

    /* Timer T4: reloads the value dutyUpTime in timer T3 */
    /* Set timer T4 in reload mode */
    IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);
    /* Set the T4 input parameter: Positive transition (rising edge) of T3 toggle latch T3OTL */
    IfxGpt12_T4_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_risingEdgeTxOTL);
    /* Set timer T4 value (the value that is reloaded in T3 on positive transition) */
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, dutyUpTime);

    /* Configure the GPT12 interrupt */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T3_getSrc(&MODULE_GPT120);            /* Get interrupt address            */
    IfxSrc_init(src, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT12_TIMER);       /* Initialize the service request   */
    IfxSrc_enable(src);                                                         /* Enable GPT12 interrupt           */

    /* Initialize the LED */
    IfxPort_setPinModeOutput(PWM_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(MOTOR_ENA, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinModeOutput(MOTOR_ENB, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
}

/* Function to start the GPT12 timer */
void runGpt12PWM(void)
{
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);                   /* Start the timer T3               */
}

uint64 return_Timer_time(void){

    uint32 currentCntVal = IfxGpt12_T3_getTimerValue(&MODULE_GPT120);
    finalCntVal = currentCntVal - g_previousCntVal + timer_cnt*TIMER_CYCLE;
    g_previousCntVal = currentCntVal;

    return finalCntVal;
}

void Clear_Timer_count(void){

    timer_cnt=0;
}
