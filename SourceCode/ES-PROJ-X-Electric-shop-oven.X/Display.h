#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h> 

// Consts:
// -------

enum ENDisplayControlBits
{
    RIGHT_DISPLAY_FLAG = 0x10
};

// Variables:
// ----------

static volatile uint8_t left_digit        = 0;
static volatile uint8_t right_digit       = 0;
static volatile _Bool is_display_enabled  = false;


// Utils:
// ------

inline void SetValueOnDisplay(uint8_t arg_left_dig, uint8_t arg_right_dig)
{
    left_digit  = arg_left_dig;
    right_digit = arg_right_dig;
}

static uint8_t Display7s(uint8_t v) 
{
    switch(v) 
    {        
        case 0:  return 0xEE;
        case 1:  return 0x28;
        case 2:  return 0xCD;
        case 3:  return 0x6D;
        case 4:  return 0x2B;
        case 5:  return 0x67;
        case 6:  return 0xE7;
        case 7:  return 0x2C;
        case 8:  return 0xEF;
        case 9:  return 0x6F;
        case 10: return 0xAF;
        case 11: return 0xE3;
        case 12: return 0xC6;
        case 13: return 0xE9;
        case 14: return 0xC7;
        case 15: return 0x87;
        default: return 0;
    }
}

inline void DisableDisplay(void)
{
    is_display_enabled = false;
}

inline void EnableDisplay(void)
{
    is_display_enabled = true;
}

// ------------------------------------------

// private:
inline void Private_ShowDisplay(void)
{
    volatile static _Bool refresh_flag = true;
          
    if(refresh_flag)
        PORTB = Display7s(left_digit);
    else
        PORTB = (uint8_t)(Display7s(right_digit) | RIGHT_DISPLAY_FLAG);
      
    refresh_flag = ~refresh_flag;
}

#endif	// !DISPLAY_H 

