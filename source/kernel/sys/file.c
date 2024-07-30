#include "sys/file.h"
#include "comm/x86.h"
#include "lib/mem.h"
#include "sys/mmu.h"
#include "lib/string.h"
#include "lib/err.h"

void wait_disk(){
    while (inb(0x1F7) & 0x80); // 等待空闲
}

void disk_cmd(uint32_t sector,uint32_t count,uint8_t cmd){
    outb(0x1F6, (uint8_t) 0xE0);

    outb(0x1F2, (uint8_t) (count >> 8));
    outb(0x1F3, (uint8_t) (sector >> 24));		// LBA参数的24~31位
    outb(0x1F4, (uint8_t) (0));					// LBA参数的32~39位
    outb(0x1F5, (uint8_t) (0));					// LBA参数的40~47位

    outb(0x1F2, (uint8_t) count);
    outb(0x1F3, (uint8_t) sector);			// LBA参数的0~7位
    outb(0x1F4, (uint8_t) (sector >> 8));		// LBA参数的8~15位
    outb(0x1F5, (uint8_t) (sector >> 16));		// LBA参数的16~23位

    outb(0x1F7, cmd);
}

void read_disk(uint32_t sector,uint32_t count,void *dst){ // sector 可以当做下标来用
    wait_disk();
    disk_cmd(sector,count,0x24);

    // 读取并将数据写入到缓存中
    uint16_t *buff = dst;
    for (int i = 0; i < count; ++i) {
        wait_disk(); // 每获取一个扇区的数据必须等一下
        for (int j = 0; j < FILE_SECTOR_SIZE / 2; j++) {
            *buff++ = inw(0x1F0);
        }
    }
}

void write_disk(uint32_t sector, uint32_t count, void *src) { // sector 可以当做下标来用
    wait_disk();
    disk_cmd(sector,count,0x34);

    // 写入数据
    uint16_t *buff = src;
    for (int i = 0; i < count; ++i) {
        wait_disk(); // 每写入一个扇区的数据必须等一下
        for (int j = 0; j < FILE_SECTOR_SIZE / 2; j++) {
            outw(0x1F0,*buff++);
        }
    }
}

void init_file_sys(){ // 恢复文件系统
    uint16_t *temp= mem_alloc(FILE_CHUNK_SIZE);
    mem_set(temp,0,FILE_CHUNK_SIZE);
    // 清除文件信息与文件索引信息
    for (int i = 0; i < (1+32); ++i) {
        write_disk(FILE_OFFSET+i*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,temp);
    }
    for (int i = 0; i < (1+32); ++i) {
        temp[i]=FILE_IDX_END; // 前 33块已经被占用了
    }
    write_disk(FILE_OFFSET+FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,temp);
    mem_free(temp);
}

//typedef struct img0{
//    char name[16];
//    uint32_t w;
//    uint32_t h;
//}img0_t;
//
//void load_img(uint32_t offset) {
//    uint32_t addr=FILE_IMG_ADDR+offset;
//    void *buff= page_alloc(16);
//    uint32_t sector=addr/(FILE_CHUNK_SIZE*16)*FILE_CHUNK_COUNT*16;
//    read_disk(sector,FILE_CHUNK_COUNT*16,buff);
//    img0_t *img= buff+addr%(FILE_CHUNK_SIZE*16);
//    uint32_t size=img->w*img->h*3+4+4;
//    file_t *file= file_open(img->name,TRUE);
//
//    offset = addr%(FILE_CHUNK_SIZE*16)+16;
//    uint32_t last=FILE_CHUNK_SIZE*16-offset;
//    sector+=FILE_CHUNK_COUNT*16;
//    while (size>0){
//        if(size>last){
//            file_write(file,buff+offset,last);
//            size-=last;
//        } else{
//            file_write(file,buff+offset,size);
//            size=0;
//        }
//        read_disk(sector,FILE_CHUNK_COUNT*16,buff);
//        sector+=FILE_CHUNK_COUNT*16;
//        last=FILE_CHUNK_SIZE*16;
//        offset=0;
//    }
//    file_close(file);
//    mem_free(buff);
//}
//
//void load_imgs(){
//    img_hdr_t *img_hdr= page_alloc(1);
//    read_disk(FILE_IMG_ADDR/FILE_SECTOR_SIZE,FILE_CHUNK_COUNT,img_hdr);
//    for (int i = 0; i < img_hdr->count; ++i) {
//        load_img(img_hdr->data[i]);
//    }
//    mem_free(img_hdr);
//}

