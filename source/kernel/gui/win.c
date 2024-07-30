#include "gui/win.h"
#include "lib/err.h"
#include "gui/btn.h"
#include "lib/mem.h"
#include "gui/gui.h"
#include "gui/gui_main.h"
#include "sys/clock.h"
#include "lib/string.h"

// 预加载需要经常使用的素材
static img_t *win_active;
static img_t *win_inactive;
static img_t *win_min;
static img_t *win_max;
static img_t *win_close0;
static img_t *win_live_bg;
static img_t *win_play_icon;
static img_t *win_play_game;
static img_t *win_play_sex;
static img_t *win_play_talk;
static img_t *win_sleep_icon;
static img_t *win_sleep_pm;
static img_t *win_sleep_night;
static img_t *win_sleep_am;
static img_t *win_drug1_base;
static img_t *win_drug2_base;
static img_t *win_drug1_item;
static img_t *win_drug2_item;
static img_t *win_drug3_item;
static img_t *win_net_title;
static img_t *win_net_twitter;
static img_t *win_net_search;
static img_t *win_net_movie;
static img_t *win_net_2ch;
static img_t *win_net_hot;
static img_t *win_net_start;
static img_t *win_out_map;
static img_t *win_task_fan;
static img_t *win_task_stress;
static img_t *win_task_love;
static img_t *win_task_yami;
static img_t *win_twitter_bg;
static img_t *win_twitter_angle;
static img_t *win_twitter_tang;
static img_t *win_twitter_retweet;
static img_t *win_twitter_star;
static img_t *win_line_bg;
static img_t *win_line_head;
static img_t *win_line_msg;
static img_t *win_secret_main;

static window_t wins[8]; // 实际最多只允许 5 个窗口，否则底部放不下
window_t *order_wins[8]; // wins 的有序版本
volatile uint8_t win_count=0;
static uint16_t win_order=0; // 当前最大宽口层级

char *win_names[]={
        [BTN_TYPE_LIVE]="live",
        [BTN_TYPE_PLAY]="play",
        [BTN_TYPE_SLEEP]="sleep",
        [BTN_TYPE_DRUG]="drug",
        [BTN_TYPE_INTERNET]="internet",
        [BTN_TYPE_OUT]="travel",
        [BTN_TYPE_TWITTER]="twitter",
        [BTN_TYPE_LINE]="line",
        [BTN_TYPE_TASK]="task",
        [BTN_TYPE_SECRET]="secret",
};

// 开始绘制的位置是 5，24     12 * 36
int win_ws[]={
        [BTN_TYPE_LIVE]=767+12,
        [BTN_TYPE_PLAY]=36*7+12,
        [BTN_TYPE_SLEEP]=36*7+12,
        [BTN_TYPE_DRUG]=300+12,
        [BTN_TYPE_INTERNET]=127+36*4+12,
        [BTN_TYPE_OUT]=524+12,
        [BTN_TYPE_TASK]=36*4+27+12,
        [BTN_TYPE_TWITTER]=298+36+9*2+12,
        [BTN_TYPE_LINE]=328+12,
        [BTN_TYPE_SECRET]=512+12,
};
int win_hs[]={ // 89, 136, 98      187, 179, 121
        [BTN_TYPE_LIVE]=494+36,
        [BTN_TYPE_PLAY]=36*3+36,
        [BTN_TYPE_SLEEP]=36*3+36,
        [BTN_TYPE_DRUG]=160+36,
        [BTN_TYPE_INTERNET]=26+36*8+36,
        [BTN_TYPE_OUT]=356+36,
        [BTN_TYPE_TASK]=36*5+36,
        [BTN_TYPE_TWITTER]=500+36,
        [BTN_TYPE_LINE]=400+36,
        [BTN_TYPE_SECRET]=544+12,
};

