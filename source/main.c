#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elev_controller.h"
// #include "driver/elevator.h"
#include "driver/orders.h"

#include "driver/timer.h"

int main(){
    elevio_init();
    static ElevatorState queue;
    init_queue(&queue);
    // update_state(&queue);
    init_controller(&queue);

    printf("running\n");
    printf("\n");
    // queue.new_floor = 2;         //for testing
    while (1)
    {
        //printf("###########################\n");
        update_state(&queue);
        stop_button_poller();
        button_poller(&queue);
        //print_table();

        //printf("last_floor: %d\n", queue.last_floor);
        //printf("is below: %d\n", order_below_curr_floor(&queue));
        //printf("is above: %d\n", order_above_curr_floor(&queue));
        //printf("############################\n");
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    
   

    return 0;
}
