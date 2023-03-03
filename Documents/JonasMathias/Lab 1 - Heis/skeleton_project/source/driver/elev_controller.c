#include <assert.h>
#include "elev_controller.h"
#include "elevio.h"
#include "buttons.h"
#include "elevator.h"

static States curr_state = NEUTRAL;

/**
 * @brief Updates the state. Initiating the state function. Uses enum States to access different cases.
 * 
 * @param state Chooses what the state is being updated to.
 */

void update_state(States state){
    switch (state)
    {
    case ASCENDING:
        ascending();
        curr_state = ASCENDING;
        break;
    case FLOOR_HIT_ASCENDING:
        floor_hit_ascending();
        curr_state = FLOOR_HIT_ASCENDING;
        break;

    case STOP_ASCENDING:
        stop_ascending();
        curr_state = STOP_ASCENDING;
        break;
    case NEUTRAL:
        neutral();
        curr_state = neutral;
        break;

    case DESCENDING:
        descending();
        curr_state = DESCENDING;
        break;

    case FLOOR_HIT_DESCENDING:
        floor_hit_descending();
        curr_state = FLOOR_HIT_DESCENDING;
        break;

    case STOP_DESCENDING:
        stop_descending();
        curr_state = STOP_DESCENDING;
        break;
    case STOP:
        stop();
        curr_state = STOP;
    
    default:
        break;
    }
}


/**
 * @brief Set the motor dir object
 * 
 * @param dir The direction of the elevator motor.
 */
void set_motor_dir(Elevator * e, MotorDirection dir){
    assert(e!=NULL);
    e->curr_motor_dir = dir;
}

/**
 * @brief If a button is pressed the button will illumiate. 
 * When task is done, the button light will stop.
 * 
 */

void elevio_buttonLamp(int floor, ButtonType button, int value);


void ascending(void){
    set_motor_dir(DIRN_UP);
}

void floor_hit_ascending(void);

void stop_ascending(void);

void neutral(void);

void descending(void);

void floor_hit_descending(void);

void stop_descending(void);

void stop(void);