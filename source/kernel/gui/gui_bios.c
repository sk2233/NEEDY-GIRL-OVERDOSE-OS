#include "gui/gui_bios.h"
#include "gui/img.h"
#include "lib/mem.h"
#include "gui/gui.h"
#include "sys/clock.h"

static uint16_t x;
static rgb_t clr;

void gui_bios_init(){
    img_t *img= img_open_raw("bios_main.png");
    mem_copy(screen->data,img->data,screen->size);
    img_close(img);
    // 9 18
    mem_set(&clr,0xFF,3);
    draw_str(screen,9,600-18*2,"Booting Windose20",&clr);
    x=18*9;
}


void gui_bios_main(){
    clock_wait(5);
    draw_char(screen,x,600-18*2,'.',&clr);
    x+=9;
    if(x>800-9*2){
        gui_next();
    }
}
