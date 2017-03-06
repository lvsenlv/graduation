/*************************************************************************
	> File Name: matlab.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月06日 星期一 16时00分56秒
 ************************************************************************/

#ifndef _MATLAB_H
#define _MATLAB_H

#include <stdarg.h>
#include "matrix.h"

typedef enum {
    SUM_ROW = 1,
    SUM_COL,
    SUM_ALL,
}SUM_OPTS;

void *sum(_matrix_pst mat, ...);

#endif

