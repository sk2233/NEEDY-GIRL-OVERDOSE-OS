#include "gui/gui.h"
#include "comm/data.h"
#include "lib/err.h"
#include "comm/math.h"
#include "gui/img.h"
#include "io/kbd.h"
#include "lib/string.h"
#include "io/mouse.h"
#include "comm/x86.h"
#include "gui/gui_bios.h"
#include "gui/gui_boot.h"
#include "gui/gui_login.h"
#include "gui/gui_main.h"
#include "lib/mem.h"
#include "sys/mmu.h"

gui_info_t *gui_info;
img_t *screen; // 不需要关闭
// 简易状态机
static void(* inits[])()={gui_bios_init,gui_boot_init,gui_login_init,gui_main_init};
static void(* mains[])()={gui_bios_main,gui_boot_main,gui_login_main,gui_main_main};
static int8_t step_idx;

void gui_next(){
    step_idx++;
    inits[step_idx]();
}

void gui_run(){
    while (TRUE){
        mains[step_idx]();
    }
//    rgb_t rgb;
//    uint16_t x=0;
//    uint16_t y=0;
//    char buff[16];
//    while (TRUE){
//        if(x!=mouse_x||y!=mouse_y){
//            x=mouse_x;
//            y=mouse_y;
//            mem_set(&rgb,0,3);
//            draw_rect(100,100,100,100,&rgb);
//            mem_set(&rgb,0xFF,3);
//            mem_set(buff,0,16);
//            parse_num(buff,x,10);
//            draw_str(100,100,buff,&rgb);
//            mem_set(buff,0,16);
//            parse_num(buff,y,10);
//            draw_str(100,150,buff,&rgb);
//        }
//        if(mouse_press(MOUSE_LEFT)){
//            draw_rect(mouse_x,mouse_y,10,10,&rgb);
//        }
//        mouse_refresh();
//        if (kbd_just_press(KDB_A)){
//            count++;
//            mem_set(&rgb,0,3);
//            draw_rect(0,0,100,100,&rgb);
//            mem_set(&rgb,0xFF,3);
//            mem_set(buff,0,16);
//            parse_num(buff,count,10);
//            draw_str(0,0,buff,&rgb);
//        }
//       kbd_refresh();
//    }
}

void gui_init(){
    gui_info=(gui_info_t *)(GUI_INFO_ADDR);
    screen= mem_alloc(sizeof(img_t));
    screen->w=gui_info->x_size;
    screen->h=gui_info->y_size;
    screen->size=screen->w*screen->h*3;
    screen->data=(rgb_t *)(gui_info->phy_addr);
    step_idx=-1;
    gui_next();

    // 绘制图片
//    img_t *img= img_open_raw("login.png");
//    mem_copy(screen,img->data,img->size);
//    img_close(img);
//    img_t *img= img_open_raw(0);
//    mem_copy(screen,img->data,img->w*img->h*3);
//    img_close(img);
//
//    // 绘制其他
//   rgb_t rgb;
//   mem_set(&rgb,0, sizeof(rgb_t));
//   rgb.g=0xFF;
//   draw_str(10,10,"HelloWorld",&rgb);
//   draw_mouse(100,100);

    // file_t *file= file_open("screen",FALSE);
    // file_read(file,screen,800*600*3);
//    file_write(file,screen,800*600*3);
    // file_close(file);
}

void draw_mouse(img_t *dst,int x,int y){
    int min_x= max(x,0);
    int min_y= max(y,0);
    int max_x= min(x+MOUSE_W,(int)dst->w);
    int max_y= min(y+MOUSE_H,(int)dst->h);
    for (int i = min_x; i <max_x; ++i) {
        for (int j = min_y; j < max_y; ++j) {
            uint8_t idx=mouse[j-y][i-x];
            if(idx!=0){
                rgb_t *rgb=dst->data+j*dst->w+i;
                if(idx==2){
                    mem_set(rgb,0xFF, sizeof(rgb_t));
                } else {
                    mem_set(rgb,0x00,sizeof(rgb_t));
                }
            }
        }
    }
}