// 1个 4k 块 立即加载
static file_item_t file_items[FILE_ITEM_COUNT];
// 32个 4k 块  延迟加载 每个存储  2048个 uint16
static uint16_t* chunk_idx[32];

void file_init(){
//    init_file_sys();
    // 先加载文件头信息
    read_disk(FILE_OFFSET, FILE_CHUNK_COUNT,file_items);
    mem_set(chunk_idx, 0, sizeof(chunk_idx));
//    file_t *file= file_open("test",TRUE);
//    char *buff= page_alloc(1);
//    file_read(file,buff,10);
//   file_close(file);
    // file_rm(file);
//    load_imgs();
}

uint16_t find_empty_idx(){
    for (int i = 0; i < 32; ++i) {
        if(chunk_idx[i]==0){ // 还没有加载，加载一下
            chunk_idx[i]= page_alloc(1);
            read_disk(FILE_OFFSET+(1+i)*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,chunk_idx[i]);
        }
        for (int j = 0; j < FILE_CHUNK_SIZE/2; ++j) {
            if(chunk_idx[i][j]==FILE_IDX_EMPTY){
                chunk_idx[i][j]=FILE_IDX_END;
                return i*FILE_CHUNK_SIZE/2+j;
            }
        }
    }
    panic("find_empty_idx err");
    return 0;
}

file_t *file_open(const char *name,bool_t create){
    for (int i = 0; i < FILE_ITEM_COUNT; ++i) {
        file_item_t *file_item = file_items+i;
        if(str_equal(file_item->name,name)){
            file_t *file= mem_alloc(sizeof(file_t));
            mem_copy(file->name,file_item->name, 10);
            file->size=file_item->size;
            file->start=file_item->start;
            file->file_idx=i; // 方便信息回写回去
            file->offset=FILE_INVALID_OFFSET;
            return file;
        }
    }
    if(create){ // 没有找到且要求创建了
        for (int i = 0; i < FILE_ITEM_COUNT; ++i) {
            file_item_t *file_item = file_items+i;
            if(str_len(file_item->name)==0){
                mem_copy(file_item->name,(void *)name, 10);
                file_item->start=find_empty_idx(); // 确保都是有值的
                // 创建 file返回
                file_t *file= mem_alloc(sizeof(file_t));
                mem_copy(file->name,file_item->name, 10);
                file->start=file_item->start;
                file->file_idx=i; // 方便信息回写回去
                file->offset=FILE_INVALID_OFFSET;
                return file;
            }
        }
    }
    panic("file_open err name %s",name);
    return 0;
}

void file_close(file_t *file){
    file_flush(file);
    mem_free(file);
}

void file_extend(uint16_t idx,uint32_t size){
    uint16_t i =idx/(FILE_CHUNK_SIZE/2);
    uint16_t j=idx%(FILE_CHUNK_SIZE/2);
    if(chunk_idx[i]==0){
        chunk_idx[i]= page_alloc(1);
        read_disk(FILE_OFFSET+(1+i)*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,chunk_idx[i]);
    }
    if(size>FILE_CHUNK_SIZE){
        uint16_t empty_idx=  find_empty_idx();
        chunk_idx[i][j]=empty_idx;
        file_extend(empty_idx,size-FILE_CHUNK_SIZE);
    } else{
        chunk_idx[i][j]=FILE_IDX_END;
    }
}

uint16_t file_pos_idx(uint16_t idx,uint32_t pos){
    if(pos<FILE_CHUNK_SIZE){
        return idx;
    } else{
        uint16_t i =idx/(FILE_CHUNK_SIZE/2);
        uint16_t j=idx%(FILE_CHUNK_SIZE/2);
        if(chunk_idx[i]==0){
            chunk_idx[i]= page_alloc(1);
            read_disk(FILE_OFFSET+(1+i)*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,chunk_idx[i]);
        }
        return file_pos_idx(chunk_idx[i][j],pos-FILE_CHUNK_SIZE);
    }
}

