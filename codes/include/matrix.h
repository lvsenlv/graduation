/*************************************************************************
	> File Name: matrix.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时37分30秒
 ************************************************************************/

#ifndef _MATRIX_H
#define _MATRIX_H

typedef 	short 						mat_short;
typedef 	int 						mat_int;
typedef 	long long 					mat_long;
typedef 	float 						mat_float;
typedef 	double 						mat_double;

#define 	MAT_TYPE 					mat_int
#define 	DISP_ERR(str) 				fprintf(stderr, "%s \n", str)
#define 	CHECK(row, col) 			((row > 0 && col > 0) ? 1 : 0)

typedef struct matrix_struct_info {
	MAT_TYPE *pMat; //point to a two-dimensional matrix
	mat_short row;
	mat_short col;
}matrix_t, *matrix_ptr;

typedef enum {
	MAT_OK = 0,
	MAT_ERR,
}MAT_STATUS;

matrix_ptr matrix_create(mat_short row, mat_short col);
MAT_STATUS matrix_insert(matrix_ptr matrix, mat_short row, \
	mat_short col, MAT_TYPE elem);
MAT_STATUS matrix_disp(matrix_ptr matrix);
MAT_STATUS matrix_free(matrix_ptr matrix);

#endif

