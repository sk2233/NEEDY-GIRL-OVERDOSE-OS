#include "kernel.h"
#include "io/cga.h"
#include "io/kbd.h"
#include "lib/stdio.h"
#include "sys/mmu.h"
#include "lib/err.h"
#include "sys/idt.h"
#include "sys/pic.h"
#include "sys/clock.h"
#include "comm/x86.h"
#include "sys/task.h"
#include "sys/file.h"
#include "gui/gui.h"
#include "io/mouse.h"
#include "gui/img.h"

void test_task(){
    uint32_t num=0;
    while (TRUE){
        num++;
    }
}

void kernel_main() {
    cga_init();
    mmu_init();
    pic_init();
    idt_init();
    kbd_init();
    mouse_init();
    clock_init();
    file_init();
    img_init();
    gui_init();
    sti(); // 打开中断
    gui_run();
    // 暂时不使用多进程
//    task_init();
//    uint32_t task_sel= task_create(test_task);
//    task_switch(task_sel);
//    vprintf0("hello world %d\n",2233);
//    while (TRUE){
//        hlt();
//    }
//    panic("not support err");
}
