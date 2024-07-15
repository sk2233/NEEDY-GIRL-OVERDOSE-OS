#ifndef OS_PIC_H
#define OS_PIC_H

#include "comm/type.h"

#define PIC_MASTER            0x20    // Master (IRQs 0-7)
#define PIC_SLAVE             0xA0    // Slave (IRQs 8-15)

#define PIC_ECHO (1<<5)

#define PIC_SLAVE_IDX           2     // Slave 在 Master 上的位置

#define PIC_OFFSET      0x20

#define PIC_TIMER               0
#define PIC_KBD                 1

#define PIC_FLAG_IF           0x00000200  // 中断标志位

void pic_init();
void pic_enable(uint16_t irq);

bool_t irq_save();
void irq_restore(bool_t flag);

#endif
