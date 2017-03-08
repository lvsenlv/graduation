/*************************************************************************
	> File Name: main.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时10分06秒
 ************************************************************************/

#include <stdio.h>
#include "matrix.h"
#include "matlab.h"
#include "template.h"

int main(int argc, char **argv)
{/*
    _MAT_TYPE array[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    _matrix matA;
    matA.pMat = &array[0][0];
    matA.row = 3;
    matA.col = 3;

    matrix_disp(&matA);*/

    _matrix_pst mat_ret = NULL;
    
    mat_ret = template_one(16);
    matrix_disp(mat_ret);
    matrix_free(mat_ret);
    DISP("\n");
    mat_ret = template_two(16);
    matrix_disp(mat_ret);
    matrix_free(mat_ret);
    DISP("\n");
    mat_ret = template_three(16);
    matrix_disp(mat_ret);
    matrix_free(mat_ret);
    DISP("\n");
    mat_ret = template_four(4);
    matrix_disp(mat_ret);
    matrix_free(mat_ret);
    
    return 0;
}

