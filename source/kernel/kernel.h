#ifndef OS_KERNEL_H
#define OS_KERNEL_H

#define PG_SIZE		4096		// bytes mapped by a page
#define PG_SHIFT		12		// log2(PGSIZE)

#define STACK_SIZE	(8*PG_SIZE)   		// size of a kernel stack

// 没有错误码的中断处理函数，需要压入一个 0 使后续处理流程一致
#define TRAP_HANDLER_NO_CODE(name, num)					\
	.globl name			;				\
	.type name, @function	;					\
	.align 2			;				\
name:								\
    pushl $0			;				\
    pushl $(num)	;						\
    pushl %ds  ;   \
    pushl %es ;\
    pushl %fs ;\
    pushl %gs ;\
    pushal ;\
    movl $16, %eax ;\
    movw %ax, %ds ;\
    movw %ax, %es ;\
    pushl %esp ;\
    call handler ;\
    popl %esp ;                             \
    popal ;\
    popl %gs ;\
    popl %fs ;\
    popl %es ;\
    popl %ds ;\
    addl $0x8, %esp ;\
    iret

// 有错误码的中断处理函数
#define TRAP_HANDLER(name, num)						\
	.globl name			;\
	.type name, @function	;		\
	.align 2			;\
name:				\
	pushl $(num)		;					\
	pushl %ds     ;\
    pushl %es ;\
    pushl %fs ;\
    pushl %gs ;\
    pushal ;\
    movl $16, %eax ;\
    movw %ax, %ds ;\
    movw %ax, %es ;\
    pushl %esp ;\
    call handler ;\
    popl %esp ;                      \
    popal ;\
    popl %gs ;\
    popl %fs ;\
    popl %es ;\
    popl %ds ;\
    addl $0x8, %esp ;\
    iret

#endif
