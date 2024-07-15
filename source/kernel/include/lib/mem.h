#ifndef OS_MEM_H
#define OS_MEM_H

#include "comm/type.h"

void mem_set(void *dst,uint8_t val,uint32_t len);
void mem_copy(void *dst,void *src,uint32_t len);

#endif
