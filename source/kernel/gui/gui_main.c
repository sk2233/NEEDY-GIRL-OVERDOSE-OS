#include "gui/gui_main.h"
#include "gui/img.h"
#include "lib/mem.h"
#include "gui/gui.h"
#include "lib/collision.h"
#include "lib/err.h"
#include "io/mouse.h"
#include "gui/win.h"

// 预加载需要经常使用的素材
static img_t *bg1; // 背景包含所有固定不动的内容
static img_t *bg2; // 在 bg1的基础上添加非激活窗口&窗口对应的底部tab
static img_t *bg3; // 在 bg2的基础上添加激活窗口 也就是除鼠标外的一切
static img_t *cursor; // 鼠标
static img_t *tab_btn;

static bool_t drag=FALSE;
static int drag_off_x;
static int drag_off_y;

void refresh_bg2() {
    mem_copy(bg2->data,bg1->data,bg2->size);
    // 绘制 win
    for (int i = win_count-1; i >0; --i) {
        window_t *win=order_wins[i];
        draw_img(bg2,win->mask.x,win->mask.y,win->buff);
    }
    // 绘制对应的 btn_tab
    rgb_t clr={.r=71,.g=45, .b=199};
    for (int i = 0; i < 16; ++i) {
        button_t *btn=btns+i;
        if(btn->type>BTN_WIN_OFFSET){
            draw_img(bg2, btn->mask.x, btn->mask.y, tab_btn);
            draw_str(bg2,btn->mask.x+21,btn->mask.y+2,win_names[btn->type-BTN_WIN_OFFSET],&clr);
        }
    }
}

void refresh_bg3() {
    mem_copy(bg3->data,bg2->data,bg3->size);
    if(win_count>0){
        window_t *win=order_wins[0];
        draw_img(bg3,win->mask.x,win->mask.y,win->buff);
    }
}

void sort_refresh(bool_t force){
    drag=FALSE; // 取消拖动状态
    win_sort_order(); // 进行排序重绘
    for (int i = 0; i < win_count; ++i) {
        if(order_wins[i]->active!=(i==0)){
            order_wins[i]->active=i==0;
            order_wins[i]->dirty=TRUE;
        }
    }
    bool_t need=FALSE; // 是否检查到状态切换的窗口，若有需要刷新屏幕
    for (int i = 0; i < win_count; ++i) {
        if(order_wins[i]->dirty){
            win_refresh(order_wins[i]);
            need=TRUE;
        }
    }
    if(!need&&!force){
        return;
    }
    refresh_bg2();
    refresh_bg3();
    mem_copy(screen->data,bg3->data,screen->size);
}

img_t *create_bg1() {
    img_t *res= img_create(screen->w,screen->h);
    img_t *temp=img_open_raw("main_bg.png");
    mem_copy(res->data,temp->data,res->size);
    img_close(temp);
    char *imgs1[]={"icon_live.png", "icon_game.png", "icon_sleep.png", "icon_drug.png", "icon_net.png", "icon_map.png"};
    uint8_t types1[]={BTN_TYPE_LIVE, BTN_TYPE_PLAY, BTN_TYPE_SLEEP, BTN_TYPE_DRUG, BTN_TYPE_INTERNET, BTN_TYPE_OUT};
    char *names[]={"live","game","sleep","drug","net","trip"};
    uint16_t x=36;
    uint16_t y=36;
    rgb_t clr={.r=76, .g=55, .b=200};
    for (int i = 0; i < 6; ++i) {
        button_t *btn= btn_alloc(types1[i]);
        btn->mask.x=x;
        btn->mask.y=y;
        btn->mask.w=36;
        btn->mask.h=36;
        temp= img_open_raw(imgs1[i]);
        draw_alpha_img(res,x,y,temp);
        img_close(temp);
        draw_str(res,x,y+36+9,names[i],&clr);
        y+=36*2;
    }
    char *imgs2[]={"tab_bird.png","tab_line.png","tab_task.png"};
    uint8_t types2[]={BTN_TYPE_TWITTER,BTN_TYPE_LINE,BTN_TYPE_TASK};
    x=87+5;
    y=579+2;
    for (int i = 0; i < 3; ++i) {
        button_t *btn= btn_alloc(types2[i]);
        btn->mask.x=x;
        btn->mask.y=y;
        btn->mask.w=13;
        btn->mask.h=13;
        temp= img_open_raw(imgs2[i]);
        draw_alpha_img(res,x,y,temp);
        img_close(temp);
        x+=13+5;
    }
//    button_t *btn= btn_alloc(BTN_TYPE_START);
//    btn->mask.x=4;
//    btn->mask.y=579;
//    btn->mask.w=76;
//    btn->mask.h=18;
    temp= img_open_raw("start_btn.png");
    draw_img(res,4,579,temp);
    img_close(temp);
    // secret
    button_t * btn= btn_alloc(BTN_TYPE_SECRET);
    btn->mask.x=360;
    btn->mask.y=288;
    btn->mask.w=36;
    btn->mask.h=36;
    temp= img_open_raw("icon_text.png");
    draw_alpha_img(res,360,288,temp);
    img_close(temp);
    draw_str(res,360,288+36+9,"secret",&clr);
    return res;
}

