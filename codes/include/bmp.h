/*************************************************************************
	> File Name: bmp.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 19时14分25秒
 ************************************************************************/

#ifndef __BMP_H
#define __BMP_H

#include "common.h"
#include <stdint.h>
#include "matrix.h"

#define     HEAD_INFO_SIZE                  54
#define     COUNT_REAL_WIDTH(bits)          ((((bits)+31)/32)*4)

typedef struct head_main_info {
    uint32_t size;                  //the size of image
    uint16_t reserved1;             //reserver word
    uint16_t reserved2;             //reserver word
    uint32_t off_bits;              //the offset bit of real color data
}_head_main_t;

typedef struct head_plus_info {
    uint32_t struct_size;           //the size of this struct
    uint32_t width;                 //the width of image
    uint32_t height;                //the height of image
    uint16_t planes;                //the number of plane of image, default value is 1
    uint16_t bit_count;             //the bits of color, may be 1,2,4,8,16,24,32.
    uint32_t comperssion;           //the way of compression,may be 0,1,2, 0 means no compression
    uint32_t real_size;             //the size of color date
    uint32_t x_pixel;               //the resolution of x
    uint32_t y_pixel;               //the resolution of y
    uint32_t clr_used;              //the number of color that is used, default value is 0
    uint32_t clr_import;            //the number of important color, 0 means all are important
}_head_plus_t;

typedef struct image_info {
    _head_main_t main_info;
    _head_plus_t plus_info;
}_bmp_t;

_G_STATUS bmp_head_parse(_CON uint8_t *ptr);
void bmp_info_disp(void);
_matrix_pt bmp_data_parse(_CON uint8_t *ptr);

#endif

