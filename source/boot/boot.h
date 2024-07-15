#ifndef OS_BOOT_H
#define OS_BOOT_H

// 定义段的宏
#define SEG_NULL						\
	.word 0, 0;					\
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

#define	LOADER_ADDR	0x10000		// loader加载到的内存地址
#define SECTOR_SIZE	512 // 扇区大小
#define SECTOR_COUNT 256
#define SECTOR_OFFSET 1

#endif
