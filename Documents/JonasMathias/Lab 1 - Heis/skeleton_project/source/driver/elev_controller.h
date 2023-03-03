#ifndef ELEV_CONTROLLER
#define ELEV_CONTROLLER

#include <stdio.h>


typedef enum{
    ASCENDING = 0,
    FLOOR_HIT_ASCENDING,
    STOP_ASCENDING,
    NEUTRAL,
    DESCENDING,
    FLOOR_HIT_DESCENDING,
    STOP_DESCENDING,
    STOP
} States;


void update_state(States state);

void set_motor_dir(Elevator * e, MotorDirection dir);

void update_state();

void illuminate_btn();

void ascending(void);

void floor_hit_ascending(void);

void stop_ascending(void);

void neutral(void);

void descending(void);

void floor_hit_descending(void);

void stop_descending(void);

void stop();

#endif