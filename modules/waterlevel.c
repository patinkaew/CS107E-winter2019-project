#include "waterlevel.h"
#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"

unsigned int TRIG, ECHO;
unsigned int tank_height = DEFAULT_TANK_HEIGHT;

static unsigned int waterlevel_get_distance(void) {
	// write HI for 10usec
	gpio_write(TRIG, 1);
	timer_delay_us(10);
	gpio_write(TRIG, 0);

	unsigned start = timer_get_ticks();
	timer_delay_us(13);
	while(!gpio_read(ECHO));

	unsigned end;
	while(gpio_read(ECHO) == 1);
	end = timer_get_ticks();

	unsigned int result = (end - start) / 12; //2.941176 us = time to go one mm
	return result < 1200 ? result : 0;
}

void waterlevel_init(int trig, int echo){
    TRIG = trig;
    ECHO = echo;

    gpio_set_output(TRIG);
    gpio_set_input(ECHO);
    gpio_set_pulldown(ECHO);
    timer_delay_ms(40);
}

void waterlevel_set_tank_height(unsigned int height){
    tank_height = height;
}

void waterlevel_calibrate_tank_height(){
    int sum_height = 0;
    for(int i = 0; i < 3; i++){
        sum_height += waterlevel_get_distance();
    }
    tank_height = sum_height / 3;
}

unsigned int waterlevel_get_tank_height(){
    return tank_height;
}

unsigned int waterlevel_get_level(){
    int result = tank_height - waterlevel_get_distance();
	if(result >= 0) return result;
	return 0;
}

unsigned int waterlevel_get_percent(){
    return (waterlevel_get_level() * 100)/tank_height;
}
