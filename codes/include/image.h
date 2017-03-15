/*************************************************************************
	> File Name: image.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分56秒
 ************************************************************************/

#ifndef __IMAGE_H
#define __IMAGE_H

#include "common.h"
#include <stdint.h>

#define     IMG_FILE                        "/root/test.png"
#define     HEAD_INFO_SIZE                  54

typedef enum {
    IMG_OK = 0,
    IMG_ERR,
}_IMG_STATUS;

typedef struct image_head_main_info {
    uint32_t size;                  //the size of image
    uint16_t reserved1;             //reserver word
    uint16_t reserved2;             //reserver word
    uint32_t off_bits;              //the offset bit of real color data
}_img_head_main_t;

typedef struct image_head_plus_info {
    uint32_t size_struct;           //the size of this struct
    uint32_t width;                 //the width of image
    uint32_t height;                //the height of image
    uint16_t planes;                //the number of plane of image, default value is 1
    uint16_t bit_count;             //the bits of color, may be 1,2,4,8,16,24,32.
    uint32_t comperssion;           //the way of compression,may be 0,1,2, 0 means no compression
    uint32_t size_real;             //the size of color date
    uint32_t x_pixel;               //the resolution of x
    uint32_t y_pixel;               //the resolution of y
    uint32_t clr_used;              //the number of color that is used, default value is 0
    uint32_t clr_import;            //the number of important color, 0 means all are important
}_img_head_plus_t;

typedef struct image_info {
    _img_head_main_t main_info;
    _img_head_plus_t plus_info;
}_image_t;

_IMG_STATUS image_read(char *img_file);

#endif

