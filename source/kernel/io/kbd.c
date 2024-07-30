#include "io/kbd.h"
#include "sys/pic.h"
#include "gui/gui.h"
#include "lib/string.h"
#include "lib/mem.h"

void kbd_init(){
    pic_enable(PIC_KBD);
}

static uint8_t pre_keys[51]; // 只有部分有用
static uint8_t curr_keys[51];

// q 16 144    p   25 153
// a 30 158    l   38 166
// z 44 172    m   50 178

void kbd_update(uint8_t data){
    if(data<51){
        curr_keys[data]=TRUE;
    } else if(data-128<51){
        curr_keys[data-128]=FALSE;
    }
}

bool_t kbd_press(uint8_t key){
    return pre_keys[key]&&curr_keys[key];
}

bool_t kbd_just_press(uint8_t key){
    return (!pre_keys[key])&&curr_keys[key];
}

bool_t kbd_just_release(uint8_t key){
    return pre_keys[key]&&(!curr_keys[key]);
}

void kbd_refresh(){
    mem_copy(pre_keys,curr_keys,51);
}
