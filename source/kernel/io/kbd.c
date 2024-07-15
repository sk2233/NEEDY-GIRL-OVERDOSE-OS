#include "io/kbd.h"
#include "sys/pic.h"

void kbd_init(){
    pic_enable(PIC_KBD);
}