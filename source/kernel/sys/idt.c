#include "sys/idt.h"
#include "comm/type.h"
#include "sys/mmu.h"
#include "comm/x86.h"
#include "sys/pic.h"
#include "lib/stdio.h"
#include "sys/clock.h"
#include "io/kbd.h"
#include "io/mouse.h"

static gate_desc_t idts[256] = {}; // 中断描述符

void idt_init(){
    extern uint32_t idt_table[];
    for (int i = 0; i < 256; ++i) {
        SET_GATE(idts[i],FALSE,idt_table[i])
    }
    lidt((uint32_t)idts, sizeof(idts));
}

void handler(trap_frame_t *trap_frame){
    uint8_t data;
    switch (trap_frame->trap_num) {
        case PIC_OFFSET+PIC_TIMER:
            clock_update();
            outb(PIC_MASTER, PIC_ECHO);
            // vprintf0("timer event\n");
            break;
        case PIC_OFFSET + PIC_KBD:
            kbd_update(inb(0x60));// 必须要取出数据，再发送响应
            outb(PIC_MASTER, PIC_ECHO);
//            vprintf0("kbd event %d\n",data);
            break;
        case PIC_OFFSET+PIC_MOUSE:
            mouse_update(inb(0x60)); // 必须要取出数据，再发送响应
            outb(PIC_SLAVE, PIC_ECHO);
            outb(PIC_MASTER, PIC_ECHO);
//            vprintf0("mouse event %d\n",data);
            break;
        default:
            break;
    }
}