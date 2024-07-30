#include "io/mouse.h"
#include "comm/x86.h"
#include "sys/pic.h"
#include "comm/math.h"
#include "gui/gui.h"
#include "lib/mem.h"
#include "lib/string.h"

void mouse_wait(bool_t write){
    uint32_t time_out = 100000;
    if (write) {
        while (--time_out) {
            if ((inb(0x64) & 2) == 0)
                return;
        }
    } else {
        while (--time_out) {
            if ((inb(0x64) & 1) == 1)
                return;
        }
    }
}

void mouse_write(uint8_t data){
    mouse_wait(TRUE);
    outb(0x64, 0xd4);
    mouse_wait(TRUE);
    outb(0x60, data);
}

uint32_t mouse_read(){
    mouse_wait(FALSE);
    return inb(0x60);
}

static uint8_t mouse_state;
static uint8_t mouse_buff[3];
volatile uint16_t last_mouse_x;
volatile uint16_t last_mouse_y;
volatile uint16_t mouse_x; // 增加其可见性
volatile uint16_t mouse_y;
static uint8_t pre_key;
static uint8_t curr_key;

bool_t mouse_press(uint8_t key){
    return (pre_key&key)&&(curr_key&key);
}

bool_t mouse_just_press(uint8_t key){
    return (!(pre_key&key))&&(curr_key&key);
}

bool_t mouse_just_release(uint8_t key){
    return (pre_key&key)&&(!(curr_key&key));
}

void mouse_refresh(){
    pre_key=curr_key;
}

bool_t mouse_dirty(){
    return last_mouse_x!=mouse_x||last_mouse_y!=mouse_y;
}

void mouse_save(){
    last_mouse_x=mouse_x;
    last_mouse_y=mouse_y;
}

void mouse_init(){
    //激活鼠标接口
    mouse_wait(TRUE);
    outb(0x64, 0xa8);
    //激活中断
    mouse_wait(TRUE);
    outb(0x64, 0x20);
    mouse_wait(FALSE);
    uint8_t temp = (inb(0x60) | 2);
    mouse_wait(FALSE);
    outb(0x64, 0x60);
    mouse_wait(TRUE);
    outb(0x60, temp);
    //设置鼠标为默认设置
    mouse_write(0xf6);
    mouse_read();
    //设置鼠标采样率
    mouse_write(0xf3);
    mouse_read();
    mouse_write(10);
    mouse_read();
    mouse_write(0xf4);
    mouse_read();
    // 启用中断  这里是最重要的，上面不设置也行
    pic_enable(PIC_MOUSE);
    mouse_state=0xFF;
    mouse_x=400;
    mouse_y=300;
}

//static int count=0;

void mouse_update(uint8_t data){
    if(mouse_state==0xFF){// 没有激活等待激活
        if(data==0xfa){
            mouse_state=0;
        }
    } else{// 激活了收集数据 3个一组
        if(mouse_state==0&&(data&0xc8)!=0x08){
            return; // 第一个字节主要记录键位信息 需要保证其合法性  鼠标信息可能断连导致数据错位使用这个校验
        }
        mouse_buff[mouse_state++]=data;
        if(mouse_state>=3){// 开始解析数据
            mouse_state=0;
            curr_key = mouse_buff[0] & 0x07; // 只放在了低3位
//            rgb_t rgb;
//            mem_set(&rgb,0,3);
//            draw_rect(100,100,100,100,&rgb);
//            mem_set(&rgb,0xFF,3);
//            char buff[16];
//            mem_set(buff,0,16);
//            parse_num(buff,curr_key,10);
//            draw_str(100,100,buff,&rgb);
//            count++;
//            mem_set(buff,0,16);
//            parse_num(buff,count,10);
//            draw_str(100,150,buff,&rgb);
            int off_x = mouse_buff[1];
            int off_y = mouse_buff[2];
            // 处理负数
            if ((mouse_buff[0] & 0x10) != 0) {
                off_x |= 0xffffff00;
            }
            if ((mouse_buff[0] & 0x20) != 0) {
                off_y |= 0xffffff00;
            }
            /* 鼠标的y方向与画面符号相反 */
            off_y = - off_y;
            mouse_x= clump(mouse_x+off_x, 0, 800);
            mouse_y=clump(mouse_y+off_y, 0, 600);
        }
    }
}