#ifndef ORDERS
#define ORDERS

#include <stdio.h>

typedef struct {
    int order;
    int new_floor;
} Orders;
// determines the path of the elevator during runtime
int * elevator_path[];


// Queue manipulators
void add_new_floor_to_queue(int new_floor);
void del_floor_from_queue(int floor);
void sort_queue();

// UML-class diagram functions
void update_queue();
void update_newfloor();

#endif