void win_init0(){
    mem_set(wins,0, sizeof(wins));
    mem_set(order_wins,0, sizeof(order_wins));
    win_active= img_open_raw("win_act.png");
    win_inactive= img_open_raw("win_ina.png");
    win_min= img_open_raw("win_min.png");
    win_max= img_open_raw("win_max.png");
    win_close0= img_open_raw("win_close.png");
    win_live_bg= img_open_raw("live_bg.png");
    win_play_icon= img_open_raw("icon_game.png");
    win_play_game= img_open_raw("game_play.png");
    win_play_sex= img_open_raw("game_sex.png");
    win_play_talk= img_open_raw("game_talk.png");
    win_sleep_icon= img_open_raw("icon_sleep.png");
    win_sleep_pm= img_open_raw("sleep_pm.png");
    win_sleep_night= img_open_raw("sleep_night.png");
    win_sleep_am= img_open_raw("sleep_am.png");
    win_drug1_base= img_open_raw("drug1_base.png");
    win_drug2_base= img_open_raw("drug2_base.png");
    win_drug1_item= img_open_raw("drug1_item.png");
    win_drug2_item= img_open_raw("drug2_item.png");
    win_drug3_item= img_open_raw("drug3.png");
    win_net_title= img_open_raw("net_title.png");
    win_net_twitter= img_open_raw("net_twitter.png");
    win_net_search= img_open_raw("net_search.png");
    win_net_movie= img_open_raw("net_movie.png");
    win_net_2ch= img_open_raw("net_2ch.png");
    win_net_hot= img_open_raw("net_hot.png");
    win_net_start= img_open_raw("net_start.png");
    win_out_map= img_open_raw("map_main.png");
    win_task_fan= img_open_raw("task_fan.png");
    win_task_stress= img_open_raw("task_stress.png");
    win_task_love= img_open_raw("task_love.png");
    win_task_yami= img_open_raw("task_yami.png");
    win_twitter_bg= img_open_raw("twit_bg.png");
    win_twitter_angle= img_open_raw("twit_angle.png");
    win_twitter_tang= img_open_raw("twit_tang.png");
    win_twitter_retweet= img_open_raw("twit_ret.png");
    win_twitter_star= img_open_raw("twit_star.png");
    win_line_bg= img_open_raw("line_bg.png");
    win_line_head= img_open_raw("line_head.png");
    win_line_msg= img_open_raw("line_msg.png");
    win_secret_main= img_open_raw("secret_main.png");
}

window_t *win_find(uint8_t type){
    for (int i = 0; i < 8; ++i) {
        window_t *res=wins+i;
        if(res->type==type){
            return res;
        }
    }
    return NULL;
}

uint16_t win_next_order(){
    return win_order++;
}

void win_sort_order(){
    // 数量较少，采用简单的冒泡排序就行
    for (int i = win_count-1; i >0; --i) {
        for (int j = 0; j < i; ++j) {
            if(order_wins[j]->order<order_wins[j+1]->order){
                window_t *temp=order_wins[j];
                order_wins[j]=order_wins[j+1];
                order_wins[j+1]=temp;
            }
        }
    }
}

void win_refresh(window_t *win){
    win->dirty=FALSE;
    // 先按大小填充背景
    img_t *bg=win_inactive;
    if(win->active){
        bg=win_active;
    } // 先绘制中间
    for (int y = 24; y < win->mask.h; y+=74) {
        for (int x = 38; x < win->mask.w; x+=121) {
            draw_sub_img(win->buff,x,y,bg,38,24,121,74);
        }
    } // 再绘制上下左右
    for (int x = 38; x < win->mask.w-8; x+=121) {
        draw_sub_img(win->buff,x,0,bg,38,0,121,24);
        draw_sub_img(win->buff,x,win->mask.h-13,bg,38,98,121,13);
    }
    for (int y = 24; y < win->mask.h - 13; y+=74) {
        draw_sub_img(win->buff,0,y,bg,0,24,38,74);
        draw_sub_img(win->buff,win->mask.w-8,y,bg,159,24,8,74);
    }// 最后绘制 4 个角
    draw_sub_img(win->buff,0,0,bg,0,0,38,24);
    draw_sub_img(win->buff,win->mask.w-8,0,bg,159,0,8,24);
    draw_sub_img(win->buff,0,win->mask.h-13,bg,0,98,38,13);
    draw_sub_img(win->buff,win->mask.w-8,win->mask.h-13,bg,159,98,8,13);
    // 绘制标题
    rgb_t rgb={.r=71,.g=55, .b=199};
    draw_str(win->buff,19,3,win_names[win->type],&rgb);
    // 绘制按钮
    draw_alpha_img(win->buff,win->mask.w-7-12*3,6,win_min);
    draw_alpha_img(win->buff,win->mask.w-7-12*2,6,win_max);
    draw_alpha_img(win->buff,win->mask.w-7-12, 6, win_close0);
    // 绘制内容
    draw_img(win->buff,5,24,win->content_buff);
    // 补充按钮信息
    win->close.x=win->mask.w-7-12;
    win->close.y=6;
    win->close.w=11;
    win->close.h=11;
}