void draw_char(img_t *dst,int x,int y,char ch,rgb_t *rgb){
    uint8_t idx=ch-0x20;
    if(idx>=CHAR_NUM){
        panic("idx out range %d",idx);
    }
    int min_x= max(x,0);
    int min_y= max(y,0);
    int max_x= min(x+CHAR_W,(int)dst->w);
    int max_y= min(y+CHAR_H,(int)dst->h);
    if(min_x>=max_x||min_y>=max_y){
        return;
    }
    for (int i = min_x; i <max_x; ++i) {
        for (int j = min_y; j < max_y; ++j) {
            if(characters[idx][j-y][i-x]){
                rgb_t *temp=dst->data+j*dst->w+i;
                *temp=*rgb;
            }
        }
    }
}

void draw_str(img_t *dst,int x,int y,char *str,rgb_t *rgb){
    while (*str){
        draw_char(dst,x,y,*str,rgb);
        str++;
        x+=CHAR_W;
    }
}

void draw_rect(img_t *dst,int x,int y,int w,int h,rgb_t *rgb){
    int min_x= max(x,0);
    int min_y= max(y,0);
    int max_x= min(x+w,(int)dst->w);
    int max_y= min(y+h,(int)dst->h);
    if(min_x>=max_x||min_y>=max_y){
        return;
    }
    for (int i = min_x; i <max_x; ++i) {
        for (int j = min_y; j < max_y; ++j) {
            rgb_t *temp=dst->data+j*dst->w+i;
            *temp=*rgb;
        }
    }
}

void draw_img(img_t *dst,int x,int y,img_t *img){
    int min_x= max(x,0);
    int min_y= max(y,0);
    int max_x= min((int)(x+img->w),(int)dst->w);
    int max_y= min((int)(y+img->h),(int)dst->h);
    if(min_x>=max_x||min_y>=max_y){
        return;
    }
    int w=max_x-min_x;
    for (int j = min_y; j <max_y; ++j) {
        mem_copy(dst->data+j*dst->w+min_x,img->data+(j-y)*img->w+min_x-x,w* sizeof(rgb_t));
    }
}

void draw_alpha_img(img_t *dst,int x,int y,img_t *img){
    int min_x= max(x,0);
    int min_y= max(y,0);
    int max_x= min((int)(x+img->w),(int)dst->w);
    int max_y= min((int)(y+img->h),(int)dst->h);
    if(min_x>=max_x||min_y>=max_y){
        return;
    }
    for (int j = min_y; j < max_y; ++j) {
        for (int i = min_x; i < max_x; ++i) {
            rgb_t *rgb=img->data+(j-y)*img->w+i-x;
            if(rgb->r>0||rgb->g!=0xFF||rgb->b>0){
                *(dst->data+j*dst->w+i)=*rgb;
            }
        }
    }
}

void draw_sub_img(img_t *dst,int x0,int y0,img_t *img,int x,int y,int w,int h){
    if(x>=(int)img->w||y>=(int)img->h){
        return;
    }
    if(x<0){
        x0-=x;
        w+=x;
        x=0;
    }
    if(y<0){
        y0-=y;
        h+=y;
        y=0;
    }
    w= min(w,(int)(img->w-x));
    h= min(h,(int)(img->h-y));
    int min_x= max(x0,0);
    int min_y= max(y0,0);
    int max_x= min(x0+w,(int)dst->w);
    int max_y= min(y0+h,(int)dst->h);
    if(min_x>=max_x||min_y>=max_y){
        return;
    }
    int l=max_x-min_x;
    for (int j = min_y; j <max_y; ++j) {
        mem_copy(dst->data+j*dst->w+min_x,img->data+(y+j-min_y)*img->w+x,l* sizeof(rgb_t));
    }
}