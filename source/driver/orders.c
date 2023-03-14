#include <stdio.h>
#include <stdlib.h>
#include "orders.h"


// Function to add a floor to the list of selected floors
void add_selected_floor(ElevatorState *state, int floor) {
    state->cab_floors[state->num_cab_floors++] = floor;
}

// Function to add a floor to the list of call-up floors
void add_call_up_floor(ElevatorState *state, int floor) {
    state->call_up_floors[state->num_call_up_floors++] = floor;
}

// Function to add a floor to the list of call-down floors
void add_call_down_floor(ElevatorState *state, int floor) {
    state->call_down_floors[state->num_call_down_floors++] = floor;
}

// Function to remove a floor from the list of selected floors
void remove_selected_floor(ElevatorState *state, int floor) {
    for (int i = 0; i < state->num_cab_floors; i++) {
        if (state->cab_floors[i] == floor) {
            for (int j = i; j < state->num_cab_floors - 1; j++) {
                state->cab_floors[j] = state->cab_floors[j + 1];
            }
            state->num_cab_floors--;
            return;
        }
    }
}

// Function to remove a floor from the list of call-up floors
void remove_call_up_floor(ElevatorState *state, int floor) {
    for (int i = 0; i < state->num_call_up_floors; i++) {
        if (state->call_up_floors[i] == floor) {
            for (int j = i; j < state->num_call_up_floors - 1; j++) {
                state->call_up_floors[j] = state->call_up_floors[j + 1];
            }
            state->num_call_up_floors--;
            return;
        }
    }
}

// Function to remove a floor from the list of call-down floors
void remove_call_down_floor(ElevatorState *state, int floor) {
    for (int i = 0; i < state->num_call_down_floors; i++) {
        if (state->call_down_floors[i] == floor) {
            for (int j = i; j < state->num_call_down_floors - 1; j++) {
                state->call_down_floors[j] = state->call_down_floors[j + 1];
            }
            state->num_call_down_floors--;
            return;
        }
    }
}