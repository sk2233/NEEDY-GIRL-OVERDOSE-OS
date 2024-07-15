#include "sys/pic.h"
#include "comm/x86.h"

// 用于管理 16个中断，默认除了从片连在主片上的位置全部屏蔽
static uint16_t irq_mask = 0xFFFF & ~(1 << PIC_SLAVE_IDX);

static void pic_mask(uint16_t mask){
    irq_mask = mask; // 更新写入控制数据
    outb(PIC_MASTER + 1, mask);
    outb(PIC_SLAVE + 1, mask >> 8);
}

void pic_enable(uint16_t irq){ // 移除对应的屏蔽
    pic_mask(irq_mask & ~(1 << irq));
}

void pic_init(){
    // 默认先全部屏蔽
    outb(PIC_MASTER + 1, 0xFF);
    outb(PIC_SLAVE + 1, 0xFF);
    // 设置主片状态
    outb(PIC_MASTER, 0x11);
    outb(PIC_MASTER + 1, PIC_OFFSET);
    outb(PIC_MASTER + 1, 1 << PIC_SLAVE_IDX);
    outb(PIC_MASTER + 1, 0x1);
    // 设置从片
    outb(PIC_SLAVE, 0x11);    // ICW1
    outb(PIC_SLAVE + 1, PIC_OFFSET + 8);  // ICW2
    outb(PIC_SLAVE + 1, PIC_SLAVE_IDX);       // ICW3
    outb(PIC_SLAVE + 1, 0x1);             // ICW4
    // 清除工作
    outb(PIC_MASTER, 0x68);    // clear specific mask
    outb(PIC_MASTER, 0x0a);    // read IRR by default
    outb(PIC_SLAVE, 0x68);    // OCW3
    outb(PIC_SLAVE, 0x0a);    // OCW3
    // 应用当前中断状态
    pic_mask(irq_mask);
}

bool_t irq_save() {
    if (read_eflags() & PIC_FLAG_IF) {
        cli();
        return TRUE;
    }
    return FALSE;
}

void irq_restore(bool_t flag) {
    if (flag) {
       sti();
    }
}

