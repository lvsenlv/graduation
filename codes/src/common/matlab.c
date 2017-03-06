/*************************************************************************
	> File Name: matlab.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月06日 星期一 16时00分46秒
 ************************************************************************/

#include <stdio.h>
#include "matlab.h"

void *sum(_matrix_pst mat, ...)
{
#ifdef __DEBUG
    if(!mat)
    {
        DISP_ERR("invalid matrix");
        return NULL;
    }
    if(!mat->pMat)
    {
        DISP_ERR("invalid pMat");
        return NULL;
    }
#endif //__DEBUG

    va_list arg_ptr = NULL;
    int opts = 0;
    _MAT_TYPE ret = 0;
    _MAT_ROW row = mat->row;
    _MAT_COL col = mat->col;
    _MAT_SIZE i = 0, j = 0;
    _matrix_pst mat_ret = NULL;
    _MAT_SIZE size = row * col;

    va_start(arg_ptr, mat);
    opts = va_arg(arg_ptr, int);

    switch(opts)
    {
        case SUM_ROW:
            if(1 == col)
                return NULL;

            mat_ret = matrix_create(row, 1);
            if(!mat_ret)
            {
                DISP_ERR("error in matrix_create");
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
        case SUM_COL:
            if(1 == row)
                return NULL;

            mat_ret = matrix_create(1, col);
            if(!mat_ret)
            {
                DISP_ERR("error in matrix_create");
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
            return (int *)ret;
            break;
        default :
            DISP_ERR("invalid parameters");
            return NULL;
            break;
    }

    va_end(arg_ptr);
    return NULL;
}

