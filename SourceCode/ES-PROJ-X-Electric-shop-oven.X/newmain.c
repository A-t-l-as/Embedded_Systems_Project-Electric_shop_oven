// Config:
// -------
#pragma config FOSC   = EXTRCCLK  // Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, Resistor and Capacitor on RA7/OSC1/CLKIN)
#pragma config WDTE   = OFF   // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE  = OFF  // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE  = ON   // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN  = ON   // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP= ON // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD= OFF// Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)

// Includes:
// ---------
#include <xc.h>
#include <stdbool.h>   
#include "Leds.h"
#include "StatesAndActions.h"
#include "Microcontroller.h"
#include "Programs.h"
#include "Errors.h"

// Consts and macros:
// ------------------

#define _XTAL_FREQ 1000000UL


// State machine - state functions def:
// ------------------------------------

void StatePowerUpFn(void)
{
	ChangeMachineState(STATE_DISABLED, StateDisabledFn); // Przejscie do stanu zablokowania

	action = ACTION_ENTRY;
}

void StateDisabledFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			LampOff();
			AllLedsOff(); 
			EnableLoadingAnim();
			action = ACTION_DO;
		break;
 
		case ACTION_DO:
			//
			// PowerButton to RA1
			// W celu wlaczenia pieca nalezy przytrzymac RA1
			// 
			if (IsPowerButtonPressed())
			{
				Loading();
				if(soft_timer_3 == 0)
				{ 
					action = ACTION_EXIT;
				}
			}
			else
			{
				AllLedsOff();
				SetSoftTimer3ForLoading(); 
			}

			CheckThatFaultAppeared();

		break;

		case ACTION_EXIT:
			DisableLoadingAnim();

			ChangeMachineState(STATE_READY, StateReadyFn);

			action = ACTION_ENTRY;
		break;
	}
}

void StateReadyFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			AllLedsOff();
			SelectProgram0();
			LampOn();
			action = ACTION_DO;

			SetSoftTimer3ForTimeOut();
		break;

		case ACTION_DO: 
			//
			// RA1 przelacza na program wyzej
			// natomiast RA2 przelacza na program nizej
			// RA3 zatwierdza program
			// 
			//
			if(SetProgram() == PROGRAM_SETTED)
			{
				ResetAllButtons();
				SetSoftTimer3ForTimeOut();
			}

			if(soft_timer_3 == 0)
			{
				ChangeMachineState(STATE_DISABLED, StateDisabledFn);

				action = ACTION_ENTRY;  
			}

			CheckThatFaultAppeared();

		break;

		case ACTION_EXIT:
			//
			// Program 7 czyli led RB7 to jest wyjscie
			// czyli powrot do stanu disabled
			//
			if(program == PROGRAM_7_QUIT)
			{
				ChangeMachineState(STATE_DISABLED, StateDisabledFn);
 
				action = ACTION_ENTRY;
			}
			//
			// Program 6 czyli led RB6 to jest mycie pieca
			//
			else if(program == PROGRAM_6_CLEANING_THE_MACHINE)
			{
				ChangeMachineState(STATE_INSERT_CARTRIDGE, StateInsertCartridgeFn);

				action = ACTION_ENTRY;
			}
			else
			{
				ChangeMachineState(STATE_HEATING, StateHeatingFn); 

				action = ACTION_ENTRY;
			}
		break;
	}
}

void StateHeatingFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			action = ACTION_DO;
			EnableLoadingAnim();
			SetSoftTimer3ForHeating(program);
			FanOn();
			HeaterOn();
		break;

		case ACTION_DO:
			// 
			// Jak sie zaladuja lampki na full
			// czyli piec osiagnie odpowiednia temperature
			// to wtedy przechodzi do stanu wkladania
			// produktow do wypiekania
			//
			Loading();
			if(soft_timer_3 == 0)
			{ 
				action = ACTION_EXIT;
			}

			CheckThatFaultAppeared();

		break;

		case ACTION_EXIT: 
			DisableLoadingAnim();
			HeaterOff();
			FanOff();

			ChangeMachineState(STATE_INSERT_PRODUCTS, StateInsertProductsFn);

			action = ACTION_ENTRY;
		break;
	}
}

void StateInsertProductsFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			AllLedsOff();
			SetSoftTimer2ForBell();
			action = ACTION_DO;
		break;

		case ACTION_DO: 
			// 
			// W tym stanie lampki migaja w celu
			// zasymulowania dzwonka.
			// 
			// Wcisniecie przycisku RB3 czyli OK
			// zatwierdza ze produkty zostaly wlozone
			// i drzwiczki zostaly zamkniete
			// 
			if(IsOkButtonPressed())
			{
				ResetAllButtons();
				action = ACTION_EXIT;
			}  
			else if (soft_timer_2 == 0) 
			{
				RingTheBell();
				SetSoftTimer2ForBell();
			}   

			CheckThatFaultAppeared();
		break;

		case ACTION_EXIT:

			ChangeMachineState(STATE_BAKING, StateBakingFn);

			action = ACTION_ENTRY;
		break;
	}
}

void StateBakingFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			FanOn();
			HeaterOn();
			AllLedsOff();
			EnableDisplay();
			SetValueOnDisplay(5, 9);
			EnableDisplayTimer();
			action = ACTION_DO;
		break;

		case ACTION_DO: 
			//
			// Podczas pieczenia nalezy przelaczyc z ledow
			// na display
			//
			// Po zakonczeniu odliczania stan pieczenia przechodzi 
			// w stan odbioru 
			// 
			if (IsDisplayTimerFinished())
			{
				action = ACTION_EXIT;
			}

			CheckThatFaultAppeared();
		break;

		case ACTION_EXIT:
			FanOff();
			HeaterOff();
			DisableDisplayTimer();
			DisableDisplay();

			ChangeMachineState(STATE_GET_PRODUCTS, StateGetProductsFn);

			action = ACTION_ENTRY;
		break;
	}
}

void StateGetProductsFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			AllLedsOff();
			SetSoftTimer2ForBell();
			action = ACTION_DO;
		break;

		case ACTION_DO: 
			// 
			// W tym stanie lampki migaja w celu
			// zasymulowania dzwonka.
			// 
			// Wcisniecie przycisku RB3 czyli OK
			// zatwierdza ze produkty zostaly wyjete
			// i drzwiczki zostaly zamkniete
			// 
			if(IsOkButtonPressed())
			{
				ResetAllButtons();
				action = ACTION_EXIT;
			}  
			else if (soft_timer_2 == 0) 
			{
				RingTheBell();
				SetSoftTimer2ForBell();
			}   

			CheckThatFaultAppeared();
		break;

		case ACTION_EXIT:

			ChangeMachineState(STATE_READY, StateReadyFn);

			action = ACTION_ENTRY;
		break;
	}
}

void StateInsertCartridgeFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			AllLedsOff();
			SetSoftTimer2ForBell();
			action = ACTION_DO;
		break;

		case ACTION_DO: 
			// 
			// W tym stanie lampki migaja w celu
			// zasymulowania dzwonka.
			// 
			// Wcisniecie przycisku RB3 czyli OK
			// zatwierdza ze wklad do czyszczenia zostal wlozony
			// i drzwiczki zostaly zamkniete
			// 
			if(IsOkButtonPressed())
			{
				ResetAllButtons();
				action = ACTION_EXIT;
			}  
			else if (soft_timer_2 == 0) 
			{
				RingTheBell();
				SetSoftTimer2ForBell();
			}   

			CheckThatFaultAppeared();
		break;

		case ACTION_EXIT:

			ChangeMachineState(STATE_PREPARE_TO_CLEANING, StatePrepareToCleaningFn);

			action = ACTION_ENTRY;
		break;
	}
}

void StatePrepareToCleaningFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			OpenWaterValve();
			FanOn();
			AllLedsOff();
			MotorOn();
			SetSoftTimer1ForPrepareToCleaning(); 
  
			SetSoftTimer2ForRotations();

			action = ACTION_DO;
		break;
		
		case ACTION_DO: 
			//
			// Czekamy az piec przygotuje sie do mycia
			// czyli jak led przestanie krazyc...
			//
			if (soft_timer_1 == 0) // soft timer 1 for preparing cleaning
			{
				action = ACTION_EXIT;
			} 
			if (soft_timer_2 == 0) // soft timer 2 for rotations 
			{
				PrepareToCleaning();
   
				SetSoftTimer2ForRotations();
			}

			CheckThatFaultAppeared();
		break;
		
		case ACTION_EXIT:
			MotorOff();
			AllLedsOff();

			ChangeMachineState(STATE_CLEANING, StateCleaningFn); // Przejscie do wlasciwej fazy mycia

			action = ACTION_ENTRY;
		break;
	}
}

void StateCleaningFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			HeaterOn();
			AllLedsOff();
			EnableDisplay();
			SetValueOnDisplay(5, 9);
			EnableDisplayTimer();
			action = ACTION_DO;
		break;

		case ACTION_DO: 
			//
			// Podczas mycia nalezy przelaczyc z ledow
			// na display
			//
			// Po zakonczeniu odliczania stan mycia przechodzi 
			// w stan odbioru wkladu do mycia
			// 
			if (IsDisplayTimerFinished())
			{
				action = ACTION_EXIT;
			}

			CheckThatFaultAppeared();
		break;

		case ACTION_EXIT:
			CloseWaterValve();
			OpenDrainValve();
			HeaterOff();
			FanOff();
			DisableDisplayTimer();
			DisableDisplay();
			CloseDrainValve();

			ChangeMachineState(STATE_GET_PRODUCTS, StateGetProductsFn); // Zakladamy ze odbior wkladu i odbior gotowego produktu to jedno i to samo
			// w celu zaoszczedzenia pamieci programu
			action = ACTION_ENTRY;
		break;
	}
}

void StateErrorFn(void)
{
	switch(action) 
	{
		case ACTION_ENTRY:
			DisableDisplayTimer();
			CloseWaterValve();
			HeaterOff();
			FanOff();
			OpenDrainValve();
			AllLedsOff();
			EnableDisplay();
			SetValueOnDisplay(0, error_code);
			action = ACTION_DO;
		break;

		case ACTION_DO: 
			if(IsOkButtonPressed())
			{
				ResetAllButtons();
				action = ACTION_EXIT;
			}  
		break;

		case ACTION_EXIT:
			CloseDrainValve();
			DisableDisplay();
			ResetError();
  
			ChangeMachineState(STATE_DISABLED, StateDisabledFn); // Powrot do stanu zablokowanego

			action = ACTION_ENTRY;
		break;
	}
}

// Entry:
// ------

void main() 
{
	InitPeripherals(); // konfiguracja mikrokontrolera 

	while (1) 
	{ 
		CurrentStateFunction();
	} 
} 
