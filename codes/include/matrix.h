/*************************************************************************
	> File Name: matrix.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时37分30秒
 ************************************************************************/

#ifndef _MATRIX_H
#define _MATRIX_H

typedef 	short 							_mat_short;
typedef 	int 							_mat_int;
typedef 	long long 						_mat_long;
typedef 	float 							_mat_float;
typedef 	double 							_mat_double;
typedef 	char 							_mat_char;
#define 	_VOL 							volatile
#define 	_CON 							const
#define 	_STA 							static

#define 	MAT_TYPE 						_mat_int
#define 	DISP_ERR(str) 					fprintf(stderr, "%s \n", str)

#ifndef _DEBUG_INLINE
#define 	CHECK_ROW_COL(row, col) 		((row > 0 && col > 0) ? 1 : 0)
#define 	CHECK_MATRIX(matrix) 			((NULL == matrix) ? -1 : 0)
#define 	CHECK_PMAT(matrix) 				((NULL == matrix->pMat) ? -1 : 0)
#endif //_DEBUG_INLINE

typedef struct matrix_struct_info {
	MAT_TYPE *pMat; //point to a two-dimensional matrix
	_mat_short row;
	_mat_short col;
}matrix_t, *matrix_ptr;

typedef enum {
	MAT_OK = 0,
	MAT_ERR,
}MAT_STATUS;

#ifdef _DEBUG_INLINE
inline MAT_STATUS CHECK_ROW_COL(_mat_short row, _mat_short col);
inline MAT_STATUS CHECK_MATRIX(matrix_ptr matrix);
inline MAT_STATUS CHECK_PMAT(matrix_ptr matrix);
#endif //_DEBUG_INLINE

matrix_ptr matrix_create(_mat_short row, _mat_short col);
MAT_STATUS matrix_set(matrix_ptr matrix, _mat_short row, 
	_mat_short col, MAT_TYPE elem);
MAT_STATUS matrix_disp(matrix_ptr matrix);
MAT_STATUS matrix_free(matrix_ptr matrix);
matrix_ptr matrix_calculate(matrix_ptr matA, matrix_ptr matB, 
	_CON _mat_char format);

#endif

