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
        stop(e);
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
    if (floor != -1) {
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = FLOOR_HIT_ASCENDING;
    }
}

void floor_hit_ascending(ElevatorState * e){
    printf("floor_hit_ascending, etg: %d \n", (elevio_floorSensor()+1));
    e->last_floor = elevio_floorSensor();
    e->curr_state = FLOOR_HIT_ASCENDING;
    if (is_flagged(e->last_floor, UP)){
        set_motor_dir(e, DIRN_STOP);
        remove_flag(e->last_floor, UP);
        e->curr_state = STOP_ASCENDING;

        if(elevio_floorSensor() != -1){
            elevio_floorIndicator(elevio_floorSensor());
        }
        door_open(e);
        

    } else{
        set_motor_dir(e, DIRN_UP);
        e->curr_state = ASCENDING;
    }
    // else {
    //     printf("WROOONG floor hit ascending\n");elevio_floorSensor
}

void stop_ascending(ElevatorState * e){
    printf("stopAscending, etg: %d \n", (elevio_floorSensor()+1));
    e->curr_state = STOP_ASCENDING;

    check_obstructed(e);
    if(e->obstructed){
        return;
    }
    else if (order_above_curr_floor(e)){
        set_motor_dir(e, DIRN_UP);
        e->curr_state = ASCENDING;
    }
    else {
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = NEUTRAL;
    }
}

void neutral(ElevatorState * e){
    printf("in neutral\n");
    e->curr_state = NEUTRAL;

    if (order_above_curr_floor(e)){       // newfloor above
        //printf("VI kom hit\n\n");
        //set_motor_dir(e, DIRN_STOP);   //Boilerplate, men) mealy
        e->curr_state = STOP_ASCENDING;
        //printf("State: %d \n", e->curr_state);
    } else if (order_below_curr_floor(e)){
        //set_motor_dir(e, DIRN_STOP);   //Boilerplate, men mealy
        e->curr_state = STOP_DESCENDING;
    } else{
        e->curr_state = NEUTRAL;
    }
}

void stop_descending(ElevatorState * e){
    printf("stop descent, etg: %d \n", (elevio_floorSensor()+1));
    e->curr_state = STOP_DESCENDING;
    
    check_obstructed(e);
    if(e->obstructed){
        return;
    }
    else if (order_below_curr_floor(e)){
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
    int floor = elevio_floorSensor();
    
    if (floor != -1){
        set_motor_dir(e, DIRN_STOP);
        e->curr_state = FLOOR_HIT_DESCENDING;
    }
}

void floor_hit_descending(ElevatorState * e){
    printf("floor hot descending, etg: %d \n", (elevio_floorSensor()+1));
    e->last_floor = elevio_floorSensor();
    e->curr_state = FLOOR_HIT_DESCENDING;
    if (is_flagged(e->last_floor, DOWN)){
        set_motor_dir(e, DIRN_STOP);
        remove_flag(e->last_floor, DOWN);
        e->curr_state = STOP_DESCENDING;
        
        if(elevio_floorSensor() != -1){
        elevio_floorIndicator(elevio_floorSensor());
        }
        door_open(e);

    } else /*if (order_above_curr_floor(e))*/{
        set_motor_dir(e, DIRN_DOWN);
        e->curr_state = DESCENDING;
    }
    // else {
    //     printf("WROOONG floor hit descending\n");
    // }
}



void stop(ElevatorState * e){
    printf("STOPP EN HALV!! \n");
    elevio_stopLamp(1);
    set_motor_dir(e, DIRN_STOP);
    init_order_table();
    while (elevio_stopButton())
    {
    }
    elevio_stopLamp(0);
    e->curr_state = 1;
}


void check_obstructed(ElevatorState * e){
    if (elevio_obstruction()){
        e->obstructed = 1;
        printf("Venligst ikke stå i døra... \n");
        nanosleep(&(struct timespec){3, 0}, NULL);
    } else if (e->obstructed == 1 && !elevio_obstruction()){
        printf("Takk for at du flyttet deg. \n");
        e->obstructed =0;
        nanosleep(&(struct timespec){3, 0}, NULL);
        elevio_doorOpenLamp(0);
    } else{
        elevio_doorOpenLamp(0);
    }


}


void door_open(ElevatorState * e){
    elevio_doorOpenLamp(1);
    printf("....................\nDOOR OPEN AT FLOOR ");    
    printf("%d\n....................\n", (elevio_floorSensor()+1));
    nanosleep(&(struct timespec){3, 0}, NULL);

}