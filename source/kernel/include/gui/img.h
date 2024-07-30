#ifndef OS_IMG_H
#define OS_IMG_H

#include "comm/type.h"
#include "sys/file.h"

#define IMG_SECTOR (0x8000000/FILE_SECTOR_SIZE)

typedef struct img{
    uint32_t w;
    uint32_t h;
    uint32_t size;
    rgb_t *data;
}img_t;

typedef struct img_hdr{
    char name[16];
    uint32_t w;
    uint32_t h;
    uint32_t offset;
}img_hdr_t;

void img_init();
img_t *img_open(file_t *file);
void img_close(img_t *img);
img_t *img_open_raw(const char *name);
img_t *img_create(uint32_t w,uint32_t h);

#endif