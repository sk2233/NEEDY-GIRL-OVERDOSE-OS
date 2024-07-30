#include "lib/collision.h"

bool_t point_in_rect(rect_t *rect,point_t *point){
    return (point->x>rect->x)&&(point->x<rect->x+rect->w)&&(point->y>rect->y)&&(point->y<rect->y+rect->h);
}