#ifndef TIMERS_H
#define	TIMERS_H

#include <xc.h> 
#include "Programs.h"
#include "Other.h"

// Macros:
// -------

#define DECREMENT_TIMER(some_timer) \
    if(some_timer) --some_timer;


// Consts:
// -------

#define PREPARE_TO_CLEANING_TIME           40
#define PERIOD_FOR_ROTATIONS               3

#define LOADING_TIME                       40   

#define TIME_OUT                           60

#define PERIOD_FOR_BELL                    5


// Soft timers:
// ------------

volatile static uint8_t soft_timer_1;
volatile static uint8_t soft_timer_2; 
volatile static uint8_t soft_timer_3;

// Timers setters:
// ---------------

inline void SetSoftTimer1ForPrepareToCleaning()
{
    soft_timer_1 = PREPARE_TO_CLEANING_TIME;  
}

inline void SetSoftTimer2ForRotations()
{
    soft_timer_2 = PERIOD_FOR_ROTATIONS;
}

inline void SetSoftTimer2ForBell()
{
    soft_timer_2 = PERIOD_FOR_BELL;
}

inline void SetSoftTimer3ForLoading()
{
    anim_tick_period = ANIM_TICK_PERIOD_FOR_LOADING;
    soft_timer_3     = LOADING_TIME;
    anim_step        = 0;
}

inline void SetSoftTimer3ForHeating(PROGRAM_t program)
{
    anim_tick_period = GetAnimTickPeriodForHeating(program);
    soft_timer_3     = GetHeatingTime(program);
    anim_step        = 0;
}

inline void SetSoftTimer3ForTimeOut(void)
{
    soft_timer_3     = TIME_OUT; 
}


#endif	// !TIMERS_H

