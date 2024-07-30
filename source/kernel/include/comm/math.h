#ifndef OS_MATH_H
#define OS_MATH_H

#include "type.h"

static inline int max(int val1,int val2){
    if(val1>val2){
        return val1;
    }
    return val2;
}

static inline int min(int val1,int val2){
    if(val1<val2){
        return val1;
    }
    return val2;
}

static inline int clump(int val,int min,int max){
    if(val<min){
        return min;
    }
    if(val>max){
        return max;
    }
    return val;
}

#endif
