#include "plot.h"
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
 * Global variables for graphic
 */
static color_t bg_color = GL_BLACK, info_color = GL_WHITE;
static plot_info_t *plot_info;

static struct plot_set_t{
    int marker_style;
    color_t marker_color;
    color_t line_color;
    uintptr_t *data_set;
};

static uintptr_t* plot_sets;

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

    plot_sets = vector_init(); //sets of data points
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

void plot_set_origin(point_t gl_origin, int marker){
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

/* internal buffer management*/
void plot_add_dataset(uintptr_t* data_set, int marker_style, color_t marker_color, color_t line_color){
    struct plot_set_t* new_plot = malloc(sizeof(struct plot_set_t));
    new_plot -> marker_style = marker_style;
    new_plot -> marker_color = marker_color;
    new_plot -> line_color = line_color;
    new_plot -> data_set = data_set;
    plot_sets = vector_add(plot_sets, (uintptr_t)new_plot);
}

void plot_remove_dataset(uintptr_t* data_set){

}

static inline bool is_in_bound(int x, int y){
    return (x > (plot_info -> gl_min).x) && (x < (plot_info -> gl_max).x)
            && (y > (plot_info -> gl_min).y) && (y < (plot_info -> gl_max).y);
}

/* graphic management */
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
            gl_draw_pixel((plot_info -> gl_max).x, i, info_color);
        }
    }

    if(plot_info -> show_x_axis){
        int max = (plot_info -> gl_max).x;
        for(int i = (plot_info -> gl_min).x; i < max; i++){
            gl_draw_pixel(i, (plot_info -> gl_origin).y, info_color);
        }
    }

    if(plot_info -> show_y_axis){
        int max = (plot_info -> gl_max).y;
        for(int i = (plot_info -> gl_min).y; i < max; i++){
            gl_draw_pixel((plot_info -> gl_origin).x, i, info_color);
        }
    }
}

static inline void plot_update_data_plot(struct plot_set_t *plot_set){
    uintptr_t* data_points = ((data_set_t*)(plot_set -> data_set)) -> list;
    int data_size = vector_size(data_points);
    int last_gl_x = 0, last_gl_y = 0;
    for(int i = 0; i < data_size; i++){
        data_point_t *dp = (data_point_t*)vector_get(data_points, i);
        int gl_x = (plot_info -> gl_origin).x + ((dp -> x)* (plot_info -> x_scaling));
        int gl_y = (plot_info -> gl_origin).y - ((dp -> y)* (plot_info -> y_scaling)); //y is opposite
        int marker_width = markers_get_width();
        int marker_size = markers_get_size();
        unsigned char buf[marker_size];
        int marker_x = gl_x - marker_width/2;
        int marker_y = gl_y - (markers_get_height())/2;
        if(markers_get_marker(plot_set -> marker_style, buf, marker_size)){ //draw data point
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
}

void plot_update_screen(){
    gl_clear(bg_color);
    plot_update_drawspace();
    int size = vector_size(plot_sets);
    for(int i = 0; i < size; i++){
        plot_update_data_plot((struct plot_set_t*)vector_get(plot_sets, i));
    }
    gl_swap_buffer();
}
