#include "elev_controller.h"

// KOMMENTERINGS LOGG:
// 15:08 kommenterte ut FLOOR_HIT ASCENDING

/**
 * @brief Set elevator to neutral state. Called in beginning
 * of the program.
 * 
 */
void init_controller() {
    while (elevio_floorSensor() == -1) {
        set_motor_dir(DIRN_DOWN);
    }
    set_motor_dir(DIRN_STOP);
}

void update_new_floor(int floor) {
    new_floor = floor;
}

void update_state(){
    if (elevio_floorSensor() == -1) return;
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
 * @brief This is the state function where the elevator is waiting to hit a floor.
 * When the elevator hits a floor, the new_state will be updated to FLOOR_HIT_ASCENDING.
 * 
 * @param e 
 */
void ascending(){
    printf("ascending\n");
    if (elevio_floorSensor() == new_floor) {
        // new_state = FLOOR_HIT_ASCENDING;
        set_motor_dir(DIRN_STOP);
        new_state = STOP_ASCENDING;
    }
}

void floor_hit_ascending(){
    // TODO: UPDATE QUEUE HERE?

    printf("floor_hit_ascending\n");
    if (elevio_floorSensor() <= new_floor){
        set_motor_dir(DIRN_STOP);
        new_state = STOP_ASCENDING;
        // TODO: OR HERE?
    } else {
        set_motor_dir(DIRN_UP);
        new_state = ASCENDING;
    }
}

void stop_ascending(){
    printf("stopAscending\n");
    if (elevio_floorSensor() < new_floor){
        set_motor_dir(DIRN_UP);
        new_state = ASCENDING;
    } else {
        set_motor_dir(DIRN_STOP);
        new_state = NEUTRAL;
    }
}

void neutral(){
    printf("in neutral\n");
    // already in updatestate()
    // if (elevio_floorSensor() == -1) {
    //     // printf("between floors\n");
    //     return; // ingen transisjon når heis mellom etasjer
    // }

    if (elevio_floorSensor() < new_floor){
        //printf("VI kom hit\n\n");
        set_motor_dir(DIRN_STOP);   //Boilerplate, men mealy
        new_state = STOP_ASCENDING;
        //printf("State: %d \n", new_state);
    } else if (elevio_floorSensor() > new_floor){
        set_motor_dir(DIRN_STOP);   //Boilerplate, men mealy
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
    printf("descending\n");
    if (elevio_floorSensor() > new_floor){
        set_motor_dir(DIRN_DOWN);
        new_state = DESCENDING;
    } else {
        set_motor_dir(DIRN_STOP);
        new_state = NEUTRAL;
    }
}

void floor_hit_descending(){
    if (elevio_floorSensor() == new_floor){
        set_motor_dir(DIRN_STOP);
        new_state = STOP_DESCENDING;
    } else if (elevio_floorSensor() > new_floor ){
        set_motor_dir(DIRN_DOWN);
        new_state = DESCENDING;
    }
}



void stop(){
}