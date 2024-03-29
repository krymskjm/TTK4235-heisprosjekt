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
    if (elevio_floorSensor() != -1) {
        e->last_floor = elevio_floorSensor();
        elevio_floorIndicator(e->last_floor);
    }
    e->curr_dir = DIRN_STOP;
}

void update_state(ElevatorState * e){
    if (elevio_stopButton()) {
        e->curr_state = STOP;
    }
    else if (elevio_floorSensor() == -1 && !e->after_stop_state) {
        e->after_stop_state = 0;
        return;
    }
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
    e->is_moving = 1;
    int floor = elevio_floorSensor();
    if (floor != -1) {
        //set_motor_dir(e, DIRN_STOP);
        e->curr_state = FLOOR_HIT_ASCENDING;
    }
    
}

void floor_hit_ascending(ElevatorState * e){
    printf("floor_hit_ascending\n");
    e->curr_state = FLOOR_HIT_ASCENDING;
    e->last_floor = elevio_floorSensor();
    if (e->last_floor < 0) {
        e->curr_state = NEUTRAL;
        return;
    }

    elevio_floorIndicator(e->last_floor);   // Illuminate floor

    //e->is_moving = 0;
    if (is_flagged(e->last_floor, UP)){
        // printf("1\n");

        remove_flag(e->last_floor, UP);
        remove_flag(e->last_floor, DOWN);

        // deluminate button(s)
        darken_buttons(e, e->last_floor);

        set_motor_dir(e, DIRN_STOP);
        open_doors(e);

        e->curr_state = STOP_ASCENDING;
    // } else if ((e->last_floor == 3 && (is_flagged(e->last_floor, DOWN)))) {   // edge case: floor=3, down-btn pressed
    //     printf("2\n");
    //     remove_flag(e->last_floor, DOWN);
    //     // deluminate button(s)
    //     darken_buttons(e, e->last_floor);

    //     set_motor_dir(e, DIRN_STOP);
    //     open_doors(e);

    //     e->curr_state = STOP_DESCENDING;
    } else if (order_above_curr_floor(e)){      // POTENTIAL BUG: order_above_curr_floor is very slow.
        printf("3\n");
        set_motor_dir(e, DIRN_UP);
        // nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
        e->curr_state = ASCENDING;
    }
    else {
        printf("WROOONG floor hit ascending, floor %d\n", e->last_floor);
        if (is_flagged(e->last_floor, DOWN)) {
            remove_flag(e->last_floor, DOWN);
            set_motor_dir(e, DIRN_STOP);
            // deluminate button(s)
            darken_buttons(e, e->last_floor);
            open_doors(e);
            e->curr_state = STOP_ASCENDING;
        }
    }
}

void stop_ascending(ElevatorState * e){
    printf("stopAscending\n");
    e->curr_state = STOP_ASCENDING;
    e->is_moving = 0;

    if (order_above_curr_floor(e)){
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
    e->is_moving = 0;

    if (e->last_floor < 0) {
        init_controller(e);
        return;
    }

    // edge case: current floor (last_floor) is not deleted from queue
    if (is_flagged(e->last_floor, UP) || is_flagged(e->last_floor, DOWN)) {
        remove_flag(e->last_floor, UP);
        remove_flag(e->last_floor, DOWN);
        // deluminate button(s)
        darken_buttons(e, e->last_floor);

        //open_doors(e);
    }

    if (order_above_curr_floor(e)){       // newfloor above
        //printf("VI kom hit\n\n");
        set_motor_dir(e, DIRN_STOP);   //Boilerplate, men mealy
        e->curr_state = STOP_ASCENDING;
        //printf("State: %d \n", e->curr_state);
    } else if (order_below_curr_floor(e)){
        set_motor_dir(e, DIRN_STOP);   //Boilerplate, men mealy
        e->curr_state = STOP_DESCENDING;
    } else{
        e->curr_state = NEUTRAL;
    }
}

void stop_descending(ElevatorState * e){
    printf("stop descent\n");
    e->curr_state = STOP_DESCENDING;
    e->is_moving = 0;
    if (order_below_curr_floor(e)){
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
    e->is_moving = 1;
    int floor = elevio_floorSensor();
    if (floor != -1){
        //set_motor_dir(e, DIRN_STOP);
        e->curr_state = FLOOR_HIT_DESCENDING;
    }
}

void floor_hit_descending(ElevatorState * e){
    printf("floor hit descending\n");
    e->curr_state = FLOOR_HIT_DESCENDING;
    e->last_floor = elevio_floorSensor();
    //printf("lastfloor: %d\n", e->last_floor);
    if (e->last_floor < 0) {
        e->curr_state = NEUTRAL;
        return;
    }
    
    elevio_floorIndicator(e->last_floor);       // Illuminate floor indicator
    //e->is_moving = 0;
    if (is_flagged(e->last_floor, DOWN) || (e->last_floor == 0 && is_flagged(0, UP))){
        remove_flag(e->last_floor, DOWN);
        remove_flag(e->last_floor, UP);
        set_motor_dir(e, DIRN_STOP);
        // deluminate button(s)
        darken_buttons(e, e->last_floor);
        open_doors(e);

        e->curr_state = STOP_DESCENDING;
    // } else if ((/*e->last_floor == 0 &&*/ (is_flagged(e->last_floor, UP)))) {   // edge case: floor=0, up-btn pressed
    //     remove_flag(e->last_floor, UP);
    //     // deluminate button(s)
    //     darken_buttons(e, e->last_floor);
    //     set_motor_dir(e, DIRN_STOP);
    //     open_doors(e);

    //     e->curr_state = STOP_DESCENDING;
    } else if (order_below_curr_floor(e)){
        set_motor_dir(e, DIRN_DOWN);
        e->curr_state = DESCENDING;
    }
    else {
        printf("WROOONG floor hit descending floor %d\n", e->last_floor);
        if (is_flagged(e->last_floor, UP)) {
            remove_flag(e->last_floor, UP);
            set_motor_dir(e, DIRN_STOP);
            // deluminate button(s)
            darken_buttons(e, e->last_floor);
            open_doors(e);
            e->curr_state = STOP_DESCENDING;
        }
    }
}


void stop(ElevatorState * e){
    printf("stop\n");
    set_motor_dir(e, DIRN_STOP);    // S4
    init_order_table(e);             // S5


    // TODO: L6, D3, S4-S7
    // L6: light stopbutton when pressed
    while (elevio_stopButton()) {
        elevio_stopLamp(1);     // turn on stopbutton when pressed

        // D3: if in floor open doors for 3 sec
        if (elevio_floorSensor() != -1) {     // returns -1 if between floors
            open_doors(e);
        }
        else {
            nanosleep(&(struct timespec){0, 200*1000*1000}, NULL);
        }
        
    }
    elevio_stopLamp(0);     // turn off stopbutton
    // TODO: transition to NEUTRAL
    e->curr_state = NEUTRAL;
    e->after_stop_state = 1;
}