img_t *create_bg2() {
    img_t *res= img_create(screen->w,screen->h);
    mem_copy(res->data,bg1->data,res->size);
    return res;
}

img_t *create_bg3() {
    img_t *res= img_create(screen->w,screen->h);
    mem_copy(res->data,bg2->data,res->size);
    return res;
}

void gui_main_init(){
    // 初始化数据
    btn_init();
    win_init0();
    // 初始化直接图片
    cursor= img_open_raw("cursor_main.png");
    tab_btn= img_open_raw("tab_btn.png");
    // 初始化间接图片
    bg1= create_bg1();
    bg2= create_bg2();
    bg3=create_bg3();
    mem_copy(screen->data,bg3->data,screen->size);
}

void gui_main_main(){
    if(mouse_just_release(MOUSE_LEFT)){
        drag=FALSE;
        mouse_refresh();
    }
    // 鼠标移动
    if(mouse_dirty()){
        if(drag){
            window_t *win=order_wins[0]; // 能进来就是肯定存在的
            draw_sub_img(bg3,win->mask.x,win->mask.y,bg2,win->mask.x,win->mask.y,win->mask.w,win->mask.h);
            win->mask.x=mouse_x+drag_off_x;
            win->mask.y=mouse_y+drag_off_y;
            draw_img(bg3,win->mask.x,win->mask.y,win->buff);
            mem_copy(screen->data,bg3->data,screen->size);
        }
        int x=last_mouse_x-GUI_CURSOR_X;
        int y=last_mouse_y-GUI_CURSOR_Y;
        draw_sub_img(screen,x,y,bg3,x,y,GUI_CURSOR_SIZE,GUI_CURSOR_SIZE);
        mouse_save();
        draw_alpha_img(screen,mouse_x-GUI_CURSOR_X,mouse_y-GUI_CURSOR_Y,cursor);
    }
    // 鼠标点击
    if(mouse_just_press(MOUSE_LEFT)){
        point_t point={.x=mouse_x,.y=mouse_y};
        // 处理按钮点击
        for (int i = 0; i < 16; ++i) {
            button_t *btn=btns+i;
            if(btn->type&& point_in_rect(&btn->mask,&point)){
                btn->callback(btn);
                mouse_refresh();
                return; // 处理完毕
            }
        }// 处理窗体点击
        for (int i = 0; i < win_count; ++i) {
            window_t *win=order_wins[i]; // 这里的都是有效的
            if(point_in_rect(&win->mask,&point)){
                if(win->active){
                    drag=TRUE;
                    drag_off_x=win->mask.x-point.x;
                    drag_off_y=win->mask.y-point.y;
                    point_t temp={point.x-win->mask.x,point.y-win->mask.y};
                    win->callback(win,&temp);
                } else{
                    win->order= win_next_order();
                    sort_refresh(FALSE);// 有可能不需要筛选
                }
                mouse_refresh();
                return;
            }
        }
    }
}