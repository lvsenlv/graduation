/*************************************************************************
	> File Name: matrix.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时36分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

static matrix_ptr matrix_sum_sub(matrix_ptr matA, matrix_ptr matB, 
	_CON _mat_char format);

#ifdef _DEBUG_INLINE
inline MAT_STATUS CHECK_ROW_COL(_mat_short row, _mat_short col)
{
	return ((row <= 0 || col <= 0) ? MAT_ERR : MAT_OK);
}

inline MAT_STATUS CHECK_MATRIX(matrix_ptr matrix)
{
	return ((NULL == matrix) ? MAT_ERR : MAT_OK);
}

inline MAT_STATUS CHECK_PMAT(matrix_ptr matrix)
{
	return ((NULL == matrix->pMat) ? MAT_ERR : MAT_OK);
}
#endif //_DEBUG_INLINE

matrix_ptr matrix_create(_mat_short row, _mat_short col)
{
	if(CHECK_ROW_COL(row, col))
	{
		DISP_ERR("invalid row or col");
		return NULL;
	}

	matrix_ptr matrix = NULL;
	matrix = (matrix_ptr)malloc(sizeof(matrix_t));
	if(CHECK_MATRIX(matrix))
	{
		DISP_ERR("error in malloc");
		return NULL;
	}

	matrix->row = row;
	matrix->col = col;
	//matrix->pMat = (MAT_TYPE *)malloc(sizeof(MAT_TYPE)*row*col);
	matrix->pMat = (MAT_TYPE *)calloc(row*col, sizeof(MAT_TYPE));
	if(CHECK_PMAT(matrix))
	{
		DISP_ERR("error in malloc \n");
		free(matrix);
		return NULL;
	}

	return matrix;
}

inline MAT_STATUS matrix_set(matrix_ptr matrix, _mat_short row, 
	_mat_short col, MAT_TYPE elem)
{
	if(CHECK_MATRIX(matrix))
	{
		DISP_ERR("invalid matrix");
		return MAT_ERR;
	}

	if(CHECK_ROW_COL(row+1, col+1) || row+1 > matrix->row || \
		col+1 > matrix->col)
	{
		DISP_ERR("incorrect insert position");
		return MAT_ERR;
	}

	matrix->pMat[matrix->col*row + col] = elem;
 
	return MAT_OK;
}

MAT_STATUS matrix_disp(matrix_ptr matrix)
{
	if(CHECK_MATRIX(matrix))
	{
		DISP_ERR("invalid matrix");
		return MAT_ERR;
	}

	if(CHECK_PMAT(matrix))
		return MAT_OK;

	_mat_short i = 0, j = 0;
	for(i = 0; i < matrix->row; i++)
	{
		for(j = 0; j < matrix->col; j++)
		{
			printf("%d\t", matrix->pMat[matrix->col*i + j]);
		}
		printf("\n");
	}

	return MAT_OK;
}

MAT_STATUS matrix_free(matrix_ptr matrix)
{
	if(CHECK_MATRIX(matrix))
	{
		if(CHECK_PMAT(matrix))
			free(matrix->pMat);
		free(matrix);
	}
	
	return MAT_OK;
}

matrix_ptr matrix_calculate(matrix_ptr matA, matrix_ptr matB, 
	_CON _mat_char format)
{
	if(CHECK_MATRIX(matA) || CHECK_MATRIX(matB))
	{
		DISP_ERR("invalid matrix");
		return NULL;
	}

	if(CHECK_PMAT(matA) || CHECK_PMAT(matB))
	{
		DISP_ERR("invalid pMat");
		return NULL;
	}

	switch(format)
	{
		case '+' :
			if(matA->row != matB->row || matA->col !=matB->col)
			{
				DISP_ERR("dimension of two matrixs must be equal");
				return NULL;
			}
			break;
		case '-' :
			if(matA->row != matB->row || matA->col !=matB->col)
			{
				DISP_ERR("dimension of two matrixs must be equal");
				return NULL;
			}
			break;
		case '*' :
			if(matA->col != matB->row)
			{
				DISP_ERR("matrixA's col != matrixB's row");
				return NULL;
			}
			break;
		default :
			DISP_ERR("invalid format");
			break;
	}

	return NULL;
}

static matrix_ptr matrix_sum_sub(matrix_ptr matA, matrix_ptr matB, 
	_CON _mat_char format)
{
	_mat_short row = matA->row, col = matB->col;
	_mat_short i = 0, j = 0;
	matrix_ptr matrix_ret = NULL;
	
	switch(format)
	{
		case '+' :
			matrix_ret = (matrix_ptr)malloc(sizeof(matrix_t));
			if(CHECK_MATRIX(matrix_ret))
			{
				DISP_ERR("error in malloc");
				return NULL;
			}
			matrix_ret->pMat = (MAT_TYPE *)malloc(sizeof(MAT_TYPE)*row*col);
			if(CHECK_PMAT(matrix_ret))
			{
				DISP_ERR("error in malloc");
				return NULL;
			}

			for(i = 0; i < row; i++)
			{
				for(j = 0; j < col; j++)
				{
					matrix_ret->pMat[matrix_ret->col*i + j] = \
						matA->pMat[matA->col*i + j] + matB->pMat[matB->col*i + j];
				}
			}

			break;
		case '-' :
			break;
		default :
			return NULL;
			break;
	}
}

