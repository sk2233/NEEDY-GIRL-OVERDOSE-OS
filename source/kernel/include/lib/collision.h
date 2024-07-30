#ifndef OS_COLLISION_H
#define OS_COLLISION_H

#include "comm/type.h"

typedef struct point{
    int x;
    int y;
}point_t;

typedef struct rect{
    int x;
    int y;
    int w;
    int h;
}rect_t;

bool_t point_in_rect(rect_t *rect,point_t *point);

#endif
