#include "gui/img.h"
#include "sys/mmu.h"
#include "lib/mem.h"
#include "lib/string.h"
#include "lib/err.h"

img_t *img_open(file_t *file){
    img_t *img= mem_alloc(sizeof(img_t));
    file_read(file,img,8);
    img->size = img->w*img->h*3;
    img->data= mem_alloc(img->size);
    file_read(file,img->data,img->size);
    return img;
}

void img_close(img_t *img){
    mem_free(img->data);
    mem_free(img);
}

static uint32_t img_count;
static img_hdr_t *img_hdrs;

void img_init(){
    void *buff= page_alloc(1);
    read_disk(IMG_SECTOR,FILE_CHUNK_COUNT,buff);
    img_count=((uint32_t *)buff)[0];
    img_hdrs=buff+4;
}

img_t *img_create(uint32_t w,uint32_t h){
    img_t *img= mem_alloc(sizeof(img_t));
    img->w=w;
    img->h=h;
    img->size=w*h*3;
    img->data= mem_alloc(img->size);
    return img;
}

img_t *img_open_raw(const char *name){
    img_hdr_t *img_hdr=NULL;
    for (int i = 0; i < img_count; ++i) {
        if(str_equal(name,img_hdrs[i].name)){
            img_hdr=img_hdrs+i;
            break;
        }
    }

    if(!img_hdr){
        panic("no img name %s",name);
        return 0;
    }
    img_t *img= mem_alloc(sizeof(img_t));
    img->w=img_hdr->w;
    img->h=img_hdr->h;
    img->size=img->w*img->h*3;
    img->data= mem_alloc(img->size);
    read_disk(IMG_SECTOR+img_hdr->offset,(img->size+FILE_SECTOR_SIZE-1)/FILE_SECTOR_SIZE,img->data);
    return img;
//    img_hdr_t *img_hdr= page_alloc(1);
//    read_disk(FILE_IMG_ADDR/FILE_SECTOR_SIZE,FILE_CHUNK_COUNT,img_hdr);
//    uint32_t offset = img_hdr->data[idx];
//    mem_free(img_hdr);
//
//    img_t *img= mem_alloc(sizeof(img_t));
//    void *temp = page_alloc(1);
//    read_disk((FILE_IMG_ADDR+offset+16)/FILE_SECTOR_SIZE,FILE_CHUNK_COUNT,temp);
//    img_t *temp0=temp+(FILE_IMG_ADDR+offset+16)%FILE_SECTOR_SIZE;
//    img->w=temp0->w;
//    img->h=temp0->h;
//    mem_free(temp);
//    uint32_t size=img->w*img->h*3;
//
//    img->data= mem_alloc(size);
//    void *buff = mem_alloc(size+512);
//    uint32_t sector = (FILE_IMG_ADDR+offset+16+4+4)/FILE_SECTOR_SIZE;
//    uint32_t src_offset=(FILE_IMG_ADDR+offset+16+4+4)%FILE_SECTOR_SIZE;
//    read_disk(sector,size/FILE_SECTOR_SIZE+1,buff);
//    mem_copy(img->data,buff+src_offset,size);
//    uint32_t dst_offset=0;
//    while (size>0){
//        read_disk(sector,16*FILE_CHUNK_COUNT,buff);
//        uint32_t last=16*FILE_CHUNK_SIZE-src_offset;
//        if(last<size){
//            mem_copy(img->data+dst_offset,buff+src_offset,last);
//            size-=last;
//            dst_offset+=last;
//        } else{
//            mem_copy(img->data+dst_offset,buff+src_offset,size);
//            size=0;
//            dst_offset+=size;
//        }
//        sector+=16*FILE_CHUNK_COUNT;
//        src_offset=0;
//    }
//    mem_free(buff);
//    return img;
}

