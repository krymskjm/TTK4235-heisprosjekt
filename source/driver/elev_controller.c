#include "elev_controller.h"

static int new_floor = 3; // TODO: ENDRE DENNE FUNKSJONEN TIL VARIABELEN I order.h


/**
 * @brief Updates the state. Initiating the state function. Uses enum States to access different cases.
 * 
 * @param state Chooses what the state is being updated to.
 */

void init() {
    while (elevio_floorSensor() == -1) {
        set_motor_dir(DIRN_DOWN);
    }
    set_motor_dir(DIRN_STOP);
}

void update_state(){
    if (new_floor == -1) return;
    switch (new_state)
    {
    case ASCENDING:
        ascending();
        break;
    case FLOOR_HIT_ASCENDING:
        floor_hit_ascending();
        break;
    case STOP_ASCENDING:
        stop_ascending();
        break;
    case NEUTRAL:
        neutral();
        break;
    case DESCENDING:
        descending();
        break;
    case FLOOR_HIT_DESCENDING:
        floor_hit_descending();
        break;
    case STOP_DESCENDING:
        stop_descending();
        break;
    case STOP:
        stop();
    default:
        break;
    }
}

/*
 * @brief Set the motor dir object
 * 
 * @param dir The direction of the elevator motor.
 */
void set_motor_dir(MotorDirection dir){
    // assert(e!=NULL);
    curr_motor_dir = dir;
    elevio_motorDirection(curr_motor_dir);
}

/**
 * @brief If a button is pressed the button will illumiate. 
 * When task is done, the button light will stop.
 * 
 */

//void elevio_buttonLamp(int floor, struct ButtonType button, int value);

/**
 * @brief This is the state function where the elevator is waiting to hit a floor.
 * When the elevator hits a floor, the new_state will be updated to FLOOR_HIT_ASCENDING.
 * 
 * @param e 
 */
void ascending(){
    if (elevio_floorSensor() < new_floor){
        set_motor_dir(DIRN_UP);
        new_state = ASCENDING;
    } else {
        new_state = FLOOR_HIT_ASCENDING;
    }
}

void floor_hit_ascending(){
    if (elevio_floorSensor() == new_floor){
        set_motor_dir(DIRN_STOP);
        new_state = STOP_ASCENDING;
    } else{
        new_state = ASCENDING;
    }
}

void stop_ascending(){
    printf("stopAscending\n");
    if (elevio_floorSensor() < new_floor){
        printf("florrsensor < newfloor\n");
        set_motor_dir(DIRN_UP);
        new_state = ASCENDING;
    } else {
        set_motor_dir(DIRN_STOP);
        new_state = NEUTRAL;
    }
}

void neutral(){
    if (elevio_floorSensor() < new_floor){
        printf("VI kom hit\n\n");
        set_motor_dir(DIRN_UP);
        new_state = STOP_ASCENDING;
        printf("State: %d \n", new_state);
    } else if (elevio_floorSensor() > new_floor){
        set_motor_dir(DIRN_STOP);
        new_state = STOP_DESCENDING;
    } else{
        new_state = NEUTRAL;
    }
}

void stop_descending(){
    if (elevio_floorSensor() > new_floor){
        set_motor_dir(DIRN_DOWN);
        new_state = DESCENDING;
    } else {
        set_motor_dir(DIRN_STOP);
        new_state = NEUTRAL;
    }
}

void descending(){
    if (elevio_floorSensor() > new_floor){
        set_motor_dir(DIRN_DOWN);
        new_state = DESCENDING;
    } else {
        new_state = FLOOR_HIT_DESCENDING;
    }
}

void floor_hit_descending(){
    if (elevio_floorSensor() == new_floor){
        set_motor_dir(DIRN_STOP);
        new_state = STOP_DESCENDING;
    } else{
        new_state = DESCENDING;
    }
}



void stop(){
}