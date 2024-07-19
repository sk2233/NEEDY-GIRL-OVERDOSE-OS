#include "boot.h"
#include "comm/x86.h"

void wait_disk(){
    while (inb(0x1F7) & 0x80); // 等待空闲
}

// 这里没必要通用，临时用用
void read_sector(uint16_t *dst){
    wait_disk();
    outb(0x1F6, (uint8_t) 0xE0); // 使用 lba模式加载

    outb(0x1F2, (uint8_t) (SECTOR_COUNT >> 8));
    outb(0x1F3, (uint8_t) (SECTOR_OFFSET >> 24));		// LBA参数的24~31位
    outb(0x1F4, (uint8_t) (0));					// LBA参数的32~39位
    outb(0x1F5, (uint8_t) (0));					// LBA参数的40~47位

    outb(0x1F2, (uint8_t) SECTOR_COUNT);
    outb(0x1F3, (uint8_t) SECTOR_OFFSET);			// LBA参数的0~7位
    outb(0x1F4, (uint8_t) (SECTOR_OFFSET >> 8));		// LBA参数的8~15位
    outb(0x1F5, (uint8_t) (SECTOR_OFFSET >> 16));		// LBA参数的16~23位

    outb(0x1F7, 0x24);
    // 读取并将数据写入到缓存中
    for (int i = 0; i < SECTOR_COUNT; ++i) {
        wait_disk(); // 每获取一个扇区的数据必须等一下
        for (int j = 0; j < SECTOR_SIZE / 2; j++) {
            *dst++ = inw(0x1F0);
        }
    }
}

void boot_main() {
    read_sector((uint16_t *)LOADER_ADDR);
    ((void (*)(void))(LOADER_ADDR))();
}

