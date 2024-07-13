#ifndef BOOT_H
#define BOOT_H

#define	LOADER_ADDR	0x8000		// loader加载到的内存地址
#define SECT_SIZE	512 // 扇区大小

//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned long uint32_t;
//
//static inline void outb(uint16_t port, uint8_t data) {
//    __asm__ __volatile__("outb %[v], %[p]" : : [p]"d" (port), [v]"a" (data));
//}
//
//static inline uint8_t inb(uint16_t  port) {
//    uint8_t rv;
//    __asm__ __volatile__("inb %[p], %[v]" : [v]"=a" (rv) : [p]"d"(port));
//    return rv;
//}
//
//static inline void insl(uint32_t port, void *addr, int cnt){
//    __asm__ __volatile__ (
//            "cld;"
//            "repne; insl;"
//            : "=D" (addr), "=c" (cnt)
//            : "d" (port), "0" (addr), "1" (cnt)
//            : "memory", "cc");
//}

#endif
