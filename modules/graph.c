#include "graph.h"
#include "dataset.h"
#include "gl.h"
#include "markers.h"
#include "malloc.h"
#include "vector.h"
#include "printf.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Global variables for graph module
 */
static color_t bg_color = GL_BLACK, info_color = GL_WHITE;
static graph_info_t *graph_info;
static bool auto_shift = false;

static struct plot_set_t{
    int marker_style;
    color_t marker_color;
    color_t line_color;
    uintptr_t *data_set_x;
    uintptr_t *data_set_y;
};

static uintptr_t* plot_sets;

void graph_init(graph_info_t info){
    graph_info = malloc(sizeof(graph_info_t));

    graph_info -> gl_min = info.gl_min;
    graph_info -> gl_max = info.gl_max;
    graph_info -> show_drawspace = info.show_drawspace;

    graph_info -> gl_origin = info.gl_origin;
    graph_info -> show_origin = info.show_origin;
    graph_info -> origin_marker = info.origin_marker;

    graph_info -> graph_label = info.graph_label;
    graph_info -> show_graph_label = info.show_graph_label;

    graph_info -> show_x_axis = info.show_x_axis;
    graph_info -> x_axis_label = info.x_axis_label;
    graph_info -> show_x_axis_label = info.show_x_axis_label;
    graph_info -> x_scaling = info.x_scaling;

    graph_info -> show_y_axis = info.show_y_axis;
    graph_info -> y_axis_label = info.y_axis_label;
    graph_info -> show_y_axis_label = info.show_y_axis_label;
    graph_info -> y_scaling = info.y_scaling;

    plot_sets = vector_init(); //sets of data points
}

void graph_set_drawspace(point_t gl_min, point_t gl_max){
    graph_info -> gl_min = gl_min;
    graph_info -> gl_max = gl_max;
}

void graph_show_drawspace(){
    graph_info -> show_drawspace = true;
}

void graph_hide_drawspace(){
    graph_info -> show_drawspace = false;
}

void graph_set_origin(point_t gl_origin, int marker){
    graph_info -> gl_origin = gl_origin;
    graph_info -> origin_marker = marker;
}

void graph_show_origin(){
    graph_info -> show_origin = true;
}

void graph_hide_origin(){
    graph_info -> show_origin = false;
}

void graph_show_axis(enum axes axis){
    if(axis == X_AXIS){
        graph_info -> show_x_axis = true;
    }else if(axis == Y_AXIS){
        graph_info -> show_y_axis = true;
    }
}

void graph_hide_axis(enum axes axis){
    if(axis == X_AXIS){
        graph_info -> show_x_axis = false;
    }else if(axis == Y_AXIS){
        graph_info -> show_y_axis = false;
    }
}

void graph_set_graph_label(char* name){
    graph_info -> graph_label = name;
}

void graph_show_graph_label(){
    graph_info -> show_graph_label = true;
}

void graph_hide_graph_label(){
    graph_info -> show_graph_label = false;
}

void graph_set_axis_label(char* name, enum axes axis){
    if(axis == X_AXIS){
        graph_info -> x_axis_label = name;
    }else if(axis == Y_AXIS){
        graph_info -> y_axis_label = name;
    }
}

void graph_show_axis_label(enum axes axis){
    if(axis == X_AXIS){
        graph_info -> show_x_axis_label = true;
    }else if(axis == Y_AXIS){
        graph_info -> show_y_axis_label = true;
    }
}

void graph_hide_axis_label(enum axes axis){
    if(axis == X_AXIS){
        graph_info -> show_x_axis_label = false;
    }else if(axis == Y_AXIS){
        graph_info -> show_y_axis_label = false;
    }
}

void graph_set_axis_scaling(int scaling, enum axes axis){
    if(axis == X_AXIS){
        graph_info -> x_scaling = scaling;
    }else if(axis == Y_AXIS){
        graph_info -> y_scaling = false;
    }
}

void graph_add_dataset(uintptr_t* data_set_x, uintptr_t* data_set_y, int marker_style, color_t marker_color, color_t line_color){
    struct plot_set_t* new_plot = malloc(sizeof(struct plot_set_t));
    new_plot -> marker_style = marker_style;
    new_plot -> marker_color = marker_color;
    new_plot -> line_color = line_color;
    new_plot -> data_set_x = data_set_x;
    new_plot -> data_set_y = data_set_y;
    plot_sets = vector_add(plot_sets, (uintptr_t)new_plot);
}

