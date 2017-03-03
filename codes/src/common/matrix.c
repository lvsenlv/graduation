/*************************************************************************
	> File Name: matrix.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时36分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

_STA _matrix_pst matrix_sum_sub(_matrix_pst matA, _matrix_pst matB, 
    _CON _mat_char symbol);
_STA _matrix_pst matrix_multiply(_matrix_pst matA, _matrix_pst matB);

#ifdef _DEBUG_INLINE
inline _MAT_STATUS CHECK_ROW_COL(_MAT_ROW row, _MAT_COL col)
{
    return ((row <= 0 || col <= 0) ? MAT_ERR : MAT_OK);
}

inline _MAT_STATUS CHECK_MATRIX(_matrix_pst mat)
{
    return ((NULL == mat) ? MAT_ERR : MAT_OK);
}

inline _MAT_STATUS CHECK_PMAT(_matrix_pst mat)
{
    return ((NULL == mat->pMat) ? MAT_ERR : MAT_OK);
}
#endif //_DEBUG_INLINE

_matrix_pst matrix_create(_MAT_ROW row, _MAT_COL col)
{
    if(MAT_ERR == CHECK_ROW_COL(row, col))
    {
        DISP_ERR("invalid row or col");
        return NULL;
    }

    _matrix_pst mat = NULL;
    mat = (_matrix_pst)malloc(sizeof(_matrix_st));
    if(MAT_ERR == CHECK_MATRIX(mat))
    {
        DISP_ERR("error in malloc");
        return NULL;
    }

    mat->row = row;
    mat->col = col;
    //mat->pMat = (_MAT_TYPE *)malloc(sizeof(_MAT_TYPE)*row*col);
    mat->pMat = (_MAT_TYPE *)calloc(row*col, sizeof(_MAT_TYPE));
    if(MAT_ERR == CHECK_PMAT(mat))
    {
        DISP_ERR("error in calloc \n");
        free(mat);
        return NULL;
    }

    return mat;
}

inline _MAT_STATUS matrix_set(_matrix_pst mat, _MAT_ROW row, 
    _MAT_COL col, _MAT_TYPE elem)
{
    if(MAT_ERR == CHECK_MATRIX(mat))
    {
        DISP_ERR("invalid matrix");
        return MAT_ERR;
    }

    if(MAT_ERR == CHECK_ROW_COL(row+1, col+1) || row+1 > mat->row || \
        col+1 > mat->col)
    {
        DISP_ERR("incorrect insert position");
        return MAT_ERR;
    }

    VALUE(mat, row, col) = elem;

    return MAT_OK;
}

_MAT_STATUS matrix_disp(_matrix_pst mat)
{
    if(MAT_ERR == CHECK_MATRIX(mat))
    {
        DISP_ERR("invalid matrix");
        return MAT_ERR;
    }

    if(MAT_ERR == CHECK_PMAT(mat))
        return MAT_OK;

    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    for(i = 0; i < mat->row; i++)
    {
        for(j = 0; j < mat->col; j++)
        {
            DISP("%d\t", VALUE(mat, i, j));
        }
        DISP("\n");
    }
    //printf("\n");

    return MAT_OK;
}

_MAT_STATUS matrix_free(_matrix_pst mat)
{
    if(MAT_ERR != CHECK_MATRIX(mat))
    {
        if(MAT_ERR != CHECK_PMAT(mat))
            free(mat->pMat);
        free(mat);
    }

    return MAT_OK;
}

_matrix_pst matrix_calculate(_matrix_pst matA, _matrix_pst matB, 
    _CON _mat_char symbol)
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

    switch(symbol)
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
            DISP_ERR("invalid symbol");
            break;
    }

    return NULL;
}

_STA _matrix_pst matrix_sum_sub(_matrix_pst matA, _matrix_pst matB, 
    _CON _mat_char symbol)
{
    _MAT_ROW row = matA->row, i = 0;
    _MAT_COL col = matB->col, j = 0;
    _matrix_pst mat_ret = NULL;

    switch(symbol)
    {
        case MAT_ADD :
            mat_ret = matrix_create(row, col);
            if(MAT_ERR == CHECK_MATRIX(mat_ret))
            {
                DISP_ERR("error in matrix_create");
                return NULL;
            }

            for(i = 0; i < row; i++)
            {
                for(j = 0; j < col; j++)
                {
                    VALUE(mat_ret, i, j) = VALUE(matA, i, j) + \
                        VALUE(matB, i, j);
                }
            }

            return mat_ret;
            break;
        case MAT_SUB :
            mat_ret = matrix_create(row, col);
            if(MAT_ERR == CHECK_MATRIX(mat_ret))
            {
                DISP_ERR("error in matrix_create");
                return NULL;
            }

            for(i = 0; i < row; i++)
            {
                for(j = 0; j < col; j++)
                {
                    VALUE(mat_ret, i, j) = VALUE(matA, i, j) - \
                        VALUE(matB, i, j);
                }
            }

            return mat_ret;
            break;
        default :
            return NULL;
            break;
    }
}

_STA _matrix_pst matrix_multiply(_matrix_pst matA, _matrix_pst matB)
{
    _MAT_ROW rowA = matA->row;
    _MAT_COL colA = matA->col, colB = matB->col;
    _matrix_pst mat_ret = NULL;

    mat_ret = matrix_create(rowA, colB);
    if(MAT_ERR == CHECK_MATRIX(mat_ret))
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }
    
    _MAT_ROW i = 0;
    _MAT_COL j = 0, k = 0;
    _MAT_TYPE sum = 0;

    for(i = 0; i < rowA; i++)
    {
        for(j = 0; j < colB; j++)
        {
            sum = 0;
            for(k = 0; k < colA; k++)
            {
                sum += VALUE(matA, i, k) * VALUE(matB, k, j);
            }
            VALUE(mat_ret, i, j) = sum;
        }
    }
    
    return mat_ret;
}

