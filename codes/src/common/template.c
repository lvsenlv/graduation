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
        
       0 0 0 0
       0 1 1 0
       0 1 1 0
       0 1 1 0
lin1   0 1 1 0    line3
       0 1 1 0
       0 1 1 0
       0 0 0 0
       
        line4
*/
_matrix_pst template_zero(_MAT_COL wid)
{
    _mat_short ratio = wid / BASE_WID; //thickness of zero
    if(!ratio)
        return NULL;
    
    _matrix_pst mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }
    
    _MAT_SIZE size = wid * ratio; //for drawing line2 and 4
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix 
    _MAT_ROW row_bg = wid*2 - ratio*2; //row of background
    _MAT_COL col_bg = wid - ratio*2; //col of background
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

/* width : height = 1 : 4
        
       1 1 1 0          
       1 1 1 0      |   direction
       1 1 1 0      |   
       1 1 1 0      |
       1 1 1 0      |   draw the row one by one
       1 1 1 0      |
       1 1 1 0      V
       1 1 1 0
       
*/
_matrix_pst template_one(_MAT_COL wid)
{
    _mat_short ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pst mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }

    _MAT_ROW row = wid * 2;
    _MAT_COL col_bg = wid - ratio; //col of background
    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = 1;
        }
        
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
    }

    return mat;
}

/* width : height = 1 : 4

        line1
        
       0 0 0 0
       1 1 1 0
       1 1 1 0     line2
       1 1 1 0
       0 0 0 0     line3
       0 1 1 1
line4  0 1 1 1
       0 0 0 0

        line5
*/
_matrix_pst template_two(_MAT_COL wid)
{
    _mat_short ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pst mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and 5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = 0;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; // low half of row of background
    _MAT_ROW row_bg_hig_hf = 
       (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; // first half of row of background
    _MAT_COL col_bg = wid - ratio;
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = 1;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = 0;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = 1;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = 0;
    }

    return mat;
}


/* width : height = 1 : 4

        line1
        
       0 0 0 0
       1 1 1 0
       1 1 1 0     line2
       1 1 1 0
       0 0 0 0     line3
       1 1 1 0
       1 1 1 0     line4
       0 0 0 0

        line5
*/
_matrix_pst template_three(_MAT_COL wid)
{
    _mat_short ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pst mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and 5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = 0;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; // low half of row of background
    _MAT_ROW row_bg_hig_hf = 
       (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; // first half of row of background
    _MAT_COL col_bg = wid - ratio;
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = 1;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = 0;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = 1;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = 0;
    }

    return mat;
}

/* width : height = 1 : 4

line1  0 1 1 0
  |    0 1 1 0
  |    0 1 1 0
  |    0 1 1 0
  V    0 0 0 0    line3
       1 1 1 0
       1 1 1 0    line4
       1 1 1 0
       
*/
_matrix_pst template_four(_MAT_COL wid)
{
    _mat_short ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pst mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR("error in matrix_create");
        return NULL;
    }

    _MAT_ROW row_bg_hig_hf = (wid*2 - ratio)/2; //first row of background
    _MAT_ROW row_bg_low_hf = wid*2 - row_bg_hig_hf - ratio; //low row of background
    _MAT_COL col_bg_hig_hf = wid - ratio*2; //first col of background
    _MAT_COL col_bg_low_hf = wid - ratio; //low col of background
    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    _MAT_SIZE pos = 0; //current index of matrix
    _MAT_SIZE size = wid * ratio; //for drawing line3
    
    for(i = 0; i < row_bg_hig_hf; i++) //draw line1
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
        for(j = 0; j < col_bg_hig_hf; j++)
        {
            mat->pMat[pos++] = 1;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
    }
    
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = 0;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < col_bg_low_hf; j++)
        {
            mat->pMat[pos++] = 1;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = 0;
        }
    }
    
    return mat;
}
    