//=========================LIVE_WIN========================

static char *live_imgs[]={"live_live1.png","live_live2.png","live_live3.png","live_live4.png"
                          ,"live_live5.png","live_live6.png","live_live7.png","live_live8.png"
                          ,"live_live9.png","live_live10.png"};

void win_live_init(window_t *win) {
    mem_copy(win->content_buff->data, win_live_bg->data, win->content_buff->size);
    win->live_mask.x=33;
    win->live_mask.y=79;
    win->live_mask.w=389;
    win->live_mask.h=251;
    // 绘制图片
    win->live_img_idx= tick_count % 10;
    img_t *temp= img_open_raw(live_imgs[win->live_img_idx]);
    draw_img(win->content_buff, 33, 79, temp);
    img_close(temp);
    // 绘制文本
    uint8_t img_idx=tick_count%14;
    rgb_t main={.r=90, .g=75, .b=185};
    draw_str(win->content_buff, 33, 330, live_megs[img_idx], &main);
    rgb_t other={.r=140, .g=136, .b=139};
    draw_str(win->content_buff, 33, 350, "999999999 are watching\x0", &other);
    for (int i = 0; i < 10; ++i) {
        draw_str(win->content_buff, 433, 79 + 3 + i * (18 + 6), live_megs[img_idx + i], &main);
    }
    win->dirty=TRUE;
}

void handle_live_win(window_t *win, point_t *point) {
    if(!point_in_rect(&win->live_mask,point)){
        return;
    }
    win->live_img_idx=(win->live_img_idx+1)%10;
    img_t *temp= img_open_raw(live_imgs[win->live_img_idx]);
    draw_img(win->content_buff, 33, 79, temp);
    img_close(temp);
    win->dirty=TRUE;
    sort_refresh(FALSE);
}

//===================PLAY_WIN=================

void win_play_init(window_t *win) {
    mem_set(win->content_buff->data,0xFF,win->content_buff->size);
    rgb_t clr={.r=77, .g=64, .b=189};
    draw_alpha_img(win->content_buff,0,0,win_play_icon);
    draw_str(win->content_buff,36,9,"play",&clr);
    draw_alpha_img(win->content_buff,36,36,win_play_game);
    draw_str(win->content_buff,36,36*2+9,"game",&clr);
    draw_alpha_img(win->content_buff,36*3,36,win_play_talk);
    draw_str(win->content_buff,36*3,36*2+9,"talk",&clr);
    draw_alpha_img(win->content_buff,36*5,36,win_play_sex);
    draw_str(win->content_buff,36*5,36*2+9,"sex",&clr);
    win->dirty=TRUE;
}

//====================SLEEP_WIN==================

void win_sleep_init(window_t *win) {
    mem_set(win->content_buff->data,0xFF,win->content_buff->size);
    rgb_t clr={.r=77, .g=64, .b=189};
    draw_alpha_img(win->content_buff,0,0,win_sleep_icon);
    draw_str(win->content_buff,36,9,"sleep",&clr);
    draw_alpha_img(win->content_buff,36,36,win_sleep_pm);
    draw_str(win->content_buff,36,36*2+9,"pm",&clr);
    draw_alpha_img(win->content_buff,36*3,36,win_sleep_night);
    draw_str(win->content_buff,36*3,36*2+9,"night",&clr);
    draw_alpha_img(win->content_buff,36*5,36,win_sleep_am);
    draw_str(win->content_buff,36*5,36*2+9,"am",&clr);
    win->dirty=TRUE;
}

//=====================DRUG_WIN====================

static int drug1_pos[]={6+17,16+30,
                        39+17,16+30,
                        4+17,44+30,
                        39+17,44+30,
                        6+17,71+30,
                        39+17,71+30};
static int drug2_pos[]={7+100+15,14+30,
                        39+100+15,14+30,
                        7+100+15,42+30,
                        39+100+15,42+30,
                        7+100+15,70+30,
                        39+100+15,70+30};

