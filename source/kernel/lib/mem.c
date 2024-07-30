#include "lib/mem.h"

void mem_set(void *dst,uint8_t val,uint32_t len){
    uint8_t *temp=dst;
    while (len>0){
        *temp++=val;
        len--;
    }
}

void mem_copy(void *dst,void *src,uint32_t len){
    uint32_t *dst_u32 = dst;
    uint32_t *src_u32 = src;
    while (len>=4){
        *dst_u32++=*src_u32++;
        len-=4;
    }
    uint8_t *dst_u8=(uint8_t *)dst_u32;
    uint8_t *src_u8=(uint8_t *)src_u32;
    while (len>=1){
        *dst_u8++=*src_u8++;
        len--;
    }
}