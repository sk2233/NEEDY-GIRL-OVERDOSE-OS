#ifndef OS_CGA_H
#define OS_CGA_H

// CGA 控制指令基地址
#define CGA_BASE        0x3D4
// CGA 显示地址
#define CGA_BUF         0xB8000
#define CGA_ROW        25
#define CGA_COL        80
#define CGA_SIZE       (CGA_ROW*CGA_COL)

void cga_init();
void cga_putc(char ch);

#endif
