#include "printf.h"
#include "uart.h"
#include "modules/plot.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void main(void){
    uart_init();
    gl_init(640, 512, GL_SINGLEBUFFER);
    plot_info_t plot_info = {{8, 8}, {632, 504}, true, {320, 256}, true, 1, "", false, true, "", false, 8, true, "", false, 8};
    plot_init(plot_info);

    for(int i = -5; i <= 5; i++){
        point_t data1 = {2*i, 2*i};
        plot_add_data_point(data1, 1, GL_AMBER);
        point_t data2 = {i, i * i};
        plot_add_data_point(data2, 2, GL_GREEN);
        point_t data3 = {i, i * i * i};
        plot_add_data_point(data3, 3, GL_BLUE);
    }
    plot_update_screen();
}
