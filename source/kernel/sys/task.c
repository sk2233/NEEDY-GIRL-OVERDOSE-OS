#include "sys/task.h"
#include "sys/mmu.h"
#include "lib/mem.h"
#include "comm/x86.h"

static seg_desc_t seg_descs[TASK_GDT_COUNT];
static int seg_desc_idx;

void set_seg_desc(seg_desc_t *seg, uint32_t limit, uint32_t base, uint16_t attr){
    if (limit > 0xfffff) {
        attr |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    seg->limit_low    = limit & 0xffff;
    seg->base_low     = base & 0xffff;
    seg->base_mid     = (base >> 16) & 0xff;
    seg->access_right = attr & 0xff;
    seg->limit_high   = ((limit >> 16) & 0x0f) | ((attr >> 8) & 0xf0);
    seg->base_high    = (base >> 24) & 0xff;
}

void task_init(){
    // 多进程必须依赖 gdt 汇编中的设置不方便修改，这里再设置一下，在这里添加多进程
    mem_set(seg_descs,0, sizeof(seg_descs));
    set_seg_desc(seg_descs, 0, 0, 0);// 若需要添加段注意总数量 注意这里是先代码段再数据段
    set_seg_desc(seg_descs + 1, 0xffffffff, 0x00000000, 0x409a);// 仅可执行的段 代码段
    set_seg_desc(seg_descs + 2, 0xffffffff, 0x00000000, 0x4092);// 读写段 数据段
    lgdt((uint32_t)seg_descs, sizeof(seg_descs));
    // 从主进程切换到其他进程 cpu 会保存退出进程（主进程）的信息，需要为主进程分配 tss 结构
    tss_t *main= mem_alloc(sizeof(tss_t));
    main->ldtr=0;
    main->iomap=0x40000000;
    set_seg_desc(seg_descs+3,103,(uint32_t)main, TASK_ATTR_TSS);
    write_tr(3*8);// 为cpu设置当前任务默认值
    seg_desc_idx=4; // 后面的从 4 开始
}

uint32_t task_create(void (*func)()){
    tss_t *tss= mem_alloc(sizeof(tss_t)); // 分配 tss 结构体
    tss->ldtr=0;
    tss->iomap=0x40000000;
    tss->eip=(uint32_t)func;
    tss->esp=(uint32_t)(page_alloc(1)+MMU_PAGE); // 新分配的栈空间，默认指向高地址
    tss->eflags=0x00000202;
    // 直接使用系统 数据/代码 段地址
    tss->es = 2 * 8;
    tss->cs = 1 * 8;
    tss->ss = 2 * 8;
    tss->ds = 2 * 8;
    tss->fs = 2 * 8;
    tss->gs = 2 * 8;
    set_seg_desc(seg_descs+seg_desc_idx,103,(uint32_t)tss,TASK_ATTR_TSS);
    seg_desc_idx++;
    return (seg_desc_idx-1)*8;
}

void task_switch(uint32_t task_sel){ // 跳转到对应的 gdt 就会进行执行
    far_jump(task_sel,0); // 任务再切换回来还会从这里回来
}