#ifndef OS_STRING_H
#define OS_STRING_H

#include <stdarg.h>
#include "comm/type.h"

void str_fmt(char *dst, const char *fmt,va_list args);
bool_t str_equal(const char *str1,const char *str2);
uint32_t str_len(const char *str);
void str_sub(const char *src,char *dst,int start,int end);
int parse_num(char *dst,int num,int base);

#endif
