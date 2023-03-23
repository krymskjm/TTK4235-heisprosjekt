#ifndef ORDERS
#define ORDERS

#include <stdio.h>
#include <assert.h>
#include "elevio.h"
// #include "elev_controller.h"     //DEBUG: including bugs in elev_controller

#define MAX_FLOORS 4
#define COLUMNS    2
#define UP         0
#define DOWN       1

typedef enum{
    NEUTRAL = 1,
    STOP_ASCENDING,
    ASCENDING,
    FLOOR_HIT_ASCENDING,
    STOP_DESCENDING,
    DESCENDING,
    FLOOR_HIT_DESCENDING,
    STOP
} States;

typedef struct {
    int flagged;    // boolean 0 if not flagged
} Order_table;


////////////OrderTable//////////////
void init_order_table();
void set_flag(int floor, int col);
void remove_flag(int floor, int col);
// dir: 0 = UP, 1 = DOWN
int is_flagged(int floor, int dir);


// Huffmann-table

// Initialize the elevator state
typedef struct {
    States curr_state;
    int last_floor;
    int new_floor;
    MotorDirection curr_dir;
    int cab_floors[MAX_FLOORS];
    int num_cab_floors;
    int call_up_floors[MAX_FLOORS];
    int num_call_up_floors;
    int call_down_floors[MAX_FLOORS];
    int num_call_down_floors;
    int obstructed;
} ElevatorState;


///////ELEVATOR_METHODS///////////
void init_queue(ElevatorState * e);
// return bool 1 if order is above e->curr_floor.
int order_above_curr_floor(ElevatorState * e);
int order_below_curr_floor(ElevatorState * e);

/////////////DEBUG////////////
void print_table();


#endif