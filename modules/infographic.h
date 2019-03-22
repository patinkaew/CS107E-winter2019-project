#ifndef INFOGRAPHIC_H
#define INFOGRAPHIC_H

/*
 * Author: Patin Inkaew
 * Date: Mar 2019 (CS 107E Winter 2019)
 */

#include <stdbool.h>
#include "graph.h"

enum orientation{
    HORIZONTAL_TABLE_FIRST  = 0,
    HORIZONTAL_GRAPH_FIRST  = 1,
    VERTICAL_TABLE_FIRST    = 2,
    VERTICAL_GRAPH_FIRST    = 3
};

typedef struct{
    bool                use_table;
    bool                use_graph;
    bool                use_laptime;
    enum orientation    orientation;
    point_t             gl_min;
    point_t             gl_max;
}infographic_info_t;

typedef struct{
    bool                use_table;
    bool                use_graph;
    bool                use_laptime;
    enum orientation    orientation;
    point_t             gl_min;
    point_t             gl_max;
    point_t             graph_gl_min;
    point_t             graph_gl_max;
    point_t             table_gl_min;
    point_t             table_gl_max;
    point_t             laptime_gl_min;
    point_t             laptime_gl_max;
}infographic_dev_t;

void infographic_init(infographic_info_t info);

bool infographic_update_screen();

#endif //close INFOGRAPHIC_H
