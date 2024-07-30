#include "gui/btn.h"
#include "lib/err.h"
#include "gui/win.h"
#include "gui/gui_main.h"
#include "lib/mem.h"

button_t btns[16]; // 只用于桌面上的按钮

void btn_init(){
    mem_set(btns,0, sizeof(btns));
}

void handle_btn(button_t *btn){
    if(btn->type>BTN_WIN_OFFSET){ // 点击的是窗口对应的底部 tab
        window_t *res= win_find(btn->type - BTN_WIN_OFFSET);
        if(!res){
            panic("type %d win not find",btn->type-BTN_WIN_OFFSET);
            return;
        }
        res->order= win_next_order(); // 置顶
    } else{ // 创建新窗口按钮
        window_t *res= win_find(btn->type);
        if(res){// 已经存在的更新层级
            res->order= win_next_order();
        } else { // 没有进行新建
            res= win_alloc(btn->type);
            if(!res){ // 不能再创建了
                return;
            }
            res->mask.w=win_ws[btn->type];
            res->mask.h=win_hs[btn->type];
            // x,y自己计算居中
            res->mask.x=(800-res->mask.w)/2;
            res->mask.y=(600-res->mask.h)/2;
            // 初始化本身的 buff与内部内容的 buff
            res->buff= img_create(res->mask.w,res->mask.h);
            res->content_buff= img_create(res->mask.w-12,res->mask.h-36);
            // 设置底部 tab按钮
            button_t *temp= btn_alloc(btn->type + BTN_WIN_OFFSET);
            temp->mask.x=152+(win_count-1)*112;
            temp->mask.y=577;
            temp->mask.w=111;
            temp->mask.h=22;
            win_init(res);
        }
    }
    sort_refresh(FALSE);// 有可能不需要刷新
}

button_t *btn_alloc(uint8_t type){
    for (int i = 0; i < 16; ++i) {
        button_t *res=btns+i;
        if(!res->type){
            res->type=type;
            res->callback=handle_btn;
            return res;
        }
    }
    panic("no btn available");
    return NULL;
}
