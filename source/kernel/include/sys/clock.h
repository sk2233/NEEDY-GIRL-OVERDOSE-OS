#ifndef OS_CLOCK_H
#define OS_CLOCK_H

#include "comm/type.h"

extern volatile uint32_t tick_count;

void clock_init();
void clock_update();
void clock_wait(uint32_t time);

#endif
