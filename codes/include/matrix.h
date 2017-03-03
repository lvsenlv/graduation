/*************************************************************************
	> File Name: matrix.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时37分30秒
 ************************************************************************/

#ifndef _MATRIX_H
#define _MATRIX_H

typedef     short                           _mat_short;
typedef     int                             _mat_int;
typedef     long long                       _mat_long;
typedef     float                           _mat_float;
typedef     double                          _mat_double;
typedef     char                            _mat_char;
typedef     unsigned char                   _mat_uchar;

#define     _VOL                            volatile
#define     _CON                            const
#define     _STA                            static

#define     _MAT_SHORT                      (short)
#define     _MAT_INT                        (int)
#define     _MAT_LONG                       (long long)
#define     _MAT_FLOAT                      (float)
#define     _MAT_DOUBLE                     (double)
#define     _MAT_CHAR                       (char)
#define     _MAT_UCHAR                      (unsigned char)

#define     _MAT_TYPE                        _mat_int
#define     _MAT_ROW                         _mat_short
#define     _MAT_COL                         _MAT_ROW
#define     MAT_ADD                         (_MAT_CHAR'+')
#define     MAT_SUB                         (_MAT_CHAR'-')
#define     MAT_MUL                         (_MAT_CHAR'*')

#define     DISP_ERR(str) \
            fprintf(stderr, "[%s][%d]: %s \n", __func__, __LINE__, str)
#define     VALUE(mat, nrow, ncol)          (mat->pMat[mat->col*nrow+ncol])

#ifndef _DEBUG_INLINE
#define     CHECK_ROW_COL(row, col)         ((row <= 0 || col <= 0) ? MAT_ERR : MAT_OK)
#define     CHECK_MATRIX(mat)               ((NULL == mat) ? MAT_ERR : MAT_OK)
#define     CHECK_PMAT(mat)                 ((NULL == mat->pMat) ? MAT_ERR : MAT_OK)
#endif //_DEBUG_INLINE

typedef struct matrix_struct_info {
    _MAT_TYPE *pMat; //point to a two-dimensional matrix
    _MAT_ROW row;
    _MAT_COL col;
}matrix_st, *matrix_pst;

typedef enum {
    MAT_OK = 0,
    MAT_ERR,
}MAT_STATUS;



#ifdef _DEBUG_INLINE
inline MAT_STATUS CHECK_ROW_COL(_MAT_ROW row, _MAT_COL col);
inline MAT_STATUS CHECK_MATRIX(matrix_pst mat);
inline MAT_STATUS CHECK_PMAT(matrix_pst mat);
#endif //_DEBUG_INLINE

matrix_pst matrix_create(_MAT_ROW row, _MAT_COL col);
MAT_STATUS matrix_set(matrix_pst mat, _MAT_ROW row, 
    _MAT_COL col, _MAT_TYPE elem);
MAT_STATUS matrix_disp(matrix_pst mat);
MAT_STATUS matrix_free(matrix_pst mat);
matrix_pst matrix_calculate(matrix_pst matA, matrix_pst matB, 
    _CON _mat_char format);

#endif

