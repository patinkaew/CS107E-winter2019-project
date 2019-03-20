#ifndef PLOT_H
#define PLOT_H

#include "gl.h"
#include "dataset.h"
#include "markers.h"
#include <stdbool.h>
#include <stdint.h>

enum axes{
    X = 0, Y = 1
};

typedef struct{
    int x;
    int y;
}point_t;

typedef struct{
    float x;
    float y;
}data_point_t;

typedef struct{
    point_t     gl_min;
    point_t     gl_max;
    bool        show_drawspace;

    point_t     gl_origin;
    bool        show_origin;
    int         origin_marker;

    char*       graph_label;
    bool        show_graph_label;

    bool        show_x_axis;
    char*       x_axis_label;
    bool        show_x_axis_label;
    int         x_scaling; //number of pixels equal to one unit of data

    bool        show_y_axis;
    char*       y_axis_label;
    bool        show_y_axis_label;
    int         y_scaling; //number of pixels equal to one unit of data
}plot_info_t;

void plot_init(plot_info_t info);

void plot_set_drawspace(point_t gl_min, point_t gl_max);

void plot_show_drawspace();

void plot_hide_drawspace();

void plot_set_origin(point_t gl_origin, int marker);

void plot_show_origin();

void plot_hide_origin();

void plot_show_axis(enum axes axis);

void plot_hide_axis(enum axes axis);

void plot_set_graph_label(char* name);

void plot_show_graph_label();

void plot_hide_graph_label();

void plot_set_axis_label(char* name, enum axes axis);

void plot_show_axis_label(enum axes axis);

void plot_hide_axis_label(enum axes axis);

void plot_set_axis_scaling(int scaling, enum axes axis);

void plot_add_dataset(uintptr_t* data_set, int marker_style, color_t marker_color, color_t line_color);

void plot_remove_dataset(uintptr_t* data_set);

void plot_update_screen();

#endif //PLOT_H
