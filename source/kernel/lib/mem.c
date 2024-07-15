#include "lib/mem.h"

void mem_set(void *dst,uint8_t val,uint32_t len){
    uint8_t *temp=dst;
    while (len>0){
        *temp++=val;
        len--;
    }
}

void mem_copy(void *dst,void *src,uint32_t len){
    uint8_t *temp0 = dst;
    uint8_t *temp1 = src;
    while (len>0){
        *temp0++=*temp1++;
        len--;
    }
}