/*************************************************************************
	> File Name: template.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月07日 星期二 13时33分15秒
 ************************************************************************/

#include "template.h"

_matrix_pt g_number_mat[10] = {NULL};

/* width : height = 1 : 4

        line2
        
       1 1 1 1
       1 0 0 1
       1 0 0 1
       1 0 0 1
lin1   1 0 0 1    line3
       1 0 0 1
       1 0 0 1
       1 1 1 1
       
        line4
*/
_matrix_pt template_zero(_MAT_COL wid)
{
    uint16_t ratio = wid / BASE_WID; //thickness of zero
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }
    
    _MAT_SIZE size = wid * ratio; //for drawing line2 and line4
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix 
    _MAT_ROW row_bg = wid*2 - ratio*2; //row of background
    _MAT_COL col_bg = wid - ratio*2; //col of background
    _MAT_COL j = 0;

    for(i = 0; i < size; i++) //draw line2
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg; i++)
    {
        for(j = 0; j < ratio; j++) //draw line1
        {
            mat->pMat[pos++] = FOREGROUND;
        }

        for(j = 0; j < col_bg; j++) //draw background
        {
            mat->pMat[pos++] = BACKGROUND;
        }

        for(j = 0; j < ratio; j++) //draw line3
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line4
    {
        mat->pMat[pos++] = FOREGROUND;
    }
    
    return mat;    
}

/* width : height = 1 : 4
        
       0 0 0 1          
       0 0 0 1      |   direction
       0 0 0 1      |   
       0 0 0 1      |
       0 0 0 1      |   draw the row one by one
       0 0 0 1      |
       0 0 0 1      V
       0 0 0 1
       
*/
_matrix_pt template_one(_MAT_COL wid)
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
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
            mat->pMat[pos++] = BACKGROUND;
        }
        
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    return mat;
}

/* width : height = 1 : 4

        line1
        
       1 1 1 1
       0 0 0 1
       0 0 0 1     line2
       0 0 0 1
       1 1 1 1     line3
       1 0 0 0
line4  1 0 0 0
       1 1 1 1

        line5
*/
_matrix_pt template_two(_MAT_COL wid)
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and line5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; //low half of row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; //first half of row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio*3 - row_bg_low_hf; //first half of row of background
    _MAT_COL col_bg = wid - ratio;
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    return mat;
}

/* width : height = 1 : 4

        line1
        
       1 1 1 1
       0 0 0 1
       0 0 0 1     line2
       0 0 0 1
       1 1 1 1     line3
       0 0 0 1
       0 0 0 1     line4
       1 1 1 1

        line5
*/
_matrix_pt template_three(_MAT_COL wid) //copy from template_two
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and line5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; //low half of row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; //first half of row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio*3 - row_bg_low_hf; //first half of row of background
    _MAT_COL col_bg = wid - ratio;
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    return mat;
}

/* width : height = 1 : 4

line1  1 0 0 1
  |    1 0 0 1
  |    1 0 0 1
  |    1 0 0 1
  V    1 1 1 1    line3
       0 0 0 1
       0 0 0 1    line4
       0 0 0 1
       
*/
_matrix_pt template_four(_MAT_COL wid)
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    //_MAT_ROW row_bg_low_hf = (wid*2 - ratio)/2; //low row of background
    _MAT_ROW row_bg_low_hf = (wid*2 - ratio)/2; //low row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ? row_bg_low_hf + 1 : row_bg_low_hf; //first row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio - row_bg_low_hf; //first row of background
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
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg_hig_hf; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }
    
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < col_bg_low_hf; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }
    
    return mat;
}

/* width : height = 1 : 4

        line1
        
       1 1 1 1
       1 0 0 0
       1 0 0 0     line2
       1 0 0 0
       1 1 1 1     line3
       0 0 0 1
       0 0 0 1     line4
       1 1 1 1

        line5
*/
_matrix_pt template_five(_MAT_COL wid) //copy from template_three
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and line5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; //low half of row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; //first half of row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio*3 - row_bg_low_hf; //first half of row of background
    _MAT_COL col_bg = wid - ratio;
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    return mat;
}

/* width : height = 1 : 4

        line1
        
       1 1 1 1
       1 0 0 0
       1 0 0 0     line2
       1 0 0 0
       1 1 1 1     line3
       1 0 0 1
       1 0 0 1     line4
       1 1 1 1

        line5
*/
_matrix_pt template_six(_MAT_COL wid) //copy from template_five
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and line5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; //low half of row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; //first half of row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio*3 - row_bg_low_hf; //first half of row of background
    _MAT_COL col_bg_hig_hf = wid - ratio; //first half of col of background
    _MAT_COL col_bg_low_hf = wid - ratio*2; //low half of col of background
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg_hig_hf; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg_low_hf; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    return mat;
}

