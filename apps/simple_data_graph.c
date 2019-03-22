#include "uart.h"
#include "keyboard.h"
#include "timer.h"
#include "console.h"
#include "printf.h"
<<<<<<< HEAD
#include "shell.h"
#include "gl.h"
#include "modules/dataset.h"
#include "modules/graph.h"
#include "modules/waterlevel.h"
#include "interrupts.h"
#include "modules/sampler.h"
#include "modules/graph.h"
#include "modules/infographic.h"
=======
#include "modules/moisture.h"
>>>>>>> b7df82336604a77702c31716d8dc44c4702cda95

/*void main(void){
    uart_init();
<<<<<<< HEAD
    //keyboard_init(GPIO_PIN25, GPIO_PIN26);
    console_init(20, 40);
    //shell_init(printf);
    sampler_init(ONE_SEC);

    gl_init(1024, 768, GL_DOUBLEBUFFER);
    graph_info_t plot_info = {{8, 8}, {1016, 760}, true, {16, 752}, true, 1, "", false, true, "", false, 4, true, "", false, 4};
    graph_init(plot_info);

    //keyboard_use_interrupts(); //use interrupt driven keyboard
    interrupts_global_enable(); // everything fully initialized, now turn on interrupts

    timer_delay(2);
    //graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(1),1, GL_RED, GL_AMBER);
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(2), 2, GL_RED, GL_GREEN);
    //shell_run();
    while(1){
        graph_update_screen();
        gl_swap_buffer();
    }
}*/

void main(void){
    uart_init();
    gl_init(1024, 768, GL_DOUBLEBUFFER);
    /*typedef struct{
        bool                use_table;
        bool                use_graph;
        bool                use_laptime;
        enum orientation    orientation;
        point_t             gl_min;
        point_t             gl_max;
    }infographic_info_t; */
    infographic_info_t ig_info = {false, true, true, 3, {8, 8}, {1016, 760}};
    infographic_init(ig_info);

    for(int i = -10; i <= 10; i++){
        dataset_add_data(0, i);
        dataset_add_data(1, i);
        dataset_add_data(2, -i);

    }
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(1),1, GL_RED, GL_AMBER);
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(2),2, GL_BLUE, GL_GREEN);

    infographic_update_screen();

=======
    timer_init();

    readSoil();
    // waterlevel_init(GPIO_PIN3, GPIO_PIN2);
    // waterlevel_calibrate_tank_height();
    // printf("tank_height: %d %\n", waterlevel_get_tank_height());
    // timer_delay(2);
    // while(1){
    //     printf("level: %d percent: %d %\n", waterlevel_get_level(), waterlevel_get_percent());
    //     timer_delay_ms(250);
    //}
>>>>>>> b7df82336604a77702c31716d8dc44c4702cda95
}
