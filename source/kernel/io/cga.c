#include "io/cga.h"
#include "comm/x86.h"

static uint16_t curr_pos;
static uint16_t *cga_buff;

void cga_init(){
    cga_buff=(uint16_t *)CGA_BUF;
    // 获取光标坐标，是展开成一纬平铺的
    outb(CGA_BASE, 14); // 写入控制
    curr_pos = (uint16_t)inb(CGA_BASE + 1) <<8; // 读取数据
    outb(CGA_BASE, 15);
    curr_pos  |=(uint16_t)inb(CGA_BASE + 1);
}

void cga_putc(char ch){
    uint16_t data = ch|0x0700; // 使用默认样式
    switch (ch) {
        case '\b':
            if(curr_pos>0){// 删除
                curr_pos--;
                cga_buff[curr_pos]=' '|0x0700;
            }
            break;
        case '\n':
            curr_pos+=CGA_COL;
            curr_pos-=curr_pos%CGA_COL;
            break;
        case '\r':
            curr_pos-=curr_pos%CGA_COL;
            break;
        default:
            cga_buff[curr_pos++]=data;
            break;
    }
    if(curr_pos>=CGA_SIZE){
        curr_pos=0;
    }
    // 写入光标位置
    outb(CGA_BASE,14);
    outb(CGA_BASE+1,(uint8_t)(curr_pos>>8));
    outb(CGA_BASE,15);
    outb(CGA_BASE+1,(uint8_t)(curr_pos));
}