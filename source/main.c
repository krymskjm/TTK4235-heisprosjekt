#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elev_controller.h"
// #include "driver/elevator.h"
#include "driver/orders.h"



int main(){
    // elevio_init();
    
    // //Dette er eksempelkode som var i main fra før.
    // printf("=== Example Program ===\n");
    // printf("Press the stop button on the elevator panel to exit\n");

    // elevio_motorDirection(DIRN_UP);
    
    // while(1){
    //     int floor = elevio_floorSensor();
    //     printf("floor: %d \n",floor);

    //     if(floor == 0){
    //         elevio_motorDirection(DIRN_UP);
    //     }

    //     if(floor == N_FLOORS-1){
    //         elevio_motorDirection(DIRN_DOWN);
    //     }


    //     for(int f = 0; f < N_FLOORS; f++){
    //         for(int b = 0; b < N_BUTTONS; b++){
    //             int btnPressed = elevio_callButton(f, b);
    //             elevio_buttonLamp(f, b, btnPressed);
    //         }
    //     }

    //     if(elevio_obstruction()){
    //         elevio_stopLamp(1);
    //     } else {
    //         elevio_stopLamp(0);
    //     }
        
    //     if(elevio_stopButton()){
    //         elevio_motorDirection(DIRN_STOP);
    //         break;
    //     }
        
    //     nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    // }

    // elevio_init();
    // init_controller();
    // printf("running\n");
    // while (1){
    //     int floor = elevio_floorSensor();
    //     printf("floor: %d \n",floor);
    //     update_state();
    //     printf("State: %d \n", new_state);
    //     printf("curr_motor_dir: %d \n", curr_motor_dir);
    //     nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    // }

    elevio_init();
    static ElevatorState queue;
    init_queue(&queue);
    update_state(&queue);
    init_controller(&queue);
    // TEST: set startfloor to 1
    add_cab_floor(&queue, 1);
    queue.new_floor = 1;

    printf("running\n");
        printf("cab_queue, len(%d): ", queue.num_cab_floors);
    for (int i = 0; i < queue.num_cab_floors; i++){
        printf("%d ", queue.cab_floors[i]);
    }
    printf("\n");


    while (1)
    {
        //////////////////Update state/////////////
        // printf("main newfloor: %d\n", queue.new_floor);
        update_state(&queue);
        queue.current_floor = elevio_floorSensor();

        // when buttonpressed add to queue
        button_handler(&queue);
        // DEBUG: print out cab queue
        printf("cab_queue, len(%d): ", queue.num_cab_floors);
        for (int i = 0; i < queue.num_cab_floors; i++){
            printf("%d ", queue.cab_floors[i]);
        }
        printf("\n");
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    
   

    return 0;
}
