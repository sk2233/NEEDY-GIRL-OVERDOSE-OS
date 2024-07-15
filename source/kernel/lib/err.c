#include <stdarg.h>
#include "lib/err.h"
#include "lib/mem.h"
#include "lib/string.h"
#include "io/cga.h"

void panic(const char *fmt, ...){
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
    while (TRUE);
}