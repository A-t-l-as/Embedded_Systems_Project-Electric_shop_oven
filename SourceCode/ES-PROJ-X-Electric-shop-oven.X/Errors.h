
#ifndef ERRORS_H
#define	ERRORS_H

#include <xc.h> 

// Enums:
// ------

enum ENErrorCodes
{
    NO_ERROR                            = 0,
    ERROR_FOR_STATE_DISABLED            = 1,
    ERROR_FOR_STATE_READY               = 2,
    ERROR_FOR_STATE_HEATING             = 3,
    ERROR_FOR_STATE_INSERT_CARTRIDGE    = 4,
    ERROR_FOR_STATE_INSERT_PRODUCTS     = 5,
    ERROR_FOR_STATE_BAKING              = 6,
    ERROR_FOR_STATE_GET_PRODUCTS        = 7,
    ERROR_FOR_STATE_PREPARE_TO_CLEANING = 8,
    ERROR_FOR_STATE_CLEANING            = 9
};

static uint8_t error_code = NO_ERROR;

// Error utils:
// ------------

static uint8_t GetErrorFromState(STATE_t state)
{
    switch(state)
    {
        case STATE_DISABLED:
            return ERROR_FOR_STATE_DISABLED;
        break;
        
        case STATE_READY:
            return ERROR_FOR_STATE_READY;
        break;
        
        case STATE_HEATING:
            return ERROR_FOR_STATE_HEATING;
        break;
            
        case STATE_INSERT_CARTRIDGE:
            return ERROR_FOR_STATE_INSERT_CARTRIDGE;
        break;
                
        case STATE_INSERT_PRODUCTS:
            return ERROR_FOR_STATE_INSERT_PRODUCTS;
        break;
                
        case STATE_BAKING:
            return ERROR_FOR_STATE_BAKING;
        break;
                
        case STATE_GET_PRODUCTS:
            return ERROR_FOR_STATE_GET_PRODUCTS;
        break;
                
        case STATE_PREPARE_TO_CLEANING:
            return ERROR_FOR_STATE_PREPARE_TO_CLEANING;
        break;
                
        case STATE_CLEANING:
            return ERROR_FOR_STATE_CLEANING;
        break;
                
        default:
            return NO_ERROR;
        break;
    }
}


inline void CheckThatFaultAppeared(void)
{
    if(IsFaultSimulationButtonPressed())
    {
        error_code = GetErrorFromState(current_state_name);

        ChangeMachineState(STATE_ERROR, StateErrorFn);
        
        action = ACTION_ENTRY;
    }
}

inline void ResetError(void)
{
    error_code = NO_ERROR;
}

#endif	// !ERRORS_H

