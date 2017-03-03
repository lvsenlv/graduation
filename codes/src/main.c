/*************************************************************************
	> File Name: main.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时10分06秒
 ************************************************************************/

#include <stdio.h>
#include "matrix.h"

int main(int argc, char **argv)
{
    _MAT_TYPE array[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    _matrix matA;
    matA.pMat = &array[0][0];
    matA.row = 3;
    matA.col = 3;    

    _matrix_pst matB = NULL;
    _MAT_ROW row = 3, i = 0;
    _MAT_COL col = 3, j = 0;
    _MAT_TYPE elem = 10;
    
    matB = matrix_create(row, col);
    if(MAT_ERR == CHECK_MATRIX(matB))
        return -1;
    
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            //matrix_set(matB, i, j, --elem);
            VALUE(matB, i, j) = --elem;
        }
    }
    
    matrix_disp(&matA);
    DISP("\n");
    matrix_disp(matB);
    DISP("\n");

    _matrix_pst mat_ret = NULL;
    mat_ret = matrix_calculate(&matA, matB, MAT_MUL);
    matrix_disp(mat_ret);

    matrix_free(matB);
    matrix_free(mat_ret);
    
    return 0;
}

