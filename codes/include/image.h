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

_bmp_pt image_read(int8_t *img_file);

//write normal bmp image to file
_G_STATUS image_write(_bmp_pt bmp, int8_t *img_file);

//write gray bmp image to file
_G_STATUS image_write_plus(_bmp_pt bmp, int8_t *img_file);

#endif

