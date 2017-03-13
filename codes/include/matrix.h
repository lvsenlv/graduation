/*************************************************************************
	> File Name: matrix.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时37分30秒
 ************************************************************************/

#ifndef __MATRIX_H
#define __MATRIX_H

#include "common.h"

typedef     short                           _mat_short;
typedef     unsigned short                  _mat_ushort;
typedef     int                             _mat_int;
typedef     unsigned int                    _mat_uint;
typedef     long long                       _mat_long;
typedef     unsigned long long              _mat_ulong;
typedef     float                           _mat_float;
typedef     double                          _mat_double;
typedef     char                            _mat_char;
typedef     unsigned char                   _mat_uchar;

#define     _MAT_TYPE                        _mat_int
#define     _MAT_ROW                         _mat_short
#define     _MAT_COL                         _MAT_ROW
#define     _MAT_SIZE                        _mat_int
#define     MAT_ADD                         ((_mat_char)'+')
#define     MAT_SUB                         ((_mat_char)'-')
#define     MAT_MUL                         ((_mat_char)'*')

#define     VALUE(mat, nrow, ncol)          (mat->pMat[mat->col*nrow+ncol])

#define     CHECK_ROW_COL(row, col)         ((row <= 0 || col <= 0) ? MAT_ERR : MAT_OK)
#define     CHECK_MATRIX(mat)               ((NULL == mat) ? MAT_ERR : MAT_OK)
#define     CHECK_PMAT(mat)                 ((NULL == mat->pMat) ? MAT_ERR : MAT_OK)

typedef struct _matrix_truct_info {
    _MAT_TYPE *pMat; //point to a two-dimensional matrix
    _MAT_ROW row;
    _MAT_COL col;
}_matrix_t, *_matrix_pt;

#define _matrix _matrix_t;

typedef enum {
    MAT_OK = 0,
    MAT_ERR,
}_MAT_STATUS;



#ifdef __DEBUG
_MAT_STATUS matrix_set(_matrix_pt mat, _MAT_ROW row, 
    _MAT_COL col, _MAT_TYPE elem);
#else
#define     matrix_set(mat, row, col, elem)      (VALUE(mat, row, col) = elem)
#endif //__DEBUG

_matrix_pt matrix_create(_MAT_ROW row, _MAT_COL col);
_MAT_STATUS matrix_disp(_matrix_pt mat);
void matrix_free(_matrix_pt mat);
_matrix_pt matrix_calculate(_matrix_pt matA, _matrix_pt matB, 
    _CON _mat_char symbol);

#endif

