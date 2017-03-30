/*************************************************************************
	> File Name: matrix.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时36分15秒
 ************************************************************************/

#include <stdlib.h>
#include "matrix.h"

_STA _matrix_pt matrix_sum_sub(_matrix_pt matA, _matrix_pt matB, 
    _CON uint8_t symbol);
_STA _matrix_pt matrix_multiply(_matrix_pt matA, _matrix_pt matB);

_matrix_pt matrix_create(_MAT_ROW row, _MAT_COL col)
{
#ifdef __DEBUG
    if(row <= 0 || col <= 0)
    {
        DISP_ERR("invalid row or col");
        return NULL;
    }
#endif //__DEBUG

    _matrix_pt mat = NULL;
    mat = (_matrix_pt)malloc(sizeof(_matrix_t));
    if(!mat)
    {
        DISP_ERR(ERR_MALLOC);
        return NULL;
    }

    mat->pMat = NULL;
    mat->row = row;
    mat->col = col;
#ifdef __DEBUG
    mat->pMat = (_MAT_TYPE *)calloc(row*col, sizeof(_MAT_TYPE));
    if(!mat->pMat)
    {
        DISP_ERR(ERR_MALLOC);
        free(mat);
        mat = NULL;
        return NULL;
    }
#else
    mat->pMat = (_MAT_TYPE *)malloc(sizeof(_MAT_TYPE)*row*col);
    if(!mat->pMat)
    {
        DISP_ERR(ERR_MALLOC);
        free(mat);
        mat = NULL;
        return NULL;
    }
#endif //__DEBUG

    return mat;
}

#ifdef __DEBUG
_MAT_STATUS matrix_set(_matrix_pt mat, _MAT_ROW row, 
    _MAT_COL col, _MAT_TYPE elem)
{

    if(!mat)
    {
        DISP_ERR(ERR_MATRIX);
        return MAT_ERR;
    }
    
    if(!mat->pMat)
    {
        DISP_ERR(ERR_PMAT);
        return MAT_ERR;
    }

    if(row < 0 || col < 0 || (row+1) > mat->row || \
        (col+1) > mat->col)
    {
        DISP_ERR("incorrect insert position");
        return MAT_ERR;
    }


    VALUE(mat, row, col) = elem;

    return MAT_OK;
}
#endif //__DEBUG

_MAT_STATUS matrix_disp(_matrix_pt mat)
{
#ifdef __DEBUG
    if(!mat)
    {
        DISP_ERR(ERR_MATRIX);
        return MAT_ERR;
    }

    if(!mat->pMat)
        return MAT_OK;
#endif //__DEBUG
    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    for(i = 0; i < mat->row; i++)
    {
        for(j = 0; j < mat->col; j++)
        {
            DISP("%x ", VALUE(mat, i, j));
        }
        DISP("\n");
    }
    //DISP("\n");

    return MAT_OK;
}

void matrix_free(_matrix_pt mat)
{
    if(mat)
    {
        if(mat->pMat)
        {
            free(mat->pMat);
        }
        free(mat);
    }
}

_matrix_pt matrix_calculate(_matrix_pt matA, _matrix_pt matB, 
    _CON uint8_t symbol)
{
#ifdef __DEBUG
    if((!matA) || (!matB))
    {
        DISP_ERR(ERR_MATRIX);
        return NULL;
    }

    if((!matA->pMat) || (!matB->pMat))
    {
        DISP_ERR(ERR_PMAT);
        return NULL;
    }
#endif //__DEBUG

    switch(symbol)
    {
        case MAT_ADD :
#ifdef __DEBUG
            if(matA->row != matB->row || matA->col !=matB->col)
            {
                DISP_ERR("dimension of two matrixs must be equal");
                return NULL;
            }
#endif //__DEBUG
            return matrix_sum_sub(matA, matB, MAT_ADD);
            break;
        case MAT_SUB :
#ifdef __DEBUG
            if(matA->row != matB->row || matA->col !=matB->col)
            {
                DISP_ERR("dimension of two matrixs must be equal");
                return NULL;
            }
#endif //__DEBUG
            return matrix_sum_sub(matA, matB, MAT_SUB);
            break;
        case MAT_MUL :
#ifdef __DEBUG
            if(matA->col != matB->row)
            {
                DISP_ERR("matrixA's col != matrixB's row");
                return NULL;
            }
#endif //__DEBUG
            return matrix_multiply(matA, matB);
            break;
        default :
            DISP_ERR("invalid symbol");
            break;
    }

    return NULL;
}

_STA _matrix_pt matrix_sum_sub(_matrix_pt matA, _matrix_pt matB, 
    _CON uint8_t symbol)
{
    _MAT_SIZE size = matA->row * matA->col;
    _MAT_SIZE i = 0;
    _matrix_pt mat_ret = NULL;

    switch(symbol)
    {
        case MAT_ADD :
            mat_ret = matrix_create(matA->row, matA->col);
            if(!mat_ret)
            {
                DISP_ERR(ERR_CREATE);
                return NULL;
            }
            
            for(i = 0; i < size; i++)
            {
                mat_ret->pMat[i] = matA->pMat[i] + \
                    matB->pMat[i];
            }

            return mat_ret;
            break;
        case MAT_SUB :
            mat_ret = matrix_create(matA->row, matA->col);
            if(!mat_ret)
            {
                DISP_ERR(ERR_CREATE);
                return NULL;
            }
            
            for(i = 0; i < size; i++)
            {
                mat_ret->pMat[i] = matA->pMat[i] - \
                    matB->pMat[i];
            }

            return mat_ret;
            break;
        default :
            return NULL;
            break;
    }
}

