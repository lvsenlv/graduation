/*************************************************************************
	> File Name: matrix.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时37分30秒
 ************************************************************************/

#ifndef _MATRIX_H
#define _MATRIX_H

typedef 	int 				matrix_int;
typedef 	long long 			matrix_long;
typedef 	float 				matrix_float;
typedef 	double 				matrix_double;

#define 	MATRIX_TYPE 		matrix_int

typedef struct matrix_info{
	MATRIX_TYPE *pMatrix;
	int row;
	int col;
}matrx_t;

#endif