/* width : height = 1 : 4

        line1  
        
       1 1 1 1          
       0 0 0 1    line2   
       0 0 0 1      |   direction
       0 0 0 1      |   
       0 0 0 1      |   draw the row one by one
       0 0 0 1      |
       0 0 0 1      V
       0 0 0 1
       
*/
_matrix_pt template_seven(_MAT_COL wid) //copy from template_one
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1
    _MAT_SIZE i = 0;
    _MAT_ROW row = wid * 2 - ratio; //for drawing line2
    _MAT_COL col_bg = wid - ratio; //col of background
    _MAT_COL j = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }
    
    for(i = 0; i < row; i++) //draw line2
    {
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    return mat;
}

/* width : height = 1 : 4

        line1
        
       1 1 1 1
       1 0 0 1
       1 0 0 1     line2
       1 0 0 1
       1 1 1 1     line3
       1 0 0 1
       1 0 0 1     line4
       1 1 1 1

        line5
*/
_matrix_pt template_eight(_MAT_COL wid) //copy from template_six
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and line5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; //low half of row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; //first half of row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio*3 - row_bg_low_hf; //first half of row of background
    _MAT_COL col_bg = wid - ratio*2; //col of background
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    return mat;
}

/* width : height = 1 : 4

        line1
        
       1 1 1 1
       1 0 0 1
       1 0 0 1     line2
       1 0 0 1
       1 1 1 1     line3
       0 0 0 1
       0 0 0 1     line4
       1 1 1 1

        line5
*/
_matrix_pt template_nine(_MAT_COL wid) //copy from template_six
{
    uint16_t ratio = wid / BASE_WID; //thickness of num
    if(!ratio)
        return NULL;
    
    _matrix_pt mat = NULL;
    mat = matrix_create(wid*2, wid);
    if(!mat)
    {
        DISP_ERR(ERR_CREATE);
        return NULL;
    }

    _MAT_SIZE size = wid * ratio; //for drawing line1 and line5
    _MAT_SIZE i = 0;
    _MAT_SIZE pos = 0; //current index of matrix

    for(i = 0; i < size; i++) //draw line1
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    _MAT_ROW row_bg_low_hf = (wid*2 - ratio*3)/2; //low half of row of background
    //_MAT_ROW row_bg_hig_hf = (wid%8) ?  row_bg_low_hf + 1 : row_bg_low_hf; //first half of row of background
    _MAT_ROW row_bg_hig_hf = wid*2 - ratio*3 - row_bg_low_hf; //first half of row of background
    _MAT_COL col_bg_hig_hf = wid - ratio*2; //first half of col of background
    _MAT_COL col_bg_low_hf = wid - ratio; //low half of col of background
    _MAT_COL j = 0;

    for(i = 0; i < row_bg_hig_hf; i++) //draw line2
    {
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
        for(j = 0; j < col_bg_hig_hf; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }
        
    for(i = 0; i < size; i++) //draw line3
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    for(i = 0; i < row_bg_low_hf; i++) //draw line4
    {
        for(j = 0; j < col_bg_low_hf; j++)
        {
            mat->pMat[pos++] = BACKGROUND;
        }
        for(j = 0; j < ratio; j++)
        {
            mat->pMat[pos++] = FOREGROUND;
        }
    }

    for(i = 0; i < size; i++) //draw line5
    {
        mat->pMat[pos++] = FOREGROUND;
    }

    return mat;
}

_MAT_STATUS template_create(_MAT_COL wid)
{
    _matrix_pt mat_ret = NULL;
    
    mat_ret = template_zero(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_CREATE);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[1] = mat_ret;

/*
    mat_ret = template_one(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[1] = mat_ret;
*/
    
    mat_ret = template_two(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[2] = mat_ret;
    
    mat_ret = template_three(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[3] = mat_ret;
    
    mat_ret = template_four(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[4] = mat_ret;
    
    mat_ret = template_five(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[5] = mat_ret;
    
    mat_ret = template_six(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[6] = mat_ret;
    
    mat_ret = template_seven(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[7] = mat_ret;
    
    mat_ret = template_eight(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[8] = mat_ret;
    
    mat_ret = template_nine(wid);
    if(!mat_ret)
    {
#ifdef __DEBUG
        DISP_ERR(ERR_FATAL);
#endif //__DEBUG
        return MAT_ERR;
    }
    g_number_mat[9] = mat_ret;

    return MAT_OK;
}

void template_free(void)
{ 
    _VOL uint16_t i = 0 ;

    for(i = 1; i < 10; i++)
    {
        /*
        //fllowing action costs several times due to access memory
        if(g_number_mat[i])
        {
            if(g_number_mat[i]->pMat)
            {
                free(g_number_mat[i]->pMat);
            }
            free(g_number_mat[i]);
        }*/
        
        //fllowing action costs several times due to push and pop stack
        matrix_free(g_number_mat[i]);  
        g_number_mat[i] = NULL;
    }
}

void template_disp(void)
{
    _VOL uint16_t i = 0 ;

    for(i = 1; i < 10; i++)
    {
#ifdef __DEBUG
        if(!g_number_mat[i])
        {
            DISP_ERR_PLUS("[%s][%d]: error in g_number_mat[%d] \n", \
                __func__, __LINE__, i);
            continue;
        }
#endif //__DEBUG
        matrix_disp(g_number_mat[i]);
        DISP("\n");
    }
}