void file_adjust_buff(file_t *file){
    if(file->pos/FILE_CHUNK_SIZE*FILE_CHUNK_SIZE!=file->offset){
        if(file->dirty){ // 有效的数据在读取前需要先写回去
            file->dirty=FALSE;
            uint16_t idx= file_pos_idx(file->start,file->offset);
            write_disk(FILE_OFFSET+idx*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,file->buff);
        }
        file->offset=file->pos/FILE_CHUNK_SIZE*FILE_CHUNK_SIZE;
        uint16_t idx= file_pos_idx(file->start,file->pos);
        read_disk(FILE_OFFSET+idx*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,file->buff);
    }
}

uint32_t file_write(file_t *file,void *buff,uint32_t size){
    uint32_t write_size=size;
    if(file->size<file->pos+size){
        file->size=file->pos+size;
        file_extend(file->start,file->size);
    }
    while (size>0){
        file_adjust_buff(file);
        uint32_t last=FILE_CHUNK_SIZE-(file->pos-file->offset);
        if(size>last){
            mem_copy(file->buff+file->pos-file->offset,buff,last);
            file->pos+=last;
            buff+=last;
            size-=last;
        } else{
            mem_copy(file->buff+file->pos-file->offset,buff,size);
            file->pos+=size;
            size=0;
        }
        file->dirty=TRUE; // 一旦写入就是脏的
    }
    return write_size;
}

uint32_t file_read(file_t *file,void *buff,uint32_t size){
    uint32_t read_size=file->size-file->pos;
    if(read_size>size){
        read_size=size;
    }
    size=read_size;
    while (size>0){
        file_adjust_buff(file);
        uint32_t last=FILE_CHUNK_SIZE-(file->pos-file->offset);
        if(size>last){
            mem_copy(buff,file->buff+file->pos-file->offset,last);
            file->pos+=last;
            buff+=last;
            size-=last;
        } else{
            mem_copy(buff,file->buff+file->pos-file->offset,size);
            file->pos+=size;
            size=0;
        }
    }
    return read_size;
}

void file_seek(file_t *file,uint32_t pos){
    file->pos=pos;
    file_adjust_buff(file);
}

void file_flush(file_t *file){
    // 写入文件元信息
    file_item_t *item=file_items+file->file_idx;
    mem_copy(item->name,file->name,10);
    item->size=file->size;
    write_disk(FILE_OFFSET, FILE_CHUNK_COUNT,file_items);
    // 写入索引信息
    for (int i = 0; i < 32; ++i) {
        if(chunk_idx[i]!=0){
            write_disk(FILE_OFFSET+(1+i)*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,chunk_idx[i]);
        }
    }
    // 写入文件块
    if(file->dirty){ // 有效的数据在读取前需要先写回去
        file->dirty=FALSE;
        uint16_t idx= file_pos_idx(file->start,file->offset);
        write_disk(FILE_OFFSET+idx*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,file->buff);
    }
}

void file_mv(file_t *file,const char *name){
    mem_copy(file->name,(void *)name,10);
    // 写入文件元信息
    file_item_t *item=file_items+file->file_idx;
    mem_copy(item->name,file->name,10);
    item->size=file->size;
    write_disk(FILE_OFFSET, FILE_CHUNK_COUNT,file_items);
}

void file_free(uint16_t idx){
    uint16_t i =idx/(FILE_CHUNK_SIZE/2);
    uint16_t j=idx%(FILE_CHUNK_SIZE/2);
    if(chunk_idx[i]==0){
        chunk_idx[i]= page_alloc(1);
        read_disk(FILE_OFFSET+(1+i)*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,chunk_idx[i]);
    }
    if(chunk_idx[i][j]!=FILE_IDX_END){
        file_free(chunk_idx[i][j]);
    }
    chunk_idx[i][j]=FILE_IDX_EMPTY;
}

void file_rm(file_t *file){
    // 清空 item
    file_item_t *item=file_items+file->file_idx;
    mem_set(item->name,0,10);
    write_disk(FILE_OFFSET, FILE_CHUNK_COUNT,file_items);
    // 清空相关索引信息
    file_free(file->start);
    for (int i = 0; i < 32; ++i) {
        if(chunk_idx[i]!=0){
            write_disk(FILE_OFFSET+(1+i)*FILE_CHUNK_COUNT,FILE_CHUNK_COUNT,chunk_idx[i]);
        }
    }
    // 回收文件
    mem_free(file);
}