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
_G_STATUS image_write_plus_col(_bmp_pt bmp, int8_t *img_file);

#endif

