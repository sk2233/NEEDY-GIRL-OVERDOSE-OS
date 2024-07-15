#ifndef OS_IDT_H
#define OS_IDT_H

#include "comm/type.h"

#define STS_IG32        0xE         // 32-bit Interrupt Gate
#define STS_TG32        0xF         // 32-bit Trap Gate

// 门描述符宏
#define SET_GATE(gate, is_trap, off) {               \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;      \
    (gate).gd_ss = 8;                                \
    (gate).gd_args = 0;                                 \
    (gate).gd_rsv1 = 0;                                 \
    (gate).gd_type = (is_trap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = 0;                              \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}

typedef struct gate_desc { // 门描述符
    unsigned gd_off_15_0 : 16;      // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;           // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;           // reserved(should be zero I guess)
    unsigned gd_type : 4;           // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;              // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;              // Present
    unsigned gd_off_31_16 : 16;     // high bits of offset in segment
}gate_desc_t;

typedef struct push_regs { // 压入的异常
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t unused;      // oesp
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
}push_regs_t;

typedef struct trap_frame { // 中断栈帧
    push_regs_t regs;
    uint16_t gs;
    // 都是按 32位压入的，有的只需要 16位  使用padding占位
    uint16_t padding0;
    uint16_t fs;
    uint16_t padding1;
    uint16_t es;
    uint16_t padding2;
    uint16_t ds;
    uint16_t padding3;
    uint32_t trap_num;
    uint32_t err;
    uint32_t eip;
    uint16_t cs;
    uint16_t padding4;
    uint32_t eflags;
    uint32_t esp;
    uint16_t ss;
    uint16_t padding5;
}__attribute__((packed)) trap_frame_t;

void idt_init();

#endif