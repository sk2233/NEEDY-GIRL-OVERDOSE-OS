#ifndef OS_BTN_H
#define OS_BTN_H

#include "lib/collision.h"

#define BTN_TYPE_LIVE 1
#define BTN_TYPE_PLAY 2
#define BTN_TYPE_SLEEP 3
#define BTN_TYPE_DRUG 4
#define BTN_TYPE_INTERNET 5
#define BTN_TYPE_OUT   6

#define BTN_TYPE_TWITTER 7
#define BTN_TYPE_LINE 8
#define BTN_TYPE_TASK 9

#define BTN_TYPE_START 10

#define BTN_TYPE_SECRET 11

// 按钮对应窗口底部tab的按钮
#define BTN_WIN_OFFSET 128

typedef struct button button_t;

struct button{ // 按钮并不一直绘制，只用于管理事件
    uint8_t type;
    rect_t mask;
    void (* callback)(button_t *);
};

extern button_t btns[16];

void btn_init();
button_t *btn_alloc(uint8_t type);

#endif
