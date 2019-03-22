#include "infographic.h"
#include "malloc.h"
#include "table.h"
#include "graph.h"
#include "gl.h"
#include "printf.h"
#include "timer.h"
#include <stdbool.h>

static infographic_dev_t *dev;

void infographic_init(infographic_info_t info){
    dev = malloc(sizeof(infographic_dev_t));
    dev -> use_table = info.use_table;
    dev -> use_graph = info.use_graph;
    dev -> use_laptime = info.use_graph;
    dev -> orientation = info.orientation;
    dev -> gl_min = info.gl_min;
    dev -> gl_max = info.gl_max;
    switch(dev -> orientation){
        case 0: break;
        case 1: break;
        case 2: break;
        case 3:
            dev -> graph_gl_min = dev -> gl_min;
            dev -> graph_gl_max = (point_t){(dev -> gl_max).x, ((dev -> gl_max).y - 24) / 2};
            dev -> table_gl_min = (point_t){(dev -> gl_min).x, ((dev -> gl_max).y - 24) / 2};
            dev -> table_gl_max = (point_t){(dev -> gl_max).x, (dev -> gl_max).y - 24};
            dev -> laptime_gl_min = (point_t){(dev -> gl_min).x, (dev -> gl_max).y - 24};
            dev -> laptime_gl_max = dev -> gl_max;
        break;
    }
    /*if(dev -> use_table){ //has to implement
        table_init();
    } */
    if(dev -> use_graph){
        graph_info_t plot_info = {{(dev -> graph_gl_min).x + 8, (dev -> graph_gl_min).y + 8},
                                    {(dev -> graph_gl_max).x - 8, (dev -> graph_gl_max).y - 8}, true,
                                    {(dev -> graph_gl_min).x + 16, (dev -> graph_gl_max).y - 16}, true, 1,
                                    "", false, true, "", false, 16, true, "", false, 16};
        graph_init(plot_info);
        graph_enable_auto_shift();
    }
}

bool infographic_update_screen(){
    gl_clear(GL_BLACK);
    bool rslt = true;
    /*if(dev -> use_table){ //has to implement
        //rslt = rslt && table_update_screen();
    }*/
    if(dev -> use_graph){
        rslt = rslt && graph_update_screen();
    }

    if(dev -> use_laptime){
        char time_buf[20];
        unsigned int curr_time = timer_get_ticks() / 1000000;
        snprintf(time_buf, 20, "LAP TIME: %02d:%02d", curr_time/60, curr_time%60);
        gl_draw_string((dev -> laptime_gl_min).x + 4, (dev -> laptime_gl_min).y + 4, time_buf, GL_WHITE);
    }

    return rslt;
}
