#include "uart.h"
#include "keyboard.h"
#include "timer.h"
#include "printf.h"
#include "gl.h"
#include "shell.h"
#include "interrupts.h"
#include "modules/graph.h"
#include "modules/waterlevel.h"
#include "modules/sampler.h"
#include "modules/graph.h"
#include "modules/infographic.h"

void main(void){
    uart_init();
    gl_init(1024, 768, GL_DOUBLEBUFFER);
    timer_init();
    dataset_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    keyboard_use_interrupts();
    printf("finishing keyboard\n");
    shell_init(printf);
    printf("finishing shell\n");
    sampler_init(ONE_SEC);
    printf("finishing sampler\n");
    infographic_info_t ig_info = {true, true, true, 3, {8, 8}, {1016, 760}};
    infographic_init(ig_info);
    interrupts_global_enable(); // everything fully initialized, now turn on interrupts
    printf("finishing info\n");
    timer_delay(2); //wait to get first data
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(2), 2, GL_AMBER, GL_GREEN);
    printf("finishing init\n");
    infographic_update_screen();
    shell_run();
}
