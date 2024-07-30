#include "gui/gui_boot.h"
#include "gui/img.h"
#include "lib/mem.h"
#include "sys/clock.h"
#include "gui/gui.h"

void gui_boot_init(){
    img_t *img= img_open_raw("boot_main.png");
    mem_copy(screen->data,img->data,screen->size);
    img_close(img);
}

void gui_boot_main(){
    clock_wait(100);
    gui_next();
}