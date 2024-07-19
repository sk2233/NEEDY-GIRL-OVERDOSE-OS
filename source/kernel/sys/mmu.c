#include "sys/mmu.h"
#include "comm/type.h"
#include "comm/x86.h"
#include "lib/stdio.h"
#include "lib/mem.h"
#include "lib/err.h"
#include "sys/pic.h"

static mmu_node_t nodes[MMU_NODE_SIZE]; // 最多分配 1024块
static mmu_node_t *first;

uint8_t rtc_read(uint8_t reg){ // 通过时钟检测内存大小
    outb(IO_RTC, reg);
    return inb(IO_RTC+1);
}

uint16_t mmu_read(uint8_t reg){ // 返回 uint8,需要读取两次
    return rtc_read(reg) | (rtc_read(reg + 1) << 8);
}

mmu_node_t *new_node(uint32_t start,uint32_t size,bool_t used){
    for (int i = 0; i < MMU_NODE_SIZE; ++i) {
        mmu_node_t *node=nodes+i;
        if(node->size==0){
            node->start=start;
            node->size=size;
            node->used=used;
            return node;
        }
    }
    panic("no empty mmu_node");
    return 0;
}

void free_node(mmu_node_t *node){
    mem_set(node,0, sizeof(mmu_node_t));
}

void mmu_init(){
    uint32_t total = ((mmu_read(MMU_EXT16) * 64) + 16 * MMU_K) * MMU_K;
    vprintf0("mem size %dm\n", total / MMU_M);
    // 第 1m已经被占用很多了，就不要再使用了，从第 2m 开始分配
    mem_set(nodes,0, sizeof(nodes));
    first= new_node(MMU_START,total-MMU_START,FALSE);
}

void *mem_alloc(uint32_t size){
    return page_alloc((size+MMU_PAGE-1)/MMU_PAGE);
}

void *page_alloc(uint32_t count){
    if(count==0){
        panic("can't alloc zero mem");
    }
    bool_t flag=irq_save();// 先使用中断控制，防止node多线程竞争
    uint32_t size=count*MMU_PAGE;
    mmu_node_t *node=first;
    while (node){
        if(node->used){
            node=node->next;
            continue;
        }
        if(node->size>size){
            mmu_node_t *next= new_node(node->start+size,node->size-size,FALSE);
            node->size=size;
            node->used=TRUE;
            next->next=node->next;
            next->pre=node;
            node->next=next;
            if(next->next){
                next->next->pre=next;
            }
            irq_restore(flag);
            void *res = (void *)node->start;
            mem_set(res,0,size);
            return res;
        } else if(node->size==size){
            node->used=TRUE;
            irq_restore(flag);
            void *res = (void *)node->start;
            mem_set(res,0,size);
            return res;
        }
        node=node->next;
    }
    irq_restore(flag);
    panic("no mem to page alloc count %d",count);
    return 0;
}

void print_mmu_info() {
    uint32_t size =0;
    int count=0;
    mmu_node_t *node=first;
    while (node){
        size+=node->size;
        count++;
        node=node->next;
    }
    vprintf0("mmu_node count %d , mmu size %d m\n",count,size/MMU_M);
}

void mem_free(void *mem){
    bool_t flag=irq_save();// 防止多线程竞争
    uint32_t start = (uint32_t)mem;
    mmu_node_t *node=first;
    while (node){
        if(node->used&&node->start==start){
            node->used=FALSE;
            mmu_node_t *next=node->next;
            if(next&&!next->used){
                node->size+=next->size;
                node->next=next->next;
                if(next->next){
                    next->next->pre=node;
                }
                free_node(next);
            }
            mmu_node_t *pre=node->pre;
            if(pre&&!pre->used){
                pre->size+=node->size;
                pre->next=node->next;
                if(node->next){
                    node->next->pre=pre;
                }
                free_node(node);
            }
            irq_restore(flag);
            return;
        }
        node=node->next;
    }
    irq_restore(flag);
    panic("err mem to free %x",start);
}

