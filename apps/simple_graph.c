#include "printf.h"
#include "uart.h"
#include "gl.h"
#include "../modules/graph.h"
#include "../modules/dataset.h"
#include "malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void main(void){
    uart_init();
    gl_init(1024, 768, GL_SINGLEBUFFER);
    graph_info_t plot_info = {{8, 8}, {1016, 760}, true, {512, 384}, true, 1, "", false, true, "", false, 16, true, "", false, 16};
    graph_init(plot_info);
    dataset_init();

    for(int i = -10; i <= 10; i++){
        dataset_add_data(0, i);
        dataset_add_data(1, i);
        dataset_add_data(2, -i);

    }
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(1),1, GL_RED, GL_AMBER);
    graph_add_dataset(dataset_get_dataset(0), dataset_get_dataset(2),2, GL_BLUE, GL_GREEN);
    graph_update_screen();
    printf("Finish\04");
}
