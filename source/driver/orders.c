#include <stdio.h>
#include <stdlib.h>
#include "orders.h"


void init_queue() {
    queue.current_floor          = -1;
    queue.direction              = 0;
    queue.num_cab_floors         = 0;
    queue.num_call_down_floors   = 0;
    queue.num_call_up_floors     = 0;
}

void button_handler(ElevatorState* e){
    
    //////////Handle buttons -> elevator action/////////////
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(floor, b); // returnerer 1 ved knapptrykk 0 ellers
            elevio_buttonLamp(floor, b, btnPressed);
            if (btnPressed) {
                // verdi til b ved heispaneltrykk gir 2, ned gir 1 og opp gir 0
                printf("pressed button %d\n", b);
                // TODO: Add buttonpressed to queue
                switch (b)
                {
                case 0:     // opp_knapp_trykket
                    // TODO: Finn ut om knapp allerede i queue
                    add_call_up_floor(e, floor); //ascending
                    break;
                
                case 1:     // ned_knapp_trykket
                    // TODO: Finn )ut om knapp allerede i queue
                    add_call_down_floor(e, floor); //descendning
                    break;
                
                case 2:     // Heispanelknapp trykket
                    // TODO: Finn ut om knapp allerede i queue
                    add_cab_floor(e, floor);
                    break;
                
                default:
                    break;
                }
            }
        }
    }
    // DEBUG: print out cab queue
    printf("cab_queue, len(%d): ", e->num_cab_floors);
    for (int i = 0; i < e->num_cab_floors; i++){
        printf("%d ", e->cab_floors[i]);
    }
    printf("\n");
}

// Function to add a floor to the list of selected floors
void add_cab_floor(ElevatorState *state, int floor) {
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
void remove_cab_floor(ElevatorState *state, int floor) {
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
