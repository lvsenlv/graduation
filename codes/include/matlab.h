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

#ifdef __DEBUG
#define     zeros(row, col)                           matrix_create(row, col)
#else
_matrix_pt zeros(_MAT_ROW row, _MAT_COL col);
#endif //__DEBUG

typedef enum {
    SUM_ROW = 1,
    SUM_COL,
    SUM_ALL,
}SUM_OPTS;

void *sum(_matrix_pt mat, ...);
_MAT_TYPE max(_matrix_pt mat);
_matrix_pt ones(_MAT_ROW row, _MAT_COL col, ...);

#endif

