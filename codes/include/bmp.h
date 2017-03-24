/*************************************************************************
	> File Name: bmp.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 19时14分25秒
 ************************************************************************/

#ifndef __BMP_H
#define __BMP_H

#include "common.h"
#include "matrix.h"

#define     HEAD_INFO_SIZE                  54
#define     COUNT_REAL_WIDTH(bits)          ((((bits)+31)>>5)<<2)
#define     FILTER_MODEL_SIZE               3
#define     FILTER_OFFSET                   (FILTER_MODEL_SIZE >> 1)
#define     FILTER_OFFSET_PLUS              ((FILTER_MODEL_SIZE*FILTER_MODEL_SIZE) >> 1)
#define     MIDDLE(a, b, c) \
            (   (a)>(b)   \
                 ?   \
                    (    (a)>(c)   ?   ( (b)>(c) ? (b):(c) )    :   (a)    )    \
                 :    \
                    (    (b)>(c)   ?   ( (a)>(c) ? (a):(c) )    :   (b)    )    \
            )

#if 0
typedef struct bmp_head_main_info {
    uint32_t size;                  //the size of image
    uint16_t reserved1;             //reserver word
    uint16_t reserved2;             //reserver word
    uint32_t off_bits;              //the offset bit of real color data
}_head_main_t, *_head_main_pt;

typedef struct bmp_head_plus_info {
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
}_head_plus_t, *_head_plus_pt;

typedef struct bmp_info {
    _head_main_t main_info;
    _head_plus_t plus_info;
}_bmp_t, *_bmp_pt;
#endif

typedef struct bmp_vital_info { //use memset to initialize this struct after creating
    uint32_t width;
    uint32_t height;
    uint16_t bit_count;
    uint32_t real_size;
    uint32_t real_width;
}_bmp_vital_t, *_bmp_vital_pt;

typedef struct bmp_info {
    _matrix_pt head_info;       //this info contains format, main_info and plus_info
    _bmp_vital_pt vital_info;
    _matrix_pt data_info;
}_bmp_t, *_bmp_pt;

_bmp_pt bmp_create(void);
_G_STATUS bmp_head_parse(_bmp_pt bmp, uint8_t *ptr);
void bmp_free(_bmp_pt bmp);
void bmp_viatl_info_disp(_bmp_pt bmp);
_G_STATUS bmp_data_parse(_bmp_pt bmp, uint8_t *ptr);
_bmp_pt bmp_convert_gray(_bmp_pt bmp);
_G_STATUS bmp_median_filter(_bmp_pt bmp);


#endif

