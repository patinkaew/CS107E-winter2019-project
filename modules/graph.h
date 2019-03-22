#ifndef GRAPH_H
#define GRAPH_H

/*
 * 'Graph module'
 * Graph module displays given data set on the monitor using gl library.
 * This library assumes that given data is data_point_t type, containing x and y
 * values of the data point. When plotting, user need to add dataset which is
 * a set of data points. User cannot add individual point.
 * In MVC model (Model-View-Controller), this library is View.
 *
 * Since this function will connect point to point, correspoding to their index
 * in the array, to ensure the proper result, data should be sorted in x from low to high.
 * In this project, x axis is time and we cannot reverse time, so datais sorted.
 *
 * File: graph.h
 * Author: Patin Inkaew
 * Date: Mar 2019 (CS 107E Winter 2019)
 */

#include "gl.h"
#include "dataset.h"
#include "markers.h"
#include <stdbool.h>
#include <stdint.h>

/***** Enumerate values and Data type definitions *****/

/*
 * Assign value to x-axis and y-axis
 */
enum axes{
    X_AXIS = 0, Y_AXIS = 1
};

/*
 * 'point_t' stores the absolute x and y value on the screen
 */
typedef struct{
    int x; //pixel index is integer value
    int y;
}point_t;

/*
 * 'graph_info_t' store all customizable functionalities of graph module
 */
typedef struct{
    //scope draw space (assume rectangle)
    point_t     gl_min; //minimum x and y value
    point_t     gl_max; //maximum x and y value
    bool        show_drawspace; //whether to show outline border

    //locate the origin as reference
    point_t     gl_origin; //graphical x and y value marked as origin
    bool        show_origin; //whether to show the origin
    int         origin_marker; //marker style of origin

    //control title of the graph
    char*       graph_label; //graph's name
    bool        show_graph_label; //whether to show graph name (top left of drawspace)

    //control display of x axis
    bool        show_x_axis; //whether to show the x-axis
    char*       x_axis_label; //x-axis's name
    bool        show_x_axis_label; //whether to show the x axis name (rightmost under x-axis)
    int         x_scaling; //number of pixels equal to one unit of data

    //control display of y axis
    bool        show_y_axis; //whether to show the y-axis
    char*       y_axis_label; //y-axis's name
    bool        show_y_axis_label;
    int         y_scaling; //number of pixels equal to one unit of data
}graph_info_t;

/***** Global functions *****/

/*
 * Initialize the graph module
 * @param info is a graph_info_t data type storing desired functionalities
 */
void graph_init(graph_info_t info);

/*
 * Set the show drawspace. Every other features will be draw in this range
 * @param gl_min is a minimum graphical x and y value
 * @param gl_max is a maximum graphical x and y value
 */
void graph_set_drawspace(point_t gl_min, point_t gl_max);

/*
 * Show the border indicating drawspace
 */
void graph_show_drawspace();

/*
 * Hide the border indicating drawspace
 */
void graph_hide_drawspace();

/*
 * Set the location of origin
 * @param gl_origin is the graphical x and y value marked as origin
 * @parem marker is the marker style of origin
 */
void graph_set_origin(point_t gl_origin, int marker);

/*
 * Show the origin point
 */
void graph_show_origin();

/*
 * Hide the origin point
 */
void graph_hide_origin();

/*
 * Show the given axis
 * @param axis is either x-axis or y-axis
 */
void graph_show_axis(enum axes axis);

/*
 * Hide the given axis
 * @param axis is either x-axis or y-axis
 */
void graph_hide_axis(enum axes axis);

/*
 * Set graph's label
 * @param name is graph's name
 */
void graph_set_graph_label(char* name);

/*
 * Show graph's name
 */
void graph_show_graph_label();

/*
 * Hide graph's name
 */
void graph_hide_graph_label();

/*
 * Set axis's name
 * @param name is the axis's name
 * @param axis is either x-axis or y-axis
 */
void graph_set_axis_label(char* name, enum axes axis);

/*
 * Show axis' name
 * @param axis is either x-axis or y-axis
 */
void graph_show_axis_label(enum axes axis);

/*
 * Hide axis' name
 * @param axis is either x-axis or y-axis
 */
void graph_hide_axis_label(enum axes axis);

/*
 * Set axis' scaling
 * @param scaling is scaling factor
 * @param axis is either x-axis or y-axis
 */
void graph_set_axis_scaling(int scaling, enum axes axis);

/*
 * Add dataset to be plotted
 */
void graph_add_dataset(uintptr_t* data_set_x, uintptr_t* data_set_y, int marker_style, color_t marker_color, color_t line_color);

/*
 * Remove dataset to be plotted
 * This function does not delete the actual data. Data is still in the memory, but it is not plotted
 */
void graph_remove_dataset(uintptr_t* data_set_x, uintptr_t* data_set_y);

/*
 * Redraw the entire graph
 * This function must be called to see changes
 */
bool graph_update_screen();

void graph_enable_auto_shift();

#endif //GRAPH_H
