#include "buttons.h"


void button_poller(ElevatorState * e){

    //////////Handle buttons -> elevator action/////////////
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(floor, b); // returnerer 1 ved knapptrykk 0 ellers
            //elevio_buttonLamp(floor, b, btnPressed);
            if (btnPressed) {
                // verdi til b ved heispaneltrykk gir 2, ned gir 1 og opp gir 0

                printf("pressed button %d\n", b);

                // Add buttonpressed to queue
                if ((b == 0 || b == 1) && (e->last_floor != floor)) {     // opp or ned knapp trykket
                    set_flag(e, floor, b, b);
                }
                else if (b == 2) {          // heispanel
                    if (e->last_floor < floor) {
                        set_flag(e, floor, b, UP);  // 0 = opp-retning i tabell
                    }
                    else if (e->last_floor > floor) {
                        set_flag(e, floor, b, DOWN);  // 1 = ned
                    }
                    else if (e->last_floor == floor) {
                        switch (e->curr_dir)
                        {
                        case DIRN_DOWN:
                            set_flag(e, floor, b, UP);
                            break;
                        
                        case DIRN_UP:
                            set_flag(e, floor, b, DOWN);
                        
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
}

void darken_buttons(ElevatorState * e, int floor) {
    elevio_buttonLamp(e->last_floor, UP, 0);
    elevio_buttonLamp(e->last_floor, DOWN, 0);
    elevio_buttonLamp(e->last_floor, CABINE, 0);
}

void stop_button_poller() {
    // printf("stop: %d\n", elevio_stopButton());
}