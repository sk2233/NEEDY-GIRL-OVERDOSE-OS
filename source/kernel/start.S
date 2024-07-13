#include "kernel.h"

	// 32位保护模式下的代码
	.globl start
	.code32
start:
	call kernel_main
