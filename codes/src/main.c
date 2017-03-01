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
    matrix_ptr matA = matrix_create(5, 5);
	printf("%s in matrix_create \n", (matA ? "success" : "error"));
    matrix_ptr matB = matrix_create(5, 5);
	printf("%s in matrix_create \n", (matB ? "success" : "error"));
    matrix_ptr mat_ret = NULL;

	int i = 0, j = 0;
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
            matrix_set(matA, i, j, i);
            matrix_set(matB, i, j, j);
        }
	}

	matrix_disp(matA);
    matrix_disp(matB);

    mat_ret = matrix_calculate(matA, matB, (unsigned char)'+');
    matrix_disp(mat_ret);
    
	matrix_free(matA);
    matrix_free(matB);
    matrix_free(mat_ret);
    return 0;
}

