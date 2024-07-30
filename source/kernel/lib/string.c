#include "lib/string.h"
#include "comm/type.h"
#include "lib/mem.h"

static char *nums="0123456789ABCDEF";

int parse_num(char *dst,int num,int base){
    char temp[32];
    mem_set(temp,0, sizeof(temp));
    int len=0;
    while (num>0){
        temp[len++]=nums[num%base];
        num/=base;
    }
    int l=0;
    int r=len-1;
    while (l<r){
        char mid = temp[l];
        temp[l++]=temp[r];
        temp[r--]=mid;
    }
    mem_copy(dst,temp,len);
    return len;
}

uint32_t str_len(const char *str){
    int len=0;
    while (*str){
        str++;
        len++;
    }
    return len;
}

void str_sub(const char *src,char *dst,int start,int end){
    for (int i = start; i < end; ++i) {
        *dst++=*(src+i);
    }
    *dst='\x0';
}

void str_fmt(char *dst,const char *fmt,va_list args){
    int idx=0;
    bool_t is_fmt=FALSE;
    int num;
    char *str;
    while (fmt[idx]){
        if(is_fmt){
            switch (fmt[idx]) {
                case 'd':
                    num = va_arg(args, int);
                    num= parse_num(dst,num,10);
                    dst+=num;
                    break;
                case 'x':
                    num = va_arg(args, int);
                    num= parse_num(dst,num,16);
                    dst+=num;
                    break;
                case 's':
                    str = va_arg(args, char *);
                    num = (int)str_len(str);
                    mem_copy(dst,str,num);
                    dst+=num;
                    break;
                default:
                    mem_copy(dst,"unknown ",8);
                    dst+=8;
                    *dst++=fmt[idx];
                    break;
            }
            is_fmt=FALSE;
        } else if(fmt[idx]=='%'){
            is_fmt=TRUE;
        } else{
            *dst++=fmt[idx];
        }
        idx++;
    }
}

bool_t str_equal(const char *str1,const char *str2){
    while (*str1 && *str2){
        if(*str1!=*str2){
            return FALSE;
        }
        str1++;
        str2++;
    }
    return (!*str1)&&(!*str2);
}