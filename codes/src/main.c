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
    matrix_ptr matrix = matrix_create(100, 5);
	printf("%s in matrix_create \n", (matrix ? "success" : "error"));

	int i = 0, j = 0;
	for(i = 0; i < 100; i++)
	{
		for(j = 0; j < 5; j++)
			matrix_set(matrix, i, j, i*j);
	}

	matrix_disp(matrix);
	matrix_free(matrix);

    return 0;
}

