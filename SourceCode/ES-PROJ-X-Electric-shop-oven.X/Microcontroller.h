
#ifndef MICROCONTROLLER_H
#define	MICROCONTROLLER_H

#include <xc.h> 
#include "Timers.h"
#include "Leds.h"
#include "Display.h"
#include "DisplayTimer.h"
#include "Other.h"

// private:
// --------

inline void Private_InitMicrocontrollerTimer0(void)
{
    OPTION_REG = 0b11010111;
    TMR0 = 13;                 
}

inline void Private_InitMicrocontrollerTimer1(void)
{
    T1CON = 0b00100101;
    TMR1H = TMR1L = 0;  
}


// Microcontroller config:
// -----------------------

static void InitPeripherals()   // Konfiguracja mikrokontrolera
{
    TRISA=0xFE;  // 0xFE = 11111110 - jedyne 0 odnosi sie do lampki  
    TRISB=0x00;
    
    Private_InitMicrocontrollerTimer0();
    Private_InitMicrocontrollerTimer1();
   
    // Interrupt setup
    INTCONbits.TMR0IE = 1;    // enable interrupts for timer 0
    PIE1bits.TMR1IE   = 1;    // enable interrupts for timer 1
    INTCONbits.PEIE   = 1;    // enable peripheral interrupts
    
    ei();     
}

void __interrupt() isr(void)
{
   // only process timer0-triggered interrupts
   if(INTCONbits.TMR0IE && INTCONbits.TMR0IF) 
   {
      Private_DebounceButton1();
      Private_DebounceButton2();
      Private_DebounceButton3();
      Private_DebounceButton4(); 
      
      DECREMENT_TIMER(soft_timer_1);
      
      DECREMENT_TIMER(soft_timer_2);
      
      DECREMENT_TIMER(soft_timer_3);
      
      
      // ------------
      // For loading:
      if(is_loading_anim_enabled)
      {
         Private_UpdateLoadingAnim();
      }
      // ----------
      
      
      // -------------------------------
      // For Switching/showing displays:
      if(is_display_enabled)
      {
         Private_ShowDisplay();
      }
      // ------------------------------
      
      
      INTCONbits.TMR0IF = 0; // clear this interrupt condition
      TMR0 = 13;
   }
   
   // only process timer1-triggered interrupts
   if(PIE1bits.TMR1IE && PIR1bits.TMR1IF)
   {
       // ------------------
       // For display timer:
       if(is_display_enabled && is_display_timer_enabled)
       {
           Private_DecrementDisplayTimer();
       }
       // ------------------
       
       PIR1bits.TMR1IF = 0; // clear this interrupt condition 
   }
}

#endif	// !MICROCONTROLLER_H 

