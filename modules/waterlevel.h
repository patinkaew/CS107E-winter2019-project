#ifndef WATERLEVEL_H
#define WATERLEVEL_H

#include "gpio.h"

#define WATERLEVEL_TRIG     GPIO_PIN5
#define WATERLEVEL_ECHO     GPIO_PIN6
#define DEFAULT_TANK_HEIGHT 90 //mm

void waterlevel_init(int trig, int echo);

void waterlevel_set_tank_height(unsigned int height);

void waterlevel_calibrate_tank_height();

unsigned int waterlevel_get_tank_height();

unsigned int waterlevel_get_level();

unsigned int waterlevel_get_percent();

#endif //close WATER_LEVEL_H
