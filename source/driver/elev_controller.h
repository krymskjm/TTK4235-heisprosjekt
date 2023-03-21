#ifndef ELEV_CONTROLLER
#define ELEV_CONTROLLER

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "elevio.h"
#include "buttons.h"
// #include "elevator.h"
#include "orders.h"

// static States curr_state = NEUTRAL;
static int between_floors = -1;
static int new_state = NEUTRAL;
static MotorDirection curr_motor_dir;
static int curr_floor = -1;
// static int new_floor;


/**
 * @brief Set the motor dir object
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 * @param dir The direction of the elevator motor.
 */
void set_motor_dir(ElevatorState * e, MotorDirection dir);

/**
 * @brief Set elevator to neutral state and other start conditions. Called in beginning of the program.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void init_controller(ElevatorState * e);

/**
 * @brief The function run the state the elevator is in.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void update_state(ElevatorState * e);

/**
 * @brief This is the state function where the elevator is waiting to hit a floor while ascending.
 * When the elevator hits a floor, the e->curr_state will be updated to FLOOR_HIT_ASCENDING.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void ascending(ElevatorState * e);

/**
 * @brief This is the state function where the elevator hits a floor while ascending.
 * If the elevator is at the next ordered floor e->curr_state will be updated to STOP_ASCENDING.
 * If not, e->curr_state will be updated back to ASCENDING.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void floor_hit_ascending(ElevatorState * e);

/**
 * @brief This is the state function where the elevator has stopped during its ascending fase.
 * If the elevator has ordered floors above current floor, it will set the state to ASCENDING.
 * If not, the state will be set to NEUTRAL.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void stop_ascending(ElevatorState * e);

/**
 * @brief This is the state function where the elevator has no orders.
 * When the elevator gets orders it will go into either the ascending fase or 
 * descending fase based on the order compared to the current floor.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void neutral(ElevatorState * e);

/**
 * @brief This is the state function where the elevator is waiting to hit a floor while descending.
 * When the elevator hits a floor, the e->curr_state will be updated to FLOOR_HIT_DESCENDING.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void descending(ElevatorState * e);

/**
 * @brief This is the state function where the elevator hits a floor while descending.
 * If the elevator is at the next ordered floor e->curr_state will be updated to STOP_DESCENDING.
 * If not, e->curr_state will be updated back to DESCENDING.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void floor_hit_descending(ElevatorState * e);   

/**
 * @brief This is the state function where the elevator has stopped during its descending fase.
 * If the elevator has ordered floors above current floor, it will set the state to ASCENDING.
 * If not, the state will be set to NEUTRAL.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void stop_descending(ElevatorState * e);


/**
 * @brief The emergency stop button. When pressed, the elevator order queue will be cleared
 * and the elevator is not going to move. Also the stop button will enlighten.
 * 
 * @param e is the struckt containing all parameters like state, direction, floor, etc.
 */
void stop(ElevatorState * e);

#endif