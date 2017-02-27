/*************************************************************************
	> File Name: matrix.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时36分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

matrix_ptr matrix_create(mat_short row, mat_short col)
{
	if(0 == CHECK(row, col))
	{
		DISP_ERR("invalid row or col");
		return NULL;
	}

	matrix_ptr matrix = NULL;
	matrix = (matrix_ptr)malloc(sizeof(matrix_t));
	if(NULL == matrix)
	{
		DISP_ERR("error in malloc");
		return NULL;
	}

	matrix->row = row;
	matrix->col = col;
	//matrix->pMat = (MAT_TYPE *)malloc(sizeof(MAT_TYPE)*row*col);
	matrix->pMat = (MAT_TYPE *)calloc(row*col, sizeof(MAT_TYPE));
	if(NULL == matrix->pMat)
	{
		DISP_ERR("error in malloc \n");
		free(matrix);
		return NULL;
	}

	return matrix;
}

inline MAT_STATUS matrix_insert(matrix_ptr matrix, mat_short row, \
	mat_short col, MAT_TYPE elem)
{
	if(NULL == matrix)
	{
		DISP_ERR("invalid matrix");
		return MAT_ERR;
	}

	if(!CHECK(row+1, col+1) || row+1 > matrix->row || col+1 > matrix->col)
	{
		DISP_ERR("incorrect insert position");
		return MAT_ERR;
	}

	matrix->pMat[matrix->col*row + col] = elem;
 
	return MAT_OK;
}

MAT_STATUS matrix_disp(matrix_ptr matrix)
{
	if(NULL == matrix)
	{
		DISP_ERR("invalid matrix");
		return MAT_ERR;
	}

	if(NULL == matrix->pMat)
		return MAT_OK;

	mat_short i = 0, j = 0;
	for(i = 0; i < matrix->row; i++)
	{
		for(j = 0; j < matrix->col; j++)
		{
			printf("%d\t", matrix->pMat[matrix->col*i +j]);
		}
		printf("\n");
	}

	return MAT_OK;
}

MAT_STATUS matrix_free(matrix_ptr matrix)
{
	if(NULL != matrix)
	{
		if(NULL != matrix->pMat)
			free(matrix->pMat);
		free(matrix);
	}
	
	return MAT_OK;
}

