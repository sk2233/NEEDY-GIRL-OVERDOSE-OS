#include <stdarg.h>
#include "lib/stdio.h"
#include "lib/string.h"
#include "lib/mem.h"
#include "io/cga.h"

void vprintf0(const char *fmt, ...){
    char buff[64]; // 临时使用
    mem_set(buff,0, sizeof(buff));
    va_list args;
    va_start(args, fmt);
    str_fmt(buff,fmt,args);
    va_end(args);

    int idx=0;
    while (buff[idx]){
        cga_putc(buff[idx]);
        idx++;
    }
}

