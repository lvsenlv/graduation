/*************************************************************************
	> File Name: template.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月07日 星期二 13时33分15秒
 ************************************************************************/

#include "template.h"
#include "matlab.h"

/* width : height = 1 : 4

        line2
         ____
        |    |
        |    |
 lin1   |    |   line3
        |____|

        line4
*/

_matrix_pst template_zero(_MAT_COL wid)
{
    _matrix_pst mat = NULL;
    mat = matrix_create(2*wid, wid);
    if(!mat)
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }
    
    _mat_short ratio = wid / 4; //thickness of zero
    if(!ratio)
        return NULL;
    
    _MAT_SIZE size = ratio * wid; //for drawing line2 and 4
    _MAT_SIZE i = 0;
    _MAT_ROW row_bg = wid*2 - ratio*2; //row of backround
    _MAT_COL col_bg = wid - ratio*2; //col of backround
    _MAT_SIZE pos = 0; //current index of matrix 
    _MAT_COL j = 0;

    for(i = 0; i < size; i++) //draw line2
    {
        mat->pMat[pos++] = 0;
    }

    for(i = 0; i < row_bg; i++)
    {
        for(j = 0; j < ratio; j++) //draw line1
        {
            mat->pMat[pos++] = 0;
        }

        for(j = 0; j < col_bg; j++) //draw background
        {
            mat->pMat[pos++] = 1;
        }

        for(j = 0; j < ratio; j++) //draw line3
        {
            mat->pMat[pos++] = 0;
        }
    }

    for(i = 0; i < size; i++) //draw line4
    {
        mat->pMat[pos++] = 0;
    }
    
    return mat;
    
}

