/*************************************************************************
	> File Name: image.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分56秒
 ************************************************************************/

#ifndef __IMAGE_H
#define __IMAGE_H

#include "common.h"

typedef enum {
    IMG_OK = 0,
    IMG_ERR,
}_IMG_STATUS;

_IMG_STATUS image_read(char *img_file);

#endif

