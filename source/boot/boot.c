#include "boot.h"
#include "x86.h"

void wait_disk(void)
{
    // 阻塞等待磁盘读取完毕
    while ((inb(0x1F7) & 0xC0) != 0x40);
}

void readsect(void *dst, uint32_t offset)
{
    wait_disk();
    // 使用 lba模式读取磁盘
    outb(0x1F2, 1);		// count = 1
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20);	// 读取指令
    wait_disk();
    // 拷贝内存
    insl(0x1F0, dst, SECT_SIZE/4);
}

void readseg(uint32_t pa, uint32_t count, uint32_t offset)
{
    uint32_t end_pa;
    end_pa = pa + count;
    // round down to sector boundary
    pa &= ~(SECT_SIZE - 1);
    // translate from bytes to sectors, and kernel starts at sector 1
    offset = (offset / SECT_SIZE) + 1;
    // If this is too slow, we could read lots of sectors at a time.
    // We'd write more to memory than asked, but it doesn't matter --
    // we load in increasing order.
    while (pa < end_pa) {
        // Since we haven't enabled paging yet and we're using
        // an identity segment mapping (see boot.S), we can
        // use physical addresses directly.  This won't be the
        // case once JOS enables the MMU.
        readsect((uint8_t*) pa, offset);
        pa += SECT_SIZE;
        offset++;
    }
}

void boot_main() {
	int num=0;
	for(;;){
		num++;
	}
}

