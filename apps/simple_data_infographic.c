#include "uart.h"
#include "keyboard.h"
#include "timer.h"
#include "console.h"
#include "printf.h"
#include "gl.h"
#include "interrupts.h"
#include "../modules/dataset.h"
#include "../modules/graph.h"
#include "../modules/waterlevel.h"
#include "../modules/sampler.h"
#include "../modules/graph.h"
#include "../modules/infographic.h"

void main(void){
    uart_init();

    gl_init(1024, 768, GL_DOUBLEBUFFER);
    infographic_info_t ig_info = {true, true, true, 3, {8, 8}, {1016, 760}};
    infographic_init(ig_info);
    timer_init();
    dataset_init();
    sampler_init(ONE_SEC);
    interrupts_global_enable(); // everything fully initialized, now turn on interrupts

    timer_delay(2);
    //graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(1),1, GL_RED, GL_AMBER);
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(2), 2, GL_RED, GL_GREEN);

    while(1){
        infographic_update_screen();
        gl_swap_buffer();
    }

}
