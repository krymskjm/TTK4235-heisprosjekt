#ifndef ELEV_CONTROLLER
#define ELEV_CONTROLLER

#include <stdio.h>
#include <assert.h>
#include "elevio.h"
#include "buttons.h"
#include "elevator.h"
#include "orders.h"

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

static States curr_state = NEUTRAL;
static int between_floors = -1;
static int new_state = NEUTRAL;
static MotorDirection curr_motor_dir;
static int curr_floor;

void init();

void update_state();

void set_motor_dir(MotorDirection dir);

void illuminate_btn();

void ascending();

void floor_hit_ascending();

void stop_ascending();

void neutral();

void descending();

void floor_hit_descending();

void stop_descending();

void stop();

#endif