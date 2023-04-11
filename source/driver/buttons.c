#include <stdio.h>
#include "buttons.h"
#include "elevio.h"


void button_poller(ElevatorState * e){

    //////////Handle buttons -> elevator action/////////////
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(floor, b); // returnerer 1 ved knapptrykk 0 ellers
            elevio_buttonLamp(floor, b, btnPressed);
            if (btnPressed) {
                // verdi til b ved heispaneltrykk gir 2, ned gir 1 og opp gir 0
                printf("pressed button %d\n", b);

                // TODO: Add buttonpressed to queue
                if ((b == 0 || b == 1) && (e->last_floor != floor)) {     // opp or ned knapp trykket
                    set_flag(floor, b);
                    //printf("flag: floor %d, pressed %d\n\n\n\n", floor, b);     //DEBUG: flag not set (flag is set)
                }
                else if (b == 2) {          // heispanel
                    if (e->last_floor < floor) {
                        set_flag(floor, UP);  // 0 = opp-retning i tabell
                    }
                    else if (e->last_floor > floor) {
                        set_flag(floor, DOWN);  // 1 = ned
                    }
                    else if (e->last_floor == floor) {
                        switch (e->curr_dir)
                        {
                        case DIRN_DOWN:
                            set_flag(floor, UP);
                            break;
                        
                        case DIRN_UP:
                            set_flag(floor, DOWN);
                        
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
}