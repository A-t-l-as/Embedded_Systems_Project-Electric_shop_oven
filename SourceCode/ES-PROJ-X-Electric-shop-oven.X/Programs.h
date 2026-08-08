#ifndef PROGRAMS_H
#define	PROGRAMS_H

#include <xc.h> 
#include "Buttons.h"

// Consts:

#define PROGRAM_SETTED      true
#define PROGRAM_NOT_SETTED  false

// Programs:
// ---------

typedef enum 
{
    PROGRAM_0_BUNS_FULL              = 0x01, // Bulki - caly piec
    PROGRAM_1_BUNS_HALF              = 0x02, // Bulki - pol pieca
    PROGRAM_2_COOKIES_FULL           = 0x04, // Ciastka - caly piec
    PROGRAM_3_COOKIES_HALF           = 0x08, // Ciastka - pol pieca
    PROGRAM_4_BREAD_2_TO_20          = 0x10, // Chleb od 2 do 20
    PROGRAM_5_CASSEROLE_OR_SWEET_BUN = 0x20, // Zapiekanka / drozdzowka
    PROGRAM_6_CLEANING_THE_MACHINE   = 0x40, // Mycie pieca
    PROGRAM_7_QUIT                   = 0x80  // Wyjscie
} PROGRAM_t;


// Initial program:
// ----------------

static PROGRAM_t program = PROGRAM_0_BUNS_FULL;     // okreslamy poczatkowy program


// Program times:
// --------------

#define PROGRAM_0_HEATING_TIME           NUMBER_OF_LEDS * 8
#define ANIM_TICK_PERIOD_FOR_PROGRAM_0   PROGRAM_0_HEATING_TIME/NUMBER_OF_LEDS

#define PROGRAM_1_HEATING_TIME           NUMBER_OF_LEDS * 7
#define ANIM_TICK_PERIOD_FOR_PROGRAM_1   PROGRAM_1_HEATING_TIME/NUMBER_OF_LEDS

#define PROGRAM_2_HEATING_TIME           NUMBER_OF_LEDS * 6
#define ANIM_TICK_PERIOD_FOR_PROGRAM_2   PROGRAM_2_HEATING_TIME/NUMBER_OF_LEDS

#define PROGRAM_3_HEATING_TIME           NUMBER_OF_LEDS * 9
#define ANIM_TICK_PERIOD_FOR_PROGRAM_3   PROGRAM_3_HEATING_TIME/NUMBER_OF_LEDS

#define PROGRAM_4_HEATING_TIME           NUMBER_OF_LEDS * 10
#define ANIM_TICK_PERIOD_FOR_PROGRAM_4   PROGRAM_4_HEATING_TIME/NUMBER_OF_LEDS

#define PROGRAM_5_HEATING_TIME           NUMBER_OF_LEDS * 8
#define ANIM_TICK_PERIOD_FOR_PROGRAM_5   PROGRAM_5_HEATING_TIME/NUMBER_OF_LEDS

// Programs utils:
// ---------------

static uint8_t GetHeatingTime(PROGRAM_t program)
{
    switch(program)
    {
        case PROGRAM_0_BUNS_FULL:
            return PROGRAM_0_HEATING_TIME;
        
        case PROGRAM_1_BUNS_HALF:
            return PROGRAM_1_HEATING_TIME;    
        
        case PROGRAM_2_COOKIES_FULL:
            return PROGRAM_2_HEATING_TIME;    
        
        case PROGRAM_3_COOKIES_HALF:
            return PROGRAM_3_HEATING_TIME;    
            
        case PROGRAM_4_BREAD_2_TO_20:
            return PROGRAM_4_HEATING_TIME;
           
        case PROGRAM_5_CASSEROLE_OR_SWEET_BUN:
            return PROGRAM_5_HEATING_TIME;   

        default:
            return 0;
    }
}



static uint8_t GetAnimTickPeriodForHeating(PROGRAM_t program)
{
    switch(program)
    {
        case PROGRAM_0_BUNS_FULL:
            return ANIM_TICK_PERIOD_FOR_PROGRAM_0;
        
        case PROGRAM_1_BUNS_HALF:
            return ANIM_TICK_PERIOD_FOR_PROGRAM_1;    
        
        case PROGRAM_2_COOKIES_FULL:
            return ANIM_TICK_PERIOD_FOR_PROGRAM_2;    
        
        case PROGRAM_3_COOKIES_HALF:
            return ANIM_TICK_PERIOD_FOR_PROGRAM_3;    
            
        case PROGRAM_4_BREAD_2_TO_20:
            return ANIM_TICK_PERIOD_FOR_PROGRAM_4;
           
        case PROGRAM_5_CASSEROLE_OR_SWEET_BUN:
            return ANIM_TICK_PERIOD_FOR_PROGRAM_5;   

        default:
            return 0;
    }
}

inline void SelectProgram0(void)
{
    PORTBbits.RB0 = 1; 
    program = PROGRAM_0_BUNS_FULL;
}

//
// Gdy ktorys z przyciskow zostal wcisniety podczas ustawiania programu
// czyli RA1 lub RA2 lub RA3 to zwracana jest wartosc true (PROGRAM_SETTED)
// oznaczajacy ze ustawiono jakis program przez co mozna potem go odebrac
// i np zresetowac timer lub zresetowac przyciski z innego miejsca w programie
// (mozna to zauwazyc w main())
//
// Jezeli nie wcisnieto przycisku to zwraca false (PROGRAM_NOT_SETTED)
//
inline _Bool SetProgram(void)
{
    if(IsUpperButtonPressed())
    {
        PORTB   = (uint8_t)(ROT_R1(PORTB));
        program = (uint8_t)(ROT_R1(program));
        return PROGRAM_SETTED;
    }
    else if(IsLowerButtonPressed())
    {
        PORTB   = (uint8_t)(ROT_L1(PORTB));
        program = (uint8_t)(ROT_L1(program));
        return PROGRAM_SETTED;
    }
    //
    // Dopiero jak user wcisnie przycisk OK czyli RA3
    // to wtedy program jest zatwierdzany i przechodzimy
    // do ACTION_EXIT
    //
    else if(IsOkButtonPressed())
    { 
        action = ACTION_EXIT;
        return PROGRAM_SETTED;
    }  
    
    return PROGRAM_NOT_SETTED;
}


#endif	// !PROGRAMS_H 

