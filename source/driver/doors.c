#include "doors.h"


void open_doors(ElevatorState * e) {
        time_t curr_ti;
        start_timer(&curr_ti);
        elevio_doorOpenLamp(1);       // turn on doorLamp
        while (!timer_3_sec(curr_ti)) {
            button_poller(e);       // handle buttons while doors are open
        }
        elevio_doorOpenLamp(0);
}