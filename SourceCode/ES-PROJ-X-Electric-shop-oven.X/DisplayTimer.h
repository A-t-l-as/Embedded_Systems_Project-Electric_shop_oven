
#ifndef DISPLAY_TIMER_H
#define	DISPLAY_TIMER_H

#include <xc.h> 

static volatile _Bool is_display_timer_enabled  = false;

inline void DisableDisplayTimer(void)
{
    is_display_timer_enabled = false;
}

inline void EnableDisplayTimer(void)
{
    is_display_timer_enabled = true;
}

inline int IsDisplayTimerFinished(void)
{
    return (left_digit == 0) && (right_digit == 0);
}

// ------------------------------------------

// private:
inline void Private_DecrementDisplayTimer(void)
{
    if(right_digit != 0)
    {
        --right_digit;
    }
    else
    {
        if(left_digit != 0)
        {
            --left_digit;
            right_digit = 9;
        }
    }
}

#endif	// !DISPLAY_TIMER_H 

