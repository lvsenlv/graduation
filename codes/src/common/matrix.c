/*************************************************************************
	> File Name: matrix.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时36分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

_STA matrix_ptr matrix_sum_sub(matrix_ptr matA, matrix_ptr matB, 
    _CON _mat_char format);
_STA matrix_ptr matrix_multiply(matrix_ptr matA, matrix_ptr matB);

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
    if(MAT_ERR == CHECK_ROW_COL(row, col))
    {
        DISP_ERR("invalid row or col");
        return NULL;
    }

    matrix_ptr matrix = NULL;
    matrix = (matrix_ptr)malloc(sizeof(matrix_t));
    if(MAT_ERR == CHECK_MATRIX(matrix))
    {
        DISP_ERR("error in malloc");
        return NULL;
    }

    matrix->row = row;
    matrix->col = col;
    //matrix->pMat = (MAT_TYPE *)malloc(sizeof(MAT_TYPE)*row*col);
    matrix->pMat = (MAT_TYPE *)calloc(row*col, sizeof(MAT_TYPE));
    if(MAT_ERR == CHECK_PMAT(matrix))
    {
        DISP_ERR("error in calloc \n");
        free(matrix);
        return NULL;
    }

    return matrix;
}

inline MAT_STATUS matrix_set(matrix_ptr matrix, _mat_short row, 
    _mat_short col, MAT_TYPE elem)
{
    if(MAT_ERR == CHECK_MATRIX(matrix))
    {
        DISP_ERR("invalid matrix");
        return MAT_ERR;
    }

    if(MAT_ERR == CHECK_ROW_COL(row+1, col+1) || row+1 > matrix->row || \
        col+1 > matrix->col)
    {
        DISP_ERR("incorrect insert position");
        return MAT_ERR;
    }

    VALUE(matrix, row, col) = elem;

    return MAT_OK;
}

MAT_STATUS matrix_disp(matrix_ptr matrix)
{
    if(MAT_ERR == CHECK_MATRIX(matrix))
    {
        DISP_ERR("invalid matrix");
        return MAT_ERR;
    }

    if(MAT_ERR == CHECK_PMAT(matrix))
        return MAT_OK;

    _mat_short i = 0, j = 0;
    for(i = 0; i < matrix->row; i++)
    {
        for(j = 0; j < matrix->col; j++)
        {
            printf("%d\t", VALUE(matrix, i, j));
        }
        printf("\n");
    }
    printf("\n");

    return MAT_OK;
}

MAT_STATUS matrix_free(matrix_ptr matrix)
{
    if(MAT_ERR != CHECK_MATRIX(matrix))
    {
        if(MAT_ERR != CHECK_PMAT(matrix))
            free(matrix->pMat);
        free(matrix);
    }

    return MAT_OK;
}

matrix_ptr matrix_calculate(matrix_ptr matA, matrix_ptr matB, 
    _CON _mat_char format)
{
    if(MAT_ERR == CHECK_MATRIX(matA) || MAT_ERR == CHECK_MATRIX(matB))
    {
        DISP_ERR("invalid matrix");
        return NULL;
    }

    if(MAT_ERR == CHECK_PMAT(matA) || MAT_ERR == CHECK_PMAT(matB))
    {
        DISP_ERR("invalid pMat");
        return NULL;
    }

    switch(format)
    {
        case MAT_ADD :
            if(matA->row != matB->row || matA->col !=matB->col)
            {
                DISP_ERR("dimension of two matrixs must be equal");
                return NULL;
            }
            return matrix_sum_sub(matA, matB, MAT_ADD);
            break;
        case MAT_SUB :
            if(matA->row != matB->row || matA->col !=matB->col)
            {
                DISP_ERR("dimension of two matrixs must be equal");
                return NULL;
            }
            return matrix_sum_sub(matA, matB, MAT_SUB);
            break;
        case MAT_MUL :
            if(matA->col != matB->row)
            {
                DISP_ERR("matrixA's col != matrixB's row");
                return NULL;
            }
            return matrix_multiply(matA, matB);
            break;
        default :
            DISP_ERR("invalid format");
            break;
    }

    return NULL;
}

_STA matrix_ptr matrix_sum_sub(matrix_ptr matA, matrix_ptr matB, 
    _CON _mat_char format)
{
    _mat_short row = matA->row, col = matB->col;
    _mat_short i = 0, j = 0;
    matrix_ptr matrix_ret = NULL;

    switch(format)
    {
        case MAT_ADD :
            matrix_ret = matrix_create(row, col);
            if(MAT_ERR == CHECK_MATRIX(matrix_ret))
            {
                DISP_ERR("error in matrix_create");
                return NULL;
            }

            for(i = 0; i < row; i++)
            {
                for(j = 0; j < col; j++)
                {
                    VALUE(matrix_ret, i, j) = VALUE(matA, i, j) + \
                        VALUE(matB, i, j);
                }
            }

            return matrix_ret;
            break;
        case MAT_SUB :
            matrix_ret = matrix_create(row, col);
            if(MAT_ERR == CHECK_MATRIX(matrix_ret))
            {
                DISP_ERR("error in matrix_create");
                return NULL;
            }

            for(i = 0; i < row; i++)
            {
                for(j = 0; j < col; j++)
                {
                    VALUE(matrix_ret, i, j) = VALUE(matA, i, j) - \
                        VALUE(matB, i, j);
                }
            }

            return matrix_ret;
            break;
        default :
            return NULL;
            break;
    }
}

_STA matrix_ptr matrix_multiply(matrix_ptr matA, matrix_ptr matB)
{
    _mat_short rowA = matA->row, colA = matA->col;
    _mat_short rowB = matB->row, colB = matB->col;
    matrix_ptr matrix_ret = NULL;

    matrix_ret = matrix_create(rowA, colB);
    if(MAT_ERR == CHECK_MATRIX(matrix_ret))
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }
    
    _mat_short i = 0, j = 0, k = 0;
    MAT_TYPE sum = 0;

    for(i = 0; i < rowA; i++)
    {
        for(j = 0; j < colB; j++)
        {
            sum = 0;
            for(k = 0; k < colA; k++)
            {
                sum += VALUE(matA, i, k) * VALUE(matB, k, j);
            }
            VALUE(matrix_ret, i, j) = sum;
        }
    }
    
    return matrix_ret;
}