void win_update_drug_buff(window_t *win) {
    mem_copy(win->content_buff->data,win->drug_buff->data,win->content_buff->size);
    for (int i = 0; i < 6; ++i) {
        rect_t *temp=&win->drug1_masks[i];
        if(temp->w&&temp->h){
            draw_alpha_img(win->content_buff,temp->x,temp->y,win_drug1_item);
        }
    }
    for (int i = 0; i < 6; ++i) {
        rect_t *temp=&win->drug2_masks[i];
        if(temp->w&&temp->h){
            draw_alpha_img(win->content_buff,temp->x,temp->y,win_drug2_item);
        }
    }
    win->dirty=TRUE;
}

void win_drug_init(window_t *win) {
    win->drug_buff= img_create(win->content_buff->w,win->content_buff->h);
    mem_set(win->drug_buff->data,0xFF,win->content_buff->size);
    draw_alpha_img(win->drug_buff,17,30,win_drug1_base);
    draw_alpha_img(win->drug_buff,100+15,30,win_drug2_base);
    draw_alpha_img(win->drug_buff,200+22,52,win_drug3_item);
    for (int i = 0; i < 6; ++i) {
        win->drug1_masks[i].x=drug1_pos[i*2];
        win->drug1_masks[i].y=drug1_pos[i*2+1];
        win->drug1_masks[i].w=22;
        win->drug1_masks[i].h=20;
    }
    for (int i = 0; i < 6; ++i) {
        win->drug2_masks[i].x=drug2_pos[i*2];
        win->drug2_masks[i].y=drug2_pos[i*2+1];
        win->drug2_masks[i].w=24;
        win->drug2_masks[i].h=14;
    }
    win_update_drug_buff(win);
}

void win_drug_close(window_t *win) {
    img_close(win->drug_buff);
}

