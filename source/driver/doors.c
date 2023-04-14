#include "doors.h"


void open_doors(ElevatorState * e) {
        time_t curr_ti;
        start_timer(&curr_ti);
        elevio_doorOpenLamp(1);       // turn on doorLamp
        while (!timer_3_sec(curr_ti) || elevio_obstruction()) {     // turn doorlamp off when timer and no obstruction
            set_motor_dir(e, DIRN_STOP);            // To satisfy specification S1 (never open doors when moving)
            button_poller(e);       // handle buttons while doors are open

            if (elevio_stopButton()) {      // stop if stop button pressed (Specification D3)
                e->curr_state = STOP;
                break;
            }
        }
        elevio_doorOpenLamp(0);
}