void graph_remove_dataset(uintptr_t* data_set_x, uintptr_t* data_set_y){
    int size = vector_size(plot_sets);
    for(int i = 0; i < size; i++){
        struct plot_set_t * curr = (struct plot_set_t*)vector_get(plot_sets, i);
        if(curr -> data_set_x == data_set_x && curr -> data_set_y == data_set_y){
            free(curr);
            vector_remove(plot_sets, i);
        }
    }
}

static inline bool is_in_bound(int x, int y){
    return (x > (graph_info -> gl_min).x) && (x < (graph_info -> gl_max).x)
            && (y > (graph_info -> gl_min).y) && (y < (graph_info -> gl_max).y);
}

/* graphic management */
static inline void graph_update_drawspace(){
    if(graph_info -> show_drawspace){
        int max = (graph_info -> gl_max).x;
        for(int i = (graph_info -> gl_min).x; i < max; i++){
            gl_draw_pixel(i, (graph_info -> gl_min).y, info_color);
            gl_draw_pixel(i, (graph_info -> gl_max).y, info_color);
        }
        max = (graph_info -> gl_max).y;
        for(int i = (graph_info -> gl_min).y; i < max; i++){
            gl_draw_pixel((graph_info -> gl_min).x, i, info_color);
            gl_draw_pixel((graph_info -> gl_max).x, i, info_color);
        }
    }

    if(graph_info -> show_x_axis){
        int max = (graph_info -> gl_max).x;
        for(int i = (graph_info -> gl_min).x; i < max; i++){
            gl_draw_pixel(i, (graph_info -> gl_origin).y, info_color);
        }
    }

    if(graph_info -> show_y_axis){
        int max = (graph_info -> gl_max).y;
        for(int i = (graph_info -> gl_min).y; i < max; i++){
            gl_draw_pixel((graph_info -> gl_origin).x, i, info_color);
        }
    }
}

static bool graph_update_data_plot(struct plot_set_t *plot_set, bool *need_shift){
    uintptr_t* data_x = ((data_set_t*)(plot_set -> data_set_x)) -> list;
    uintptr_t* data_y = ((data_set_t*)(plot_set -> data_set_y)) -> list;
    int data_size = vector_size(data_x);
    if(data_size != vector_size(data_y)){
        return false;
    }

    int last_gl_x = 0, last_gl_y = 0;
    for(int i = 0; i < data_size; i++){
        unsigned int dp_x = vector_get(data_x, i);
        unsigned int dp_y = vector_get(data_y, i);
        int gl_x = (graph_info -> gl_origin).x + (dp_x * (graph_info -> x_scaling));
        int gl_y = (graph_info -> gl_origin).y - (dp_y * (graph_info -> y_scaling)); //y is opposite
        if(gl_x >= (graph_info -> gl_max).x){
            *need_shift = true;
        }
        int marker_width = markers_get_width();
        int marker_size = markers_get_size();
        unsigned char buf[marker_size];
        int marker_x = gl_x - marker_width/2;
        int marker_y = gl_y - (markers_get_height())/2;

        //draw data point
        if(markers_get_marker(plot_set -> marker_style, buf, marker_size)){
            for(int j = 0; j < marker_size; j++){
                if(buf[j] == 0xff){
                    //draw with given color
                    int x = marker_x + (j % marker_width);
                    int y = marker_y + (j / marker_width);
                    if(is_in_bound(x, y)){
                        gl_draw_pixel(x, y, plot_set -> marker_color);
                    }
                }
            }
        }

        //draw line between two point
        if(i != 0 && is_in_bound(gl_x, gl_y) && is_in_bound(last_gl_x, last_gl_y)){ //draw line
            gl_draw_line(last_gl_x, last_gl_y, gl_x, gl_y, plot_set -> line_color);
        }
        last_gl_x = gl_x;
        last_gl_y = gl_y;
    }
    return true;
}

bool graph_update_screen(){
    bool rslt = true;

    //clean drawspace
    gl_draw_rect((graph_info->gl_min).x, (graph_info->gl_min).y, (graph_info->gl_max).x, (graph_info->gl_max).y, bg_color);
    graph_update_drawspace();
    int size = vector_size(plot_sets);
    bool need_shift = false;
    for(int i = 0; i < size; i++){
        bool temp = false;
        rslt = rslt && graph_update_data_plot((struct plot_set_t*)vector_get(plot_sets, i), &temp);
        need_shift = need_shift || temp;
    }

    if(need_shift && auto_shift){
        point_t new_origin = {(graph_info -> gl_origin).x - (8 * graph_info -> x_scaling), (graph_info -> gl_origin).y};
        graph_set_origin(new_origin, graph_info -> origin_marker);
    }

    return rslt;
}

void graph_enable_auto_shift(){
    auto_shift = true;
}
