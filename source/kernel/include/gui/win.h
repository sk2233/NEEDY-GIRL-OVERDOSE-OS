#ifndef OS_WIN_H
#define OS_WIN_H

#include "lib/collision.h"
#include "img.h"

typedef struct window window_t;

struct window{
    uint8_t type;
    rect_t mask; // 总体的 mask 决定时间是否传递进来
    uint16_t order;
    void (* callback)(window_t *,point_t *); // 还要传递 位置信息
    bool_t active;
    img_t *buff;
    rect_t close; // 先不管最小化只管关闭
    img_t *content_buff; // 内部内容的buff
    bool_t dirty;
    // 特定类型才会使用的数据
    // LIVE
    uint8_t live_img_idx;
    rect_t live_mask;
    // DRUG
    img_t *drug_buff;
    rect_t drug1_masks[6]; // 6个药1的位置
    rect_t drug2_masks[6]; // 6个药2的位置
    // TWITTER
    img_t *twitter_scroll_buff;
    int twitter_offset;
    int twitter_scroll_offset;
    // LINE
    rect_t req_masks[8];
};

extern window_t *order_wins[8];
extern volatile uint8_t win_count;
extern char *win_names[];
extern int win_ws[];
extern int win_hs[];

void win_init0();
window_t *win_find(uint8_t type);
uint16_t win_next_order();
window_t *win_alloc(uint8_t type);
void win_sort_order();
void win_init(window_t *win);
void win_refresh(window_t *win);

static char *live_megs[]={"A ray of light\x0",
                          "Bringing happiness to\x0",
                          "A promise of future\x0",
                          "The Internet Angel has\x0",
                          "Leave your society\x0",
                          "Get into a dark room\x0",
                          "It s OK, all your\x0",
                          "Who needs reality, I m\x0",
                          "A mind-boggling amount\x0",
                          "I m gonna save you\x0",
                          "I can see a lie for a\x0",
                          "I m gonna breakdown\x0",
                          "You don t have to think\x0",
                          "It ll all be better\x0",
                          "Poison radio, its\x0",
                          "It s sweeter than drugs\x0",
                          "Like!, Dislike!, it s\x0",
                          "Like!, Dislike!, but\x0",
                          "Like!, Dislike!, it s\x0",
                          "Just keep your eyes on\x0",
                          "Smiling like an angel\x0",
                          "A strong hallucination\x0",
                          "Whispering like a devil\x0",
                          "A downer just for you\x0"};

typedef struct twitter_item{
    uint16_t offset; // 辅助数据
    char name[16];
    char content[64];
    char retweet[8];
    char star[8];
    char img[16];
}twitter_item_t;

static twitter_item_t twitter_data[]={{
      .name="Internet Angle\x0",
      .content="A ray of light, illuminating the chaotic internet of today\x0",
      .retweet="897986\x0",
      .star="348382\x0",
      .img="twit_img1.png\x0",
},{
        .name="Internet Angle\x0",
        .content="Bringing happiness to the otaku floating in this electronic sea\x0",
        .retweet="324432\x0",
        .star="456654\x0",
        .img="\x0",
},{
        .name="Tang Tang\x0",
        .content="A promise of future peace; manic and moody but it s alright\x0",
        .retweet="56456\x0",
        .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="The Internet Angel has descended\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img2.png\x0",
},{
      .name="Internet Angle\x0",
      .content="Leave your society, leave your family, leave your relationships\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="Get into a dark room and bathe in the pale light\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="It s OK, all your fears will be overwritten\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img3.png\x0",
},{
      .name="Internet Angle\x0",
      .content="Who needs reality, I m crazy for you\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="A mind-boggling amount of data on the internet\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="I m gonna save you darling, darling\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img4.png\x0",
},{
      .name="Internet Angle\x0",
      .content="I can see a lie for a lie, my love is going crazy\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="I m gonna breakdown, but still\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="You don t have to think\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img5.png\x0",
},{
      .name="Internet Angle\x0",
      .content="It ll all be better soon\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="Poison radio, its scattered reflection in your mind\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="It s sweeter than drugs\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img6.png\x0",
},{
      .name="Internet Angle\x0",
      .content="Like!, Dislike!, it s poison for you\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="Like!, Dislike!, but you drink it anyway\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="Like!, Dislike!, it s our little secret\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img7.png\x0",
},{
      .name="Internet Angle\x0",
      .content="Just keep your eyes on me\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="Smiling like an angel\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="A strong hallucination, INTERNET GIRL\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img8.png\x0",
},{
      .name="Internet Angle\x0",
      .content="Whispering like a devil\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="A downer just for you, NEEDY GIRL\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="Light sleep and an overdose on need for approval\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img9.png\x0",
},{
      .name="Internet Angle\x0",
      .content="Our very own religion and light, straight into your brain\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="Rougher than ethics, a love that erodes the universe itself\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{
        .name="Internet Angle\x0",
        .content="Embrace the kindness in the poison waves\x0",
        .retweet="897986\x0",
        .star="348382\x0",
        .img="twit_img10.png\x0",
},{
      .name="Internet Angle\x0",
      .content="An upside-down computer and scrappy counseling\x0",
      .retweet="324432\x0",
      .star="456654\x0",
        .img="\x0",
},{
      .name="Tang Tang\x0",
      .content="A dreamy melody played on a sparkling rooftop\x0",
      .retweet="56456\x0",
      .star="34354\x0",
        .img="\x0",
},{ // 哨兵节点
        .name="END\x0",
        .content="END\x0",
        .retweet="END\x0",
        .star="END\x0",
        .img="\x0",
}};

#endif
