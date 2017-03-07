/*************************************************************************
	> File Name: common.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月06日 星期一 13时42分52秒
 ************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>

#define     _VOL                            volatile
#define     _CON                            const
#define     _STA                            static

#define     DISP_ERR(str) \
            fprintf(stderr, "[%s][%d]: %s \n", __func__, __LINE__, str)
#define     DISP(format, args...) \
            fprintf(stdout, format, ##args)

#endif
