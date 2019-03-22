#include "sampler.h"
#include "armtimer.h"
#include "timer.h"
#include "malloc.h"
#include "console.h"
#include "strings.h"
#include "interrupts.h"
#include "dataset.h"
#include "printf.h"
//#include "moisture.h"
#include "waterlevel.h"
#include <stdbool.h>
#include <stdint.h>

/*
 * Increment the coressponding counter when interrupt happen
 * this function is a event handler
 */
static bool sampler_handler(unsigned int pc){
    if(armtimer_check_and_clear_interrupt()){
        dataset_add_data(0, timer_get_ticks() / ONE_SEC); //time stamp in sec
        //dataset_add_data(1, ); //moisture
        dataset_add_data(2, waterlevel_get_percent()); //water level
        return true;
    }
    return false;
}

/*
 * Initiate sampler
 */
void sampler_init(unsigned int interval){

    //initiate armtimer
    armtimer_init(interval); //countdown
    armtimer_enable();

    //initiate interrupts
    interrupts_enable_basic(INTERRUPTS_BASIC_ARM_TIMER_IRQ);
    interrupts_attach_handler(sampler_handler);
    armtimer_enable_interrupts();

    //initiate dataset
    dataset_init();

    //initiate sensors
    gpio_init();
    timer_init();
    waterlevel_init(WATERLEVEL_TRIG, WATERLEVEL_ECHO);
    //moisture_init();
}
