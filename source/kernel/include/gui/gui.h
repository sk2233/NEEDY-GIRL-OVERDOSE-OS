#ifndef OS_GUI_H
#define OS_GUI_H

#include "comm/type.h"
#include "img.h"

#define GUI_INFO_ADDR 0x1000

#define GUI_CURSOR_X 6
#define GUI_CURSOR_Y 2
#define GUI_CURSOR_SIZE 18

typedef struct gui_info {
    uint8_t unused1[18];
    uint16_t x_size;
    uint16_t y_size;
    uint8_t x_char_size;
    uint8_t y_char_size;
    uint8_t num_of_plane;
    uint8_t bit_per_pixel;
    uint8_t unused2[14];
    uint32_t phy_addr;
    uint8_t unused3[212];
} gui_info_t;

extern img_t *screen;
extern gui_info_t *gui_info;

void gui_init();
void gui_run();
void draw_char(img_t *dst,int x,int y,char ch,rgb_t *rgb);
void draw_str(img_t *dst,int x,int y,char *str,rgb_t *rgb);
void draw_mouse(img_t *dst,int x,int y);
void draw_rect(img_t *dst,int x,int y,int w,int h,rgb_t *rgb);
void draw_img(img_t *dst,int x,int y,img_t *img);
void draw_sub_img(img_t *dst,int x0,int y0,img_t *img,int x,int y,int w,int h);
void draw_alpha_img(img_t *dst,int x,int y,img_t *img);

void gui_next();

#endif
