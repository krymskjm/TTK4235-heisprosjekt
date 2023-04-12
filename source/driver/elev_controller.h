#ifndef ELEV_CONTROLLER
#define ELEV_CONTROLLER

#include <stdio.h>
#include <assert.h>
#include "elevio.h"
#include "buttons.h"
// #include "elevator.h"
#include "orders.h"
#include "timer.h"
#include "doors.h"


// static States curr_state = NEUTRAL;
static int between_floors = -1;
static int new_state = NEUTRAL;
static MotorDirection curr_motor_dir;
static int curr_floor = -1;
// static int new_floor;


// Accessors and mutators
void set_motor_dir(ElevatorState * e, MotorDirection dir);

void init_controller(ElevatorState * e);

void illuminate_btn();


// Aksjoner og transisjoner i FSM
void update_state(ElevatorState * e);

void ascending(ElevatorState * e);
void floor_hit_ascending(ElevatorState * e);
void stop_ascending(ElevatorState * e);
void neutral(ElevatorState * e);
void descending(ElevatorState * e);
void floor_hit_descending(ElevatorState * e);    
void stop_descending(ElevatorState * e);
void stop();

#endif