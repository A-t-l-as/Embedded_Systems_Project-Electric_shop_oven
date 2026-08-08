#ifndef BUTTONS_H
#define	BUTTONS_H

#include <xc.h> 

// Macros for buttons:
// -------------------

#define BUTTON_ON              0    // Na plytce Board 1 przyciski pracuje w odwróconej logice
#define BUTTON_OFF             1    // czyli: 0 gdy przycisk jest wcisniety, 1 gdy puszczony

#define CHECK_MSEC             5    // Read hardware every 5 msec
#define PRESS_MSEC             20   // Stable time before registering pressed

// Buttons:
// --------

volatile static _Bool bt1_pressed = false;
volatile static _Bool bt2_pressed = false;
volatile static _Bool bt3_pressed = false;
volatile static _Bool bt4_pressed = false;

// Check buttons without debouncing:
// ---------------------------------

    // private:
    // --------
    inline int Private_IsButton1PressedWithoutDebouncing()   
    { 
        return PORTAbits.RA1 == BUTTON_ON; 
    } 

    inline int Private_IsButton2PressedWithoutDebouncing(void)
    {
        return PORTAbits.RA2 == BUTTON_ON; 
    }

    inline int Private_IsButton3PressedWithoutDebouncing(void)
    {
        return PORTAbits.RA3 == BUTTON_ON; 
    }
    
    inline int Private_IsButton4PressedWithoutDebouncing(void)
    {
        return PORTAbits.RA4 == BUTTON_ON; 
    }
    // -------------------------------
    
// Debouncing for buttons:
// -----------------------

    // private:
    // --------
    static void Private_DebounceButton1()
    {
        static unsigned char counter = PRESS_MSEC/CHECK_MSEC;
    
        if(Private_IsButton1PressedWithoutDebouncing())
        {
            if(--counter == 0)
                bt1_pressed = true;
        }
        else
        {
            counter = PRESS_MSEC/CHECK_MSEC;
            bt1_pressed = false;
        }
    }

    static void Private_DebounceButton2()
    {
        static unsigned char counter = PRESS_MSEC/CHECK_MSEC;    
        if(Private_IsButton2PressedWithoutDebouncing())
        {
            if(--counter == 0)
                bt2_pressed = true;
        }
        else
        {
            counter = PRESS_MSEC/CHECK_MSEC;
            bt2_pressed = false;
        }
    }

    static void Private_DebounceButton3()
    {
        static unsigned char counter = PRESS_MSEC/CHECK_MSEC;
    
        if(Private_IsButton3PressedWithoutDebouncing())
        {
            if(--counter == 0)
                bt3_pressed = true;
        }
        else
        {
            counter = PRESS_MSEC/CHECK_MSEC;
            bt3_pressed = false;
        }
    }
    
    static void Private_DebounceButton4()
    {
        static unsigned char counter = PRESS_MSEC/CHECK_MSEC;
    
        if(Private_IsButton4PressedWithoutDebouncing())
        {
            if(--counter == 0)
                bt4_pressed = true;
        }
        else
        {
            counter = PRESS_MSEC/CHECK_MSEC;
            bt4_pressed = false;
        }
    }
    
    // -------------------------------
    

// Reset buttons:
// --------------

inline void ResetButton1(void)   
{ 
    bt1_pressed = false;
} 

inline void ResetButton2(void)   
{ 
    bt2_pressed = false;
} 

inline void ResetButton3(void)   
{ 
    bt3_pressed = false;
} 

inline void ResetButton4(void)   
{ 
    bt4_pressed = false;
} 

// Check buttons:
// --------------

inline int IsPowerButtonPressed()   
{ 
    return bt1_pressed == true;
} 

inline int IsUpperButtonPressed(void)
{
    return bt1_pressed == true;
}

inline int IsLowerButtonPressed(void)
{
    return bt2_pressed == true;
}

inline int IsOkButtonPressed()   
{ 
    return bt3_pressed == true;
} 

inline int IsFaultSimulationButtonPressed()  
{ 
    return bt4_pressed == true;
} 

// OkButtonPressed:
// ----------------

inline void ResetAllButtons(void)
{
    ResetButton1();
    ResetButton2();
    ResetButton3();
    ResetButton4();
}

#endif	// !BUTTONS_H 
