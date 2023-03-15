#ifndef ORDERS
#define ORDERS

#include <stdio.h>
#include "elevio.h"

#define MAX_FLOORS 4

// Initialize the elevator state
typedef struct {
    int current_floor;
    int direction; // 1 for up, -1 for down, 0 for stopped
    int cab_floors[MAX_FLOORS];
    int num_cab_floors;
    int call_up_floors[MAX_FLOORS];
    int num_call_up_floors;
    int call_down_floors[MAX_FLOORS];
    int num_call_down_floors;
} ElevatorState;

static ElevatorState queue;

void init_queue();

/**
 * @brief add floor to queue when button is pressed.
 * 
 * @param state 
 * @param floor 
 */
void button_handler(ElevatorState* e);


void add_cab_floor(ElevatorState *state, int floor);
void add_call_up_floor(ElevatorState *state, int floor);
void add_call_down_floor(ElevatorState *state, int floor);
void remove_cab_floor(ElevatorState *state, int floor);
void remove_call_up_floor(ElevatorState *state, int floor);
void remove_call_down_floor(ElevatorState *state, int floor);

#endif