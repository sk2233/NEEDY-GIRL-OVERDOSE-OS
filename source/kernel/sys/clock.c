#include "sys/clock.h"
#include "sys/pic.h"
#include "comm/x86.h"

#define CLOCK_MODE           0x040               // 控制端口

#define CLOCK_FREQ      1193182

volatile uint32_t tick_count; // 增加其可见性

void clock_init(){
    outb(CLOCK_MODE + 3, 0x00 | 0x04 | 0x30);
    // 先后两次写入
    uint16_t freq = 100;
    uint16_t val = (CLOCK_FREQ+freq/2)/freq;
    outb(CLOCK_MODE, (uint8_t)(val&0xFF));
    outb(CLOCK_MODE, (uint8_t)(val>>8));
    tick_count = 0;
    pic_enable(PIC_TIMER);
}