_STA _matrix_pt matrix_multiply(_matrix_pt matA, _matrix_pt matB)
{
    _MAT_ROW rowA = matA->row;
    _MAT_COL colA = matA->col, colB = matB->col;
    _matrix_pt mat_ret = NULL;

    mat_ret = matrix_create(rowA, colB);
    if(!mat_ret)
    {
        DISP_ERR(ERR_CREATE);
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



/*************************************************************************
                          extended functions
 ************************************************************************/
 
#include <string.h>

void *sum(_matrix_pt mat, ...)
{
#ifdef __DEBUG
    if(!mat)
    {
        DISP_ERR(ERR_MATRIX);
        return NULL;
    }
    if(!mat->pMat)
    {
        DISP_ERR(ERR_PMAT);
        return NULL;
    }
#endif //__DEBUG

    va_list arg_ptr = NULL;
    int opts = 0;
    uint32_t ret = 0;
    _MAT_ROW row = mat->row;
    _MAT_COL col = mat->col;
    _MAT_SIZE i = 0, j = 0;
    _matrix_pt mat_ret = NULL;
    _MAT_SIZE size = row * col;

    va_start(arg_ptr, mat);
    opts = va_arg(arg_ptr, int);

    switch(opts)
    {
        case SUM_COL:
            if(1 == row)
                return NULL;

            mat_ret = matrix_create(1, col);
            if(!mat_ret)
            {
                DISP_ERR(ERR_CREATE);
                return NULL;
            }

            for(j = 0; j < col; j++)
            {
                ret = 0;
                for(i = 0; i < row; i++)
                {
                    ret += VALUE(mat, i, j);
                }
                mat_ret->pMat[j] = ret;
            }
            
            va_end(arg_ptr);
            return mat_ret;
            break;
        case SUM_ALL :
            for(i = 0; i < size; i++)
            {
                ret += mat->pMat[i];
            }

            va_end(arg_ptr);
            return (uint32_t *)ret;
            break;
        case SUM_ROW:
        default :    
            if(1 == col)
                return NULL;

            mat_ret = matrix_create(row, 1);
            if(!mat_ret)
            {
                DISP_ERR(ERR_CREATE);
                return NULL;
            }
            
            for(i = 0; i < row; i++)
            {
                ret = 0;
                for(j = 0; j < col; j++)
                {
                    ret += VALUE(mat, i, j);
                }
                mat_ret->pMat[i] = ret;
            }
            
            va_end(arg_ptr);
            return mat_ret;
            break;
    }

    va_end(arg_ptr);
    return NULL;
}

_MAT_TYPE max(_matrix_pt mat)
{
#ifdef __DEBUG
    if(!mat)
    {
        DISP_ERR(ERR_MATRIX);
        return -1;
    }
    if(!mat->pMat)
    {
        DISP_ERR(ERR_PMAT);
        return -1;
    }
#endif //__DEBUG

    _MAT_SIZE size = mat->row * mat->col;
    _MAT_SIZE i = 0;
    _MAT_TYPE ret = mat->pMat[0];
    
    for(i = 1; i < size; i++)
    {
        ret = (mat->pMat[i] > ret ? mat->pMat[i] : ret);
    }

    return ret;
}

#ifndef __DEBUG
_matrix_pt zeros(_MAT_ROW row, _MAT_COL col)
{
    _matrix_pt mat_ret = NULL;
    
    mat_ret = matrix_create(row, col);
    if(!mat_ret)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    memset(mat_ret->pMat, 0, row*col*sizeof(_MAT_TYPE));
    
    return mat_ret;
}

_matrix_pt ones(_MAT_ROW row, _MAT_COL col)
{
    _matrix_pt mat_ret = matrix_create(row, col);
    if(!mat_ret)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }
    
    _MAT_SIZE size = row * col;
    _MAT_SIZE i = 0;
    for(i = 0; i < size ; i++)
    {
        mat_ret->pMat[i] = 1;
    }
    
    return mat_ret;
}

#else //__DEBUG
_matrix_pt ones(_MAT_ROW row, _MAT_COL col, ...)
{
    _matrix_pt mat_ret = matrix_create(row, col);
    if(!mat_ret)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    va_list arg_ptr = NULL;
    int elem = 0;

    va_start(arg_ptr, col);
    elem = va_arg(arg_ptr, int);
    
    elem = ((elem > 65535 || elem < -65535) ? 1 : elem);

    _MAT_SIZE size = row * col;
    _MAT_SIZE i = 0;
    for(i = 0; i < size ; i++)
    {
        mat_ret->pMat[i] = elem;
    }

    va_end(arg_ptr);
    return mat_ret;
}
#endif //__DEBUG


