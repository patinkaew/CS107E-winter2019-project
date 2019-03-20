#include "printf.h"
#include "uart.h"
#include "gl.h"
#include "modules/plot.h"
#include "modules/dataset.h"
#include "malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void main(void){
    uart_init();
    gl_init(1024, 768, GL_DOUBLEBUFFER);
    plot_info_t plot_info = {{8, 8}, {1016, 760}, true, {512, 384}, true, 1, "", false, true, "", false, 16, true, "", false, 16};
    plot_init(plot_info);
    dataset_init();

    for(int i = -10; i <= 10; i++){
        data_point_t* p1 = malloc(sizeof(point_t));
        p1 -> x = i;
        p1 -> y = i;
        dataset_add_data(1, (uintptr_t)p1);
        data_point_t* p2 = malloc(sizeof(point_t));
        p2 -> x = i;
        p2 -> y = ((float)i * i)/10;
        dataset_add_data(2, (uintptr_t)p2);
    }
    /*point_t* p = malloc(sizeof(point_t));
    p -> x = 10;
    p -> y = 10;
    dataset_add_data(1, (uintptr_t)p);*/
    plot_add_dataset(dataset_get_dataset(1), 1, GL_RED, GL_AMBER);
    plot_add_dataset(dataset_get_dataset(2), 1, GL_GREEN, GL_AMBER);
    plot_update_screen();
    printf("Finish\04");
}
