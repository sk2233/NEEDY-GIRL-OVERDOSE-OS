#ifndef OS_FILE_H
#define OS_FILE_H

#include "comm/type.h"

#define FILE_ITEM_COUNT 256 // 最多支持 256 个文件
#define FILE_OFFSET    512  // 从磁盘的 512 个扇区开始创建文件系统，前面的都被占用了
// 512 ～ 520 文件头描述信息 1个块  520 ~ 776 文件索引表   32个块   776 ～   具体文件内容  下标从 33开始
#define FILE_SECTOR_SIZE	512 // 扇区大小
#define FILE_CHUNK_COUNT 8
#define FILE_CHUNK_SIZE (FILE_SECTOR_SIZE*FILE_CHUNK_COUNT)  // 一个块 4k
#define FILE_IDX_EMPTY  0  // 可以被使用的
#define FILE_IDX_END    1  // 结束于该扇区
#define FILE_INVALID_OFFSET 0xFFFFFFFF

typedef struct file_item{ // 16byte 储存在磁盘的结构
    char name[10];  // 文件名
    uint16_t start; // 开始的节点
    uint32_t size;  // 文件大小
}file_item_t;

typedef struct file{
    char name[10];
    uint16_t start;
    uint32_t size;
    uint16_t file_idx;
    uint32_t pos;
    uint32_t offset; // buff 的偏移
    char buff[FILE_CHUNK_SIZE]; // 一个块大小的缓存
    bool_t dirty;
}file_t;

//typedef struct img_hdr{
//    uint32_t count;
//    uint32_t data[0xFF];
//}img_hdr_t;

void file_init();

file_t *file_open(const char *name,bool_t create); // 打开指定文件,并是否在不存在时进行创建
void file_close(file_t *file);
uint32_t file_write(file_t *file,void *buff,uint32_t size);
uint32_t file_read(file_t *file,void *buff,uint32_t size);
void file_seek(file_t *file,uint32_t pos); // 绝对定位
void file_flush(file_t *file);
void file_mv(file_t *file,const char *name);
void file_rm(file_t *file);

void read_disk(uint32_t sector,uint32_t count,void *dst);

#endif
