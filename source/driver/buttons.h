#ifndef BUTTONS
#define BUTTONS

#include <stdio.h>
#include "elevio.h"
#include "orders.h"
#include <time.h>
#include "doors.h"
//#include "elev_controller.h"

void button_poller(ElevatorState * e);

void stop_button_poller(ElevatorState * e);

void darken_buttons(ElevatorState * e, int floor);

#endif 