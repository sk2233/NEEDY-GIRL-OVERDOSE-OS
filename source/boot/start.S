#include "boot.h"

.set PROT_MODE_CSEG, 0x8         // 代码段
.set PROT_MODE_DSEG, 0x10        // 数据段
.set CR0_PE_ON,      0x1         // 保护模式标记

    .globl _start
_start:
    .code16                     // 设置为 16bit
    cli                         // 关中断

    // 初始化各种段
    xorw    %ax,%ax             // 设置为 0
    movw    %ax,%ds             // -> Data Segment
    movw    %ax,%es             // -> Extra Segment
    movw    %ax,%ss             // -> Stack Segment

// 启用 A20寻址线保证可以寻址 4G 的内存
//seta20.1:
//    inb     $0x64,%al               // Wait for not busy
//    testb   $0x2,%al
//    jnz     seta20.1
//    movb    $0xd1,%al               // 0xd1 -> port 0x64
//    outb    %al,$0x64
//seta20.2:
//    inb     $0x64,%al               // Wait for not busy
//    testb   $0x2,%al
//    jnz     seta20.2
//    movb    $0xdf,%al               // 0xdf -> port 0x60
//    outb    %al,$0x60

//800x600x24
// 设置显示信息
    movb    $0x4f,%ah
    movb    $0x02,%al
    movw    $0x4115,%bx
    int     $0x10
// 获取显示信息到 0x1000
    movb    $0x4f,%ah
    movb    $0x01,%al
    movw    $0x4115,%cx
    movw    $0x1000,%di
    int     $0x10

    // 进入保护模式
    lgdt    gdtdesc
    movl    %cr0, %eax
    orl     $CR0_PE_ON, %eax
    movl    %eax, %cr0
    // 清除指令流水线 使用的是保护模式下的 段选择子+代码偏移的方式
    ljmp    $PROT_MODE_CSEG, $protcseg

// 指定为 32 位
    .code32
protcseg:
// 初始化寄存器
    movw    $PROT_MODE_DSEG, %ax    // Our data segment selector
    movw    %ax, %ds                // -> DS: Data Segment
    movw    %ax, %es                // -> ES: Extra Segment
    movw    %ax, %fs                // -> FS
    movw    %ax, %gs                // -> GS
    movw    %ax, %ss                // -> SS: Stack Segment

  // 设置栈指针并调用 C 代码
    movl    $_start, %esp
    call boot_main
// 调用失败死循环
spin:
    jmp spin

// 设置 gdt 数据与其描述信息
gdt:
    SEG_NULL				// null seg
    SEG(STA_X|STA_R, 0x0, 0xffffffff)	// code seg
    SEG(STA_W, 0x0, 0xffffffff)	       // data seg

gdtdesc:
    .word   0x17                            // sizeof(gdt) - 1
    .long   gdt                             // address gdt

// 引导段结束在末尾写入 55AA 标记为启动扇区 ax 是执行权限
    .section boot_end, "ax"
    .byte 0x55, 0xaa