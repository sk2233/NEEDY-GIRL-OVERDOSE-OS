#ifndef OS_TYPE_H
#define OS_TYPE_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef long int32_t;

typedef int bool_t;

#define TRUE 1
#define FALSE 0
#define NULL 0

typedef struct rgb{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}rgb_t;

#endif
