#include "orders.h"


Order_table order_table[MAX_FLOORS][COLUMNS];     // 2 columns: 0 = UP, 1 = DOWN

void init_order_table(ElevatorState * e) {
    for (int row = 0; row < MAX_FLOORS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            remove_flag(row, col);
            elevio_buttonLamp(row, UP, 0);
            elevio_buttonLamp(row, DOWN, 0);
            elevio_buttonLamp(row, CABINE, 0);
        }
    }
}

void set_flag(ElevatorState * e, int floor, int col, int dir) {
    // illegal orders
    if (floor < 0 || floor >= MAX_FLOORS) {
        printf("Illegal floor: %d\n", floor);
    }
    else if (col < 0 || col >= COLUMNS) {
        printf("Illegal column: %d\n", col);
    }
    if (!((e->curr_state == NEUTRAL) && (e->last_floor == elevio_floorSensor()))) {
        elevio_buttonLamp(floor, (ButtonType)col, 1);    // illuminate button.
    }

    order_table[floor][dir].flagged = 1;
}

// remove flagged order in OrderList and stop illuminate_button
void remove_flag(int floor, int col) {
    // illegal orders
    if (floor < 0 || floor >= MAX_FLOORS) {
        printf("Illegal floor: %d\n", floor);
    }
    else if (col < 0 || col >= COLUMNS) {
        printf("Illegal column: %d\n", col);
    }

    

    order_table[floor][col].flagged = 0;
}

int is_flagged(int floor, int dir) {
    // Illegal order
    if (floor < 0 || floor >= MAX_FLOORS) {
        printf("Illegal floor: %d\n", floor);
    }
    else if (dir < 0 || dir >= COLUMNS) {
        printf("Illegal column: %d\n", dir);
    }

    return order_table[floor][dir].flagged;
}




//////////ELEVATOR METHODS////////////
void init_queue(ElevatorState * e) {
    e->curr_state = NEUTRAL;
    e->is_moving = 0;
    e->after_stop_state = 0;
}

int order_above_curr_floor(ElevatorState * e) {
    for (int floor = e->last_floor; floor < MAX_FLOORS; floor++) {
        if ((is_flagged(floor, UP) || is_flagged(floor, DOWN))
            && floor != e->last_floor) {
            return 1;
        }
    }
    return 0;
}


int order_below_curr_floor(ElevatorState * e) {
    for (int floor = e->last_floor; floor >= 0; floor--) {
        if ((is_flagged(floor, DOWN) || is_flagged(floor, UP))
        && floor != e->last_floor) {
            return 1;
        }
    }
    return 0;
}


////////DEBUG/////////////
void print_table() {
    printf("\tUP\tDOWN\n");
    for (int row = MAX_FLOORS - 1; row >= 0; row--) {
        printf("floor %d: ", row);
        for (int col = 0; col < COLUMNS; col++) {
            printf("%d\t", is_flagged(row, col));
        }
        printf("\n");
    }
    printf("\n");
}