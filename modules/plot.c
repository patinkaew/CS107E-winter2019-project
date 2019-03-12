#include "plot.h"
#include "malloc.h"
#include "gl.h"
#include <stdbool.h>
//#include "vector.c"

/*
 * Global variables for graphic
 */
static color_t bg_color = GL_BLACK, info_color = GL_WHITE;
static plot_info_t *plot_info;

/*
 * Global variables for data buffer
 */
static struct data_point_t{
    int x;
    int y;
    int marker;
    color_t color;
};

static data_point_t *data_points;

void plot_init(plot_info_t info){
    plot_info = malloc(sizeof(plot_info_t));

    plot_info -> gl_min = info.gl_min;
    plot_info -> gl_max = info.gl_max;
    plot_info -> show_drawspace = info.show_drawspace;

    plot_info -> gl_origin = info.gl_origin;
    plot_info -> show_origin = info.show_origin;
    plot_info -> origin_marker = info.origin_marker;

    plot_info -> graph_label = info.graph_label;
    plot_info -> show_graph_label = info.show_graph_label;

    plot_info -> show_x_axis = info.show_x_axis;
    plot_info -> x_axis_label = info.x_axis_label;
    plot_info -> show_x_axis_label = info.show_x_axis_label;
    plot_info -> x_scaling = info.x_scaling;

    plot_info -> show_y_axis = info.show_y_axis;
    plot_info -> y_axis_label = info.y_axis_label;
    plot_info -> show_y_axis_label = info.show_y_axis_label;
    plot_info -> y_scaling = info.y_scaling;
}

void plot_set_drawspace(point_t gl_min, point_t gl_max){
    plot_info -> gl_min = gl_min;
    plot_info -> gl_max = gl_max;
}

void plot_show_drawspace(){
    plot_info -> show_drawspace = true;
}

void plot_hide_drawspace(){
    plot_info -> show_drawspace = false;
}

void plot_set_origin(point_t gl_origin, marker_t marker){
    plot_info -> gl_origin = gl_origin;
    plot_info -> origin_marker = marker;
}

void plot_show_origin(){
    plot_info -> show_origin = true;
}

void plot_hide_origin(){
    plot_info -> show_origin = false;
}
/*
void plot_show_axis(enum axes axis);

void plot_hide_axis(enum axes axis);

void plot_set_graph_label(char* name);

void plot_show_graph_label();

void plot_hide_graph_label();

void plot_set_axis_label(char* name, enum axes axis);

void plot_show_axis_label(enum axes axis);

void plot_hide_axis_label(enum axes axis);

void plot_set_axis_scaling(int scaling, enum axes axis); */

static inline bool is_in_bound(){
    return (x >= (plot_info -> gl_min).x) && (x <= (plot_info -> gl_max).x)
            && (y >= (plot_info -> gl_min).y) && (y <= (plot_info -> gl_max).y);
}

static inline void plot_update_drawspace(){
    if(plot_info -> show_drawspace){
        int max = (plot_info -> gl_max).x;
        for(int i = (plot_info -> gl_min).x; i < max; i++){
            gl_draw_pixel(i, (plot_info -> gl_min).y, info_color);
            gl_draw_pixel(i, (plot_info -> gl_max).y, info_color);
        }
        max = (plot_info -> gl_max).y;
        for(int i = (plot_info -> gl_min).y; i < max; i++){
            gl_draw_pixel((plot_info -> gl_min).x, i, info_color);
            gl_draw_pixel((plot_info -> gl_max).x, i info_color);
        }
    }

    if(plot_info -> show_x_axis){
        int max = (plot_info -> gl_max).x;
        for(int i = (plot_info -> origin).x; i < max; i++){
            gl_draw_pixel(i, (plot_info -> origin).y, info_color);
        }
    }

    if(plot_info -> show_y_axis){
        int max = (plot_info -> gl_max).y;
        for(int i = (plot_info -> origin).y; i < max; i++){
            gl_draw_pixel((plot_info -> origin).x, i, info_color);
        }
    }
}

static inline void plot_update_data_points(){

}

void plot_update_screen(){
    gl_clear(bg_color);
    plot_update_drawspace();
    plot_update_data_points();
    gl_swap_buffer();
}
