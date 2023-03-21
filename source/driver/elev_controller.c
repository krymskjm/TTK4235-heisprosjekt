#include "elev_controller.h"


// LOGG:
// 15:08 kommenterte ut FLOOR_HIT ASCENDING
// 16.03.2023 14:35 Implementerer kø trekker fra etasje fra køen i FLOOR_HIT_ASCENDING
// 17.03.2023 13:49 Oppdaterer queue dir medlemsvariabel i FSM

/**
 * @brief Set elevator to neutral state. Called in beginning
 * of the program.
 * 
 */
void init_controller(ElevatorState * e) {
    while (elevio_floorSensor() == -1) {
        set_motor_dir(e, DIRN_DOWN);
    }
    set_motor_dir(e, DIRN_STOP);
    if (elevio_floorSensor() != -1)
        e->last_floor = elevio_floorSensor();
    e->curr_dir = DIRN_STOP;
}

void update_state(ElevatorState * e){
    if (elevio_floorSensor() == -1) return;
    switch (e->curr_state)
    {
    case ASCENDING:
        ascending(e);
        break;
    case FLOOR_HIT_ASCENDING:
        floor_hit_ascending(e);
        break;
    case STOP_ASCENDING:
        stop_ascending(e);
        break;
    case NEUTRAL:
        neutral(e);
        break;
    case DESCENDING:
        descending(e);
        break;
    case FLOOR_HIT_DESCENDING:
        floor_hit_descending(e);
        break;
    case STOP_DESCENDING:
        stop_descending(e);
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
void set_motor_dir(ElevatorState * e, MotorDirection dir){
    // assert(e!=NULL);
    curr_motor_dir = dir;
    elevio_motorDirection(curr_motor_dir);
    e->curr_dir = dir;
}


/**
 * @brief This is the state function where the elevator is waiting to hit a floor.
 * When the elevator hits a floor, the e->curr_state will be updated to FLOOR_HIT_ASCENDING.
 * 
 * @param e 
 */
void ascending(ElevatorState * e){
    printf("ascending\n");
    e->curr_state = ASCENDING;
    int floor = elevio_floorSensor();
    if (floor == e->new_floor) {
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = FLOOR_HIT_ASCENDING;
    }
}

void floor_hit_ascending(ElevatorState * e){
    printf("floor_hit_ascending\n");
    e->curr_state = FLOOR_HIT_ASCENDING;
    if (elevio_floorSensor() >= e->new_floor){
        if (elevio_floorSensor() != -1)
            e->last_floor = elevio_floorSensor();
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = STOP_ASCENDING;
    } else if (elevio_floorSensor() < e->new_floor){
        set_motor_dir(e, DIRN_DOWN);
        e->curr_state = ASCENDING;
    }
}

void stop_ascending(ElevatorState * e){
    printf("stopAscending\n");
    e->curr_state = STOP_ASCENDING;

    if (elevio_floorSensor() < e->new_floor){
        set_motor_dir(e, DIRN_UP);
        e->curr_state = ASCENDING;
    } else {
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = NEUTRAL;
    }
}

void neutral(ElevatorState * e){
    printf("in neutral\n");
    e->curr_state = NEUTRAL;
    printf("newfloor: %d\n", e->new_floor);

    if (elevio_floorSensor() < e->new_floor){
        //printf("VI kom hit\n\n");
        set_motor_dir(e, DIRN_STOP);   //Boilerplate, men) mealy
        e->curr_state = STOP_ASCENDING;
        //printf("State: %d \n", e->curr_state);
    } else if (elevio_floorSensor() > e->new_floor){
        set_motor_dir(e, DIRN_STOP);   //Boilerplate, men mealy
        e->curr_state = STOP_DESCENDING;
    } else{
        e->curr_state = NEUTRAL;
    }
}

void stop_descending(ElevatorState * e){
    printf("stop descent\n");
    e->curr_state = STOP_DESCENDING;
    if (elevio_floorSensor() > e->new_floor){
        set_motor_dir(e, DIRN_DOWN);
        e->curr_state = DESCENDING;
    } else {
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = NEUTRAL;
    }
}

void descending(ElevatorState * e){
    printf("descending\n");
    e->curr_state = DESCENDING;
    if (elevio_floorSensor() > e->new_floor){
        set_motor_dir(e, DIRN_DOWN);
        e->curr_state = DESCENDING;
    } else {
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = FLOOR_HIT_DESCENDING;
    }
}

void floor_hit_descending(ElevatorState * e){
    printf("floor hot descending\n");
    e->curr_state = FLOOR_HIT_DESCENDING;
    if (elevio_floorSensor() != -1)
        e->last_floor = elevio_floorSensor();
    if (elevio_floorSensor() <= e->new_floor){
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = STOP_DESCENDING;
    } else if (elevio_floorSensor() > e->new_floor ){
        set_motor_dir(e, DIRN_DOWN);
        e->curr_state = DESCENDING;
    }
}



void stop(){
}
