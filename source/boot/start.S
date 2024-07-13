// 定义段的宏
#define SEG_NULL						\
	.word 0, 0;						\
	.byte 0, 0, 0, 0
#define SEG(type,base,lim)					\
	.word (((lim) >> 12) & 0xffff), ((base) & 0xffff);	\
	.byte (((base) >> 16) & 0xff), (0x90 | (type)),		\
		(0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

// 段属性宏
#define STA_X		0x8	    // Executable segment
#define STA_E		0x4	    // Expand down (non-executable segments)
#define STA_C		0x4	    // Conforming code segment (executable only)
#define STA_W		0x2	    // Writeable (non-executable segments)
#define STA_R		0x2	    // Readable (executable segments)
#define STA_A		0x1	    // Accessed

.set PROT_MODE_CSEG, 0x8         // 代码段选择子
.set PROT_MODE_DSEG, 0x10        // 数据段选择子
.set CR0_PE_ON,      0x1         // 保护模式标记

.globl start
start:
  .code16                     // 设置为 16bit
  cli                         // 关中断
  cld                         // 清除方向指令

  // 初始化各种段
  xorw    %ax,%ax             // 设置为 0
  movw    %ax,%ds             // -> Data Segment
  movw    %ax,%es             // -> Extra Segment
  movw    %ax,%ss             // -> Stack Segment

// 实模式下打印 hello world
//  movw    $0xb800,%ax
//  movw    %ax,%es
//  movw    $msg1,%si
//  movw    $0xbe2,%di
//  movw    $24,%cx
//  rep     movsb   // 一种循环指令
//  movw    $str,%si
//  movw    $0xc04,%di
//  movw    $26,%cx
//  rep     movsb

// 启用 A20寻址线保证可以寻址 4G 的内存
seta20.1:
  inb     $0x64,%al               // Wait for not busy
  testb   $0x2,%al
  jnz     seta20.1
  movb    $0xd1,%al               // 0xd1 -> port 0x64
  outb    %al,$0x64
seta20.2:
  inb     $0x64,%al               // Wait for not busy
  testb   $0x2,%al
  jnz     seta20.2
  movb    $0xdf,%al               // 0xdf -> port 0x60
  outb    %al,$0x60

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

// 打印 hello world
//  movl $msg2,%esi
//  movl $0xb8d22,%edi
//  movl $60,%ecx
//  rep movsb
  // 设置栈指针并调用 C 代码
  movl    $start, %esp
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
// 静态数据
//msg1:
//  .byte 'i',0x7,'n',0x7,' ',0x7,'r',0x7,'e',0x7,'a',0x7,'l',0x7,' ',0x7,'m',0x7,'o',0x7,'d',0x7,'e',0x7
//msg2:
//  .byte 'i',0x7,'n',0x7,' ',0x7,'p',0x7,'r',0x7,'o',0x7,'t',0x7, 'e',0x7,'c',0x7,'t',0x7, 'e',0x7,'d',0x7,' ',0x7,'m',0x7,'o',0x7,'d',0x7, 'e',0x7
//str:
//  .byte ':',0xc,' ',0xc,'h',0xc,'e',0xc,'l',0xc,'l',0xc,'o',0xc,' ',0xc,'w',0xc,'o',0xc,'r',0xc,'l',0xc,'d',0xc

// 引导段结束在末尾写入 55AA 标记为启动扇区 ax 是执行权限
  .section boot_end, "ax"
  .byte 0x55, 0xaa