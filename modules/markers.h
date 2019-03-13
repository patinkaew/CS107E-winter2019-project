#ifndef MARKERS_H
#define MARKERS_H

#include <stddef.h>
#include <stdbool.h>

typedef struct{
    size_t num_markers;
    size_t width;
    size_t height;
    unsigned char pixel_data[];
}markers_t;

size_t markers_get_height(void);

size_t markers_get_width(void);

size_t markers_get_size(void);

bool markers_get_marker(int style, unsigned char buf[], size_t buflen);

#endif //close MARKERS_H
