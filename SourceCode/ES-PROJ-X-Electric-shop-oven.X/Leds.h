#ifndef LEDS_H
#define	LEDS_H

#include <xc.h>

// Macros for leds:
// ----------------

#define ROT_R1(x)   (((x) >> 1) | ((x) << 7))
#define ROT_L1(x)	(((x) << 1) | ((x) >> 7))

#define NUMBER_OF_LEDS 8

// Enums for leds:
// ---------------

enum ENLedFlags
{
    FIRST_LED_ON = 0x01,
    ALL_LEDS_OFF = 0x00,
    ALL_LEDS_ON  = 0xFF
};

// Functions:
// ----------

inline void AllLedsOn(void)
{
    PORTB = ALL_LEDS_ON;
}

inline void AllLedsOff(void)
{
    PORTB = ALL_LEDS_OFF;
}

inline void LampOn(void)
{ 
    PORTAbits.RA0 = 1; 
}

inline void LampOff(void)  
{ 
    PORTAbits.RA0 = 0; 
}


#endif	// !LEDS_H 

