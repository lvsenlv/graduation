/*************************************************************************
	> File Name: image.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分56秒
 ************************************************************************/

#ifndef __IMAGE_H
#define __IMAGE_H

#include "common.h"
#include "bmp.h"

#define IMAGE_FILE_NAME_LEN 128

_bmp_pt image_read(int8_t *img_file);

//write normal bmp image to file
_G_STATUS image_write(_bmp_pt bmp, int8_t *img_file);

//write gray bmp image to file
_G_STATUS image_write_gray(_bmp_pt bmp, int8_t *img_file);
_G_STATUS image_write_binary(_bmp_pt bmp, int8_t *img_file);
_G_STATUS image_write_plus_row(_bmp_pt bmp, int8_t *img_file);
_G_STATUS image_write_plus_col(_bmp_pt bmp, int8_t *img_path);

/*************************************************************************
                          extended functions
 ************************************************************************/

#ifdef __OS
uint16_t *image_read_16bit_bmp(uint32_t img_width, uint32_t img_height, 
    int8_t *img_file); //test failed

#else //__OS
#ifdef __FATFS
#include "ff.h"

static inline int fclose_rewrite(FIL *fp)
{
    f_close(fp);
    return 0;
}

#define sd_umount()                         f_mount(NULL, "0:", 1)

FIL *fopen_rewrite(const int8_t *path, int8_t *mode);
uint32_t fwrite_rewrite(const void* buf, uint32_t unused, 
    uint32_t write_size, FIL *fp);

_G_STATUS sd_mount(void);

_G_STATUS image_write_rbg888(uint32_t img_width, uint32_t img_height, 
    uint8_t *data_addr, int8_t *img_file);

#endif //__FATFS

#endif //#ifndef __OS

#endif

