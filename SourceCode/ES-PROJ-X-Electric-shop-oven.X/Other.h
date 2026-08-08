
#ifndef OTHER_H
#define	OTHER_H

#include <xc.h> 
#include "Leds.h"

#define ANIM_TICK_PERIOD_FOR_LOADING       5   // co ile tickow przesuwa sie lampka

volatile static _Bool   is_loading_anim_enabled = false;
volatile static uint8_t anim_tick_period = ANIM_TICK_PERIOD_FOR_LOADING;
volatile static uint8_t anim_step = 0;
volatile static uint8_t anim_tick = 0;

// Bell abstraction:
// -----------------

// Czyli wszystkie ledy migaja
inline void RingTheBell(void) 
{ 
    PORTB = (uint8_t)~PORTB; 
} 

// Loading abstraction:
// --------------------

// Czyli ledy zapalaja sie po kolei
inline void Loading(void)
{
    static uint8_t last_step = ALL_LEDS_ON;  // wymusza rysowanie przy starcie
    
    if(anim_step != last_step)
    {
        last_step = anim_step;
        
        if(anim_step >= NUMBER_OF_LEDS)
        {
            // Caly pasek zapalony = gotowe
            AllLedsOn();
        }
        else
        {
            // Zapalanie od RB7 do RB0:
            // Dla anim_step = 0: 0xFF << (8 - 1 - 0) -> 0xFF << 7 -> 0b10000000
            // Dla anim_step = 1: 0xFF << (8 - 1 - 1) -> 0xFF << 6 -> 0b11000000
            PORTB = (uint8_t)(ALL_LEDS_ON << (NUMBER_OF_LEDS - 1 - anim_step));
            
            
            // Ta wersja przesuwala w lewo od RB0 do RB7:
            // PORTB = (uint8_t)((1 << (anim_step + 1)) - 1);
        }
    }

}

// Loading anim functions:
// -----------------------

inline void EnableLoadingAnim(void)
{
    is_loading_anim_enabled = true;
}

inline void DisableLoadingAnim(void)
{
    is_loading_anim_enabled = false;
}


// Cleaning abstractions:
// ----------------------

inline void MotorOn(void)  
{ 
    PORTBbits.RB1 = 1; 
}

inline void MotorOff(void) 
{ 
    PORTBbits.RB1 = 0; 
}

inline void PrepareToCleaning(void) 
{ 
    PORTB = (uint8_t)(ROT_L1(PORTB));
} 

// Heating abstractions:
// ---------------------

inline void HeaterOn(void)
{
    PORTBbits.RB7 = 1; 
}

inline void HeaterOff(void)
{
    PORTBbits.RB7 = 0; 
}

// Fan abstractions:
// -----------------

inline void FanOn(void)
{
    PORTBbits.RB6 = 1; 
}

inline void FanOff(void)
{
    PORTBbits.RB6 = 0; 
}

// Water valve abstraction:
// ------------------------

inline void OpenWaterValve(void)
{
    PORTBbits.RB4 = 1; 
}

inline void CloseWaterValve(void)
{
    PORTBbits.RB4 = 0; 
}

// Drain valve abstraction:
// ------------------------

inline void OpenDrainValve(void)
{
    PORTBbits.RB5 = 1; 
}

inline void CloseDrainValve(void)
{
    PORTBbits.RB5 = 0; 
}

// --------------------------------------------------

// private:

inline void Private_UpdateLoadingAnim(void)
{
    if(anim_tick < anim_tick_period)
    {
        ++anim_tick;
    }
    else
    {
        anim_tick = 0;
        
        if(anim_step < NUMBER_OF_LEDS)
            ++anim_step;   // flaga dla main()
    }
}

#endif	// !OTHER_H 

