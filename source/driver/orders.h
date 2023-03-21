#ifndef ORDERS
#define ORDERS

#include <stdio.h>
#include <assert.h>
#include "elevio.h"
// #include "elev_controller.h"     //DEBUG: including bugs in elev_controller

#define MAX_FLOORS 4

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

// Initialize the elevator state
typedef struct {
    States curr_state;
    int current_floor;
    int new_floor;
    MotorDirection direction; // 1 for up, -1 for down, 0 for stopped
    int cab_floors[MAX_FLOORS];
    int num_cab_floors;
    int call_up_floors[MAX_FLOORS];
    int num_call_up_floors;
    int call_down_floors[MAX_FLOORS];
    int num_call_down_floors;
} ElevatorState;


void remove_hitfloor_from_cab_queue(ElevatorState * e);


void init_queue(ElevatorState * e);

/**
 * @brief add floor to queue when button is pressed.
 * 
 * @param state 
 * @param floor 
 */
void button_handler(ElevatorState* e);

/**
 * @brief update newfloor of e when elevator in ascending state
 * 
 * @param e 
 */
void update_new_floor_ascending(ElevatorState * e);

void add_cab_floor(ElevatorState *state, int floor);
void add_call_up_floor(ElevatorState *state, int floor);
void add_call_down_floor(ElevatorState *state, int floor);
void remove_cab_floor(ElevatorState *state, int floor);
void remove_call_up_floor(ElevatorState *state, int floor);
void remove_call_down_floor(ElevatorState *state, int floor);

#endif