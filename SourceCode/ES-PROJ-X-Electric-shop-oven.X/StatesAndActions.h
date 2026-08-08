#ifndef STATES_AND_ACTIONS_H
#define	STATES_AND_ACTIONS_H

#include <xc.h> 

// State machine - state names:
// ----------------------------

typedef enum 
{
    STATE_POWERUP,
    STATE_DISABLED,
    STATE_READY,
    STATE_HEATING,
    STATE_INSERT_CARTRIDGE,
    STATE_INSERT_PRODUCTS,
    STATE_BAKING,
    STATE_GET_PRODUCTS,
    STATE_PREPARE_TO_CLEANING,
    STATE_CLEANING,
    STATE_ERROR
} STATE_t;

// State machine - state function type:
// ------------------------------------

typedef void (*pStateFn)(void);

// State machine - state functions declarations:
// ---------------------------------------------

void StatePowerUpFn(void);
void StateDisabledFn(void);
void StateReadyFn(void);
void StateHeatingFn(void);
void StateInsertCartridgeFn(void);
void StateInsertProductsFn(void);
void StateBakingFn(void);
void StateGetProductsFn(void);
void StatePrepareToCleaningFn(void);
void StateCleaningFn(void);
void StateErrorFn(void);


// State machine - state actions:
// ------------------------------

typedef enum 
{
    ACTION_ENTRY,         // akcja wykonywana 1x podczas wejscia do stanu
    ACTION_DO,            //            
    ACTION_EXIT,          // akcja wykonywana 1x podczas wyjscia ze stanu
} ACTION_t;


// Initial state:
// --------------

static STATE_t  current_state_name     = STATE_POWERUP;   // okreslamy stan poczatkowy
static pStateFn CurrentStateFunction   = StatePowerUpFn;  //

// Initial action:
// ---------------

static ACTION_t action = ACTION_ENTRY;    // okreslamy akcje poczatkowa

// Change state:
// -------------

inline void ChangeMachineState(STATE_t state_name, pStateFn state_function)
{
    current_state_name     = state_name;
    CurrentStateFunction = state_function;
}

#endif	// !STATES_AND_ACTIONS_H 

