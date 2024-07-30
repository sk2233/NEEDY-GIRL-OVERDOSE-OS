#include "gui/gui_login.h"
#include "gui/img.h"
#include "lib/mem.h"
#include "gui/gui.h"
#include "lib/collision.h"
#include "io/mouse.h"

static rect_t rect;
static img_t *bg;
static img_t *cursor;

void gui_login_init(){
    // 准备背景
    bg= img_create(screen->w,screen->h);
    img_t *img= img_open_raw("login_main.png");
    mem_copy(bg->data,img->data,bg->size);
    img_close(img);
    // 边框 2
    img= img_open_raw("login_icon.png");
    rect.x=450;
    rect.y=(int)(600-img->h-4)/2;
    rect.w=(int)img->w+4;
    rect.h=(int)img->h+4;
    rgb_t rgb={.r=226,.g=170,.b=159};
    draw_rect(bg,rect.x,rect.y,rect.w,rect.h,&rgb);
    draw_img(bg,rect.x+2,rect.y+2,img);
    img_close(img);
    mem_copy(screen->data,bg->data,screen->size);
    // 光标
    cursor= img_open_raw("cursor_main.png");
    mouse_save();
    draw_alpha_img(screen,mouse_x-GUI_CURSOR_X,mouse_y-GUI_CURSOR_Y,cursor);
}

void gui_login_main(){
    if(mouse_dirty()){
        int x=last_mouse_x-GUI_CURSOR_X;
        int y=last_mouse_y-GUI_CURSOR_Y;
        draw_sub_img(screen,x,y,bg,x,y,GUI_CURSOR_SIZE,GUI_CURSOR_SIZE);
        mouse_save();
        draw_alpha_img(screen,mouse_x-GUI_CURSOR_X,mouse_y-GUI_CURSOR_Y,cursor);
    }
    if(mouse_just_press(MOUSE_LEFT)){
        point_t point={.x=mouse_x,.y=mouse_y};
        if(point_in_rect(&rect,&point)){
            gui_next();
            img_close(bg);
            img_close(cursor);
        }
    }
    mouse_refresh();
}