void handle_drug_win(window_t *win, point_t *point) {
    for (int i = 0; i < 6; ++i) {
        if(point_in_rect(&win->drug1_masks[i],point)){
            mem_set(&win->drug1_masks[i],0, sizeof(rect_t));
            win_update_drug_buff(win);
            sort_refresh(FALSE);
            return;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if(point_in_rect(&win->drug2_masks[i],point)){
            mem_set(&win->drug2_masks[i],0, sizeof(rect_t));
            win_update_drug_buff(win);
            sort_refresh(FALSE);
            return;
        }
    }
}

//===============NET_WIN===============

void win_net_init(window_t *win) {
    mem_set(win->content_buff->data,0xFF,win->content_buff->size);
    draw_alpha_img(win->content_buff,36*2,36,win_net_title);
    draw_alpha_img(win->content_buff,36,36,win_net_start);
    draw_alpha_img(win->content_buff,127+36*3-26,36,win_net_start);
    rgb_t clr={.r=77, .g=64, .b=189};
    draw_alpha_img(win->content_buff,36,36*2+26,win_net_twitter);
    draw_str(win->content_buff,36*2,36*2+26+9,"____________twitter",&clr);
    draw_alpha_img(win->content_buff,36*2,36*3+26,win_net_search);
    draw_str(win->content_buff,36*3,36*3+26+9,"________search",&clr);
    draw_alpha_img(win->content_buff,36,36*4+26,win_net_movie);
    draw_str(win->content_buff,36*2,36*4+26+9,"____________movie",&clr);
    draw_alpha_img(win->content_buff,36*2,36*5+26,win_net_2ch);
    draw_str(win->content_buff,36*3,36*5+26+9,"________2ch",&clr);
    draw_alpha_img(win->content_buff,36,36*6+26,win_net_hot);
    draw_str(win->content_buff,36*2,36*6+26+9,"____________hot",&clr);
    win->dirty=TRUE;
}

//===================OUT_WIN=================

void win_out_init(window_t *win) {
    draw_img(win->content_buff,0,0,win_out_map);
    win->dirty=TRUE;
}

//==================TASK_WIN=====================

void win_task_init(window_t *win) {
    rgb_t title={.r=107, .g=172, .b=221};
    rgb_t num={.r=77, .g=64, .b=189};
    mem_set(win->content_buff->data,0xFF,win->content_buff->size);
    draw_alpha_img(win->content_buff,18,18+7,win_task_fan);
    draw_str(win->content_buff,18+27,18,"fan",&title);
    draw_str(win->content_buff,18+36+27,18+18,"999999999",&num);
    draw_alpha_img(win->content_buff,18,18+36+7,win_task_stress);
    draw_str(win->content_buff,18+27,18+36,"stress",&title);
    draw_str(win->content_buff,18+36+27,18+36+18,"0/100",&num);
    draw_alpha_img(win->content_buff,18,18+36*2+7,win_task_love);
    draw_str(win->content_buff,18+27,18+36*2,"love",&title);
    draw_str(win->content_buff,18+36+27,18+36*2+18,"100/100",&num);
    draw_alpha_img(win->content_buff,18,18+36*3+7,win_task_yami);
    draw_str(win->content_buff,18+27,18+36*3,"tami",&title);
    draw_str(win->content_buff,18+36+27,18+36*3+18,"0/100",&num);
    win->dirty=TRUE;
}

//===================TWITTER_WIN=======================

#define TWITTER_SCROLL_H 1000

void win_update_twitter_buff(window_t *win) {
    uint16_t h=win->content_buff->h-18; // 重新获取缓存判断
    if(win->twitter_offset<win->twitter_scroll_offset||win->twitter_offset+h>win->twitter_scroll_offset+TWITTER_SCROLL_H){
        mem_set(win->twitter_scroll_buff->data,0xFF,win->twitter_scroll_buff->size);
        win->twitter_scroll_offset=win->twitter_offset-(TWITTER_SCROLL_H-h)/2; // 使原对象位于中间
        rgb_t name_clr={.r=107, .g=172, .b=221};
        rgb_t content_clr={.r=77, .g=64, .b=189};
        rgb_t retweet_clr={.r=97, .g=200, .b=88};
        rgb_t star_clr={.r=222, .g=209, .b=98};
        rgb_t other_clr={.r=140, .g=136, .b=139};
        char buff[64];
        char line[64];
        mem_set(line,'-',37);
        line[37]='\x0';
        for (int i = 0; i < 30; ++i) {
            twitter_item_t *item=twitter_data+i;
            twitter_item_t *next=twitter_data+i+1;
            if(next->offset>win->twitter_scroll_offset&&item->offset<win->twitter_scroll_offset+TWITTER_SCROLL_H){
                int y=item->offset-win->twitter_scroll_offset;
                img_t *head=win_twitter_angle;
                if(str_equal(item->name,"Tang Tang")){
                    head=win_twitter_tang;
                }
                draw_img(win->twitter_scroll_buff,0,y,head);
                draw_str(win->twitter_scroll_buff,36,y,item->name,&name_clr);
                y+=18;
                uint32_t len= str_len(item->content);
                for (int j = 0; j < len; j+=33) {
                    str_sub(item->content,buff,j,j+33);
                    draw_str(win->twitter_scroll_buff,36,y,buff,&content_clr);
                    y+=18;
                }
                if(str_len(item->img)>0){
                    img_t *temp= img_open_raw(item->img);
                    draw_img(win->twitter_scroll_buff,36,y,temp);
                    img_close(temp);
                    y+=177;
                }
                draw_alpha_img(win->twitter_scroll_buff,36,y,win_twitter_retweet);
                draw_str(win->twitter_scroll_buff,36+18,y,item->retweet,&retweet_clr);
                draw_alpha_img(win->twitter_scroll_buff,36+149,y,win_twitter_star);
                draw_str(win->twitter_scroll_buff,36+149+18,y,item->star,&star_clr);
                y+=18;
                draw_str(win->twitter_scroll_buff,0,y,line,&other_clr);
            }
        }
    }
    uint16_t w=win->content_buff->w-18;
    draw_sub_img(win->content_buff,9,9,win->twitter_scroll_buff,
                 0,win->twitter_offset-win->twitter_scroll_offset,w,h);
    win->dirty=TRUE;
}

void win_twitter_init(window_t *win) {
    mem_copy(win->content_buff->data,win_twitter_bg->data,win->content_buff->size);
    win->twitter_scroll_buff= img_create(win->content_buff->w-18,TWITTER_SCROLL_H); // 滚动 Buff 稍微大点
    win->twitter_offset=0;
    win->twitter_scroll_offset=5000; // 保证一定触发重新绘制
    // 初始化数据方便后面使用 初始化时要初始化 31个但是只使用 30个
    uint16_t offset=0;
    for (int i = 0; i < 31; ++i) {
        twitter_item_t *item=twitter_data+i;
        item->offset=offset;
        offset+=18+18+18;
        offset+= ((str_len(item->content)+32)/33)*18;
        if(str_len(item->img)>0){
            offset+=177;
        }
    }
    win_update_twitter_buff(win);
}

void win_twitter_close(window_t *win) {
    img_close(win->twitter_scroll_buff);
}

void handle_twitter_win(window_t *win, point_t *point) {
    uint16_t h=win->content_buff->h;
    if(point->y<0||point->y>h){
        return;
    }
    if(point->y>h/2){
        if(win->twitter_offset+18+h<=twitter_data[30].offset){
            win->twitter_offset+=18;
            win_update_twitter_buff(win);
            sort_refresh(FALSE);
        }
    } else{
        if(win->twitter_offset-18>=0){
            win->twitter_offset-=18;
            win_update_twitter_buff(win);
            sort_refresh(FALSE);
        }
    }
}

//====================LINE_WIN=======================

static int req_pos[]={36+4+64*0,400-128+4,36+4+64*1,400-128+4,36+4+64*2,400-128+4,36+4+64*3,400-128+4,
                      36+4+64*0,400-128+4+64,36+4+64*1,400-128+4+64,36+4+64*2,400-128+4+64,36+4+64*3,400-128+4+64};
static char *req_imgs[]={"line_req1.png","line_req2.png","line_req3.png","line_req4.png",
                        "line_req5.png","line_req6.png","line_req7.png","line_req8.png"};

typedef struct req_resp{
    uint8_t req; // 下标
    bool_t resp_text; // 是否回复文本
    uint8_t resp; // 文本或表情下标
}req_resp_t;

static req_resp_t req_resp_data[3];
static int req_resp_idx;

void win_line_init(window_t *win) {
    mem_copy(win->content_buff->data,win_line_bg->data,win->content_buff->size);
    rgb_t clr={.r=202, .g=165, .b=222};
    draw_rect(win->content_buff,0,400-128,328,128,&clr);
    for (int i = 0; i < 8; ++i) {
        img_t *temp= img_open_raw(req_imgs[i]);
        draw_alpha_img(win->content_buff,req_pos[i*2],req_pos[i*2+1],temp);
        img_close(temp);
        win->req_masks[i].x=req_pos[i*2];
        win->req_masks[i].y=req_pos[i*2+1];
        win->req_masks[i].w=56;
        win->req_masks[i].h=56;
    }
    mem_set(req_resp_data,0, sizeof(req_resp_data));
    req_resp_idx=0;
    win->dirty=TRUE;
}

static char *resp_texts[]={"A ray of light, illuminating the chaotic internet",
                           "I m gonna save you darling, darling",
                           "Poison radio, its scattered reflection in your mind",
                           "A strong hallucination, INTERNET GIRL",
                           "A downer just for you, NEEDY GIRL",
                           "Light sleep and an overdose on need for approval",
                           "I can see a lie for a lie, my love is going crazy"};
static char *resp_imgs[]={"line_resp1.png","line_resp2.png","line_resp3.png","line_resp4.png",
                          "line_resp5.png","line_resp6.png","line_resp7.png"};

void win_update_line_buff(window_t *win) {
    int idx=req_resp_idx-1;
    draw_sub_img(win->content_buff,18,0,win_line_bg,18,0,292,400-128);
    int y=400-128;
    char buff[32];
    rgb_t clr={.r=73, .g=53, .b=201};
    while (idx>=0){
        req_resp_t *item=req_resp_data+(idx%3);
        if(item->resp_text){
            y-=48;
            draw_alpha_img(win->content_buff,18,y,win_line_head);
            draw_alpha_img(win->content_buff,18+36,y,win_line_msg);
            uint32_t len=str_len(resp_texts[item->resp]);
            int count=0;
            for (int i = 0; i < len; i+=26) {
                str_sub(resp_texts[item->resp],buff,i,i+26);
                draw_str(win->content_buff,18+36+16,y+6+count*18,buff,&clr);
                count++;
            }
        } else{
            y-=56;
            draw_alpha_img(win->content_buff,18,y,win_line_head);
            img_t *temp= img_open_raw(resp_imgs[item->resp]);
            draw_alpha_img(win->content_buff,18+36,y,temp);
            img_close(temp);
        }
        y-=9;
        if(y<=0){
            break;
        }
        y-=56;
        img_t *temp= img_open_raw(req_imgs[item->req]);
        draw_alpha_img(win->content_buff,328-18-56,y,temp);
        img_close(temp);
        y-=9;
        if(y<=0){
            break;
        }
        idx--;
    }
    win->dirty=TRUE;
}

void handle_line_win(window_t *win, point_t *point) {
    for (int i = 0; i < 8; ++i) {
        if(point_in_rect(&win->req_masks[i],point)){
            req_resp_t *item=req_resp_data+(req_resp_idx%3);
            item->req=i;
            uint8_t temp=tick_count%14;
            item->resp_text=temp<7;
            item->resp=temp%7;
            req_resp_idx++;
            win_update_line_buff(win);
            sort_refresh(FALSE);
            return;
        }
    }
}

//================SECRET_WIN=================

static char *secret_text[]={"  Notes on the current P-chan:",
                            "  Profile: My favorite person in the world and also my producer. Tells me what to do so we can turn KAngel into the strongest internet angel. Also supports me emotionally and mentally. I wuv my P-chan!",
                            "  Thoughts: The current P-Chan isn t bad, but I kind of can t deal with people only being nice to me, so. I made things work by myself but it s still not ideal. I ll just make a better P-chan next time. One that s so dreamy that I ll never wake. I think I ll start off with the goal of making marriage material next.",
                            "  I can t wait to live a new life happily married~!"};

void win_secret_init(window_t *win) {
    mem_set(win->content_buff->data,0xFF,win->content_buff->size);
    int y=0;
    char buff[64];
    for (int i = 0; i < 4; ++i) { // 57
        uint32_t len= str_len(secret_text[i]);
        for (int j = 0; j < len;j+=57) {
            str_sub(secret_text[i],buff,j,j+57);
            draw_str(win->content_buff,0,y,buff,0);
            y+=18;
        }
    }
    draw_img(win->content_buff,0,y,win_secret_main);
    win->dirty=TRUE;
}

void win_init(window_t *win){
    switch (win->type) {
        case BTN_TYPE_LIVE:
            win_live_init(win);
            break;
        case BTN_TYPE_PLAY:
            win_play_init(win);
            break;
        case BTN_TYPE_SLEEP:
            win_sleep_init(win);
            break;
        case BTN_TYPE_DRUG:
            win_drug_init(win);
            break;
        case BTN_TYPE_INTERNET:
            win_net_init(win);
            break;
        case BTN_TYPE_OUT:
            win_out_init(win);
            break;
        case BTN_TYPE_TASK:
            win_task_init(win);
            break;
        case BTN_TYPE_TWITTER:
            win_twitter_init(win);
            break;
        case BTN_TYPE_LINE:
            win_line_init(win);
            break;
        case BTN_TYPE_SECRET:
            win_secret_init(win);
            break;
    }
}

void win_close(window_t *win) {
    img_close(win->buff);
    img_close(win->content_buff);
    switch (win->type) {
        case BTN_TYPE_DRUG:
            win_drug_close(win);
            break;
        case BTN_TYPE_TWITTER:
            win_twitter_close(win);
            break;
    }
    mem_set(win,0, sizeof(window_t));
}

void handle_win(window_t *win, point_t *point){
    if(point_in_rect(&win->close,point)){
        // 处理按钮
        int count=0;
        for (int i = 0; i < 16; ++i) {
            button_t *btn=btns+i;
            if(btn->type==win->type+BTN_WIN_OFFSET){ // 对应的移除
                mem_set(btn,0, sizeof(button_t));
            } else if(btn->type>BTN_WIN_OFFSET){ // 其他的调整位置
                btn->mask.x=152+count*112;
                count++;
            }
        }
        // 处理窗口
        win_close(win);
        win_count=0;
        for (int i = 0; i < 8; ++i) {
            win=wins+i;
            if(win->type){
                order_wins[win_count++]=win;
            }
        }
        sort_refresh(TRUE); // 关闭窗口需要 强制刷新
        return;
    }
    // 转换为内容内部的坐标
    point->x-=5;
    point->y-=24;
    switch (win->type) {
        case BTN_TYPE_LIVE:
            handle_live_win(win,point);
            break;
        case BTN_TYPE_DRUG:
            handle_drug_win(win,point);
            break;
        case BTN_TYPE_TWITTER:
            handle_twitter_win(win,point);
            break;
        case BTN_TYPE_LINE:
            handle_line_win(win,point);
            break;
    }
}

window_t *win_alloc(uint8_t type){
    if(win_count>=5){
        return NULL;
    }
    for (int i = 0; i < 8; ++i) {
        window_t *res=wins+i;
        if(!res->type){
            res->type=type;
            res->order= win_next_order();
            res->callback=handle_win;
            order_wins[win_count++]=res;
            return res;
        }
    }
    panic("no win available");
    return NULL;
}
