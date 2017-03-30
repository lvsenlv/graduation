/*************************************************************************
	> File Name: template.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月07日 星期二 13时33分26秒
 ************************************************************************/

#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#include "matrix.h"

#define     BASE_WID                        4
#define     FOREGROUND                      1
#define     BACKGROUND                      0

extern _matrix_pt g_number_mat[10];

_matrix_pt template_zero(_MAT_COL wid);
_matrix_pt template_one(_MAT_COL wid);
_matrix_pt template_two(_MAT_COL wid);
_matrix_pt template_three(_MAT_COL wid);
_matrix_pt template_four(_MAT_COL wid);
_matrix_pt template_five(_MAT_COL wid);
_matrix_pt template_six(_MAT_COL wid);
_matrix_pt template_seven(_MAT_COL wid);
_matrix_pt template_eight(_MAT_COL wid);
_matrix_pt template_nine(_MAT_COL wid);
_MAT_STATUS template_create(_MAT_COL wid);
void template_free(void);
void template_disp(void);

#endif

