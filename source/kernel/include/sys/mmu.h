#ifndef OS_MMU_H
#define OS_MMU_H

#include "comm/type.h"

#define	IO_RTC		0x0070
// 获取16m以外的内存
#define MMU_EXT16 	0x34
#define MMU_K       1024
#define MMU_M       0x100000
#define MMU_PAGE    0x1000
// 从 1m 开始分配
#define MMU_START   MMU_M
#define MMU_NODE_SIZE 1024

void mmu_init();
void *mem_alloc(uint32_t size);
void *page_alloc(uint32_t count);
void mem_free(void *mem);
void print_mmu_info();

typedef struct mmu_node{
    uint32_t start;
    uint32_t size;
    bool_t used;
    struct mmu_node *next;
    struct mmu_node *pre;
}mmu_node_t;

#endif
