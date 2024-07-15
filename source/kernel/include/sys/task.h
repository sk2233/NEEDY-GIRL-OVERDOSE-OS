#ifndef OS_TASK_H
#define OS_TASK_H

#include "comm/type.h"

#define TASK_GDT_COUNT 1024

#define TASK_ATTR_TSS	0x0089

// cpu因为设计为兼容历史逻辑，所以很多数字很分散
typedef struct seg_desc {
    // base 32bit  limit 20bit (limit_high能使用的只有低4位) 通过标志位中的粒度标记标明单位是byte还是4kb 其他的让给控制位
    uint16_t limit_low, base_low;
    uint8_t base_mid, access_right;
    uint8_t limit_high, base_high;
}seg_desc_t;

typedef struct tss {
    uint32_t backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldtr, iomap;
}tss_t;

void task_init();
uint32_t task_create(void (*func)());
void task_switch(uint32_t task_sel);

#endif
