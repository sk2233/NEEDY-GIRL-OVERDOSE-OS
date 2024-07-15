#ifndef OS_CLOCK_H
#define OS_CLOCK_H

#include "comm/type.h"

extern volatile uint32_t tick_count;

void clock_init();

#endif
