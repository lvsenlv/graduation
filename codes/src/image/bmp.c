/*************************************************************************
	> File Name: bmp.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 19时14分11秒
 ************************************************************************/

#include "bmp.h"
#include <stdlib.h>
#include "template.h"

_bmp_pt bmp_create(void)
{
    _bmp_pt bmp = (_bmp_pt)malloc(sizeof(_bmp_t));
    if(!bmp)
    {
        DISP_ERR(ERR_MALLOC);
        return NULL;
    }
    
    bmp->head_info = (_matrix_pt)malloc(sizeof(_matrix_t));
    if(!bmp->head_info)
    {
        DISP_ERR(ERR_MALLOC);
        free(bmp);
        bmp = NULL;
        return NULL;
    }
    
#ifdef __DEBUG
    bmp->vital_info = (_bmp_vital_pt)calloc(1, sizeof(_bmp_vital_t));
#else //__DEBUG
    bmp->vital_info = (_bmp_vital_pt)malloc(sizeof(_bmp_vital_t));
#endif //__DEBUG
    if(!bmp->vital_info)
    {
        DISP_ERR(ERR_MALLOC);
        free(bmp->head_info);
        free(bmp);
        bmp = NULL;
        return NULL;
    }
    
    bmp->data_info = (_matrix_pt)malloc(sizeof(_matrix_t));
    if(!bmp->data_info)
    {
        DISP_ERR(ERR_MALLOC);
        free(bmp->head_info);
        free(bmp->vital_info);
        free(bmp);
        bmp = NULL;
        return NULL;
    }

    bmp->head_info->pMat = NULL;
    bmp->vital_info->reserved4 = NULL;
    bmp->data_info->pMat = NULL;

    return bmp;
}

_G_STATUS bmp_head_parse(_bmp_pt bmp, uint8_t *ptr)
{
#ifdef __DEBUG
    if(!ptr)
    {
        DISP_ERR("invalid ptr");
        return STAT_ERR;
    }
    
    if(!bmp || !bmp->head_info || !bmp->vital_info || !bmp->data_info)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }

#endif //__DEBUG

    if( ('B' != ptr[0]) || ('M' != ptr[1]) )
    {
        
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    bmp->head_info->pMat = ptr;
    bmp->head_info->row = 1;
    bmp->head_info->col = HEAD_INFO_SIZE;

    //get vital info
    _bmp_vital_pt vital = bmp->vital_info;
    
    uint32_t width = 0;
    width |= ptr[21];
    width <<= 8;
    width |= ptr[20];
    width <<= 8;
    width |= ptr[19];
    width <<= 8;
    width |= ptr[18];
    vital->width = width;

    uint32_t height = 0;
    height |= ptr[25];
    height <<= 8;
    height |= ptr[24];
    height <<= 8;
    height |= ptr[23];
    height <<= 8;
    height |= ptr[22];
    vital->height = height;
    
    uint16_t bit_count = 0;
    bit_count <<= 8;
    bit_count |= ptr[29];
    bit_count <<= 8;
    bit_count |= ptr[28];
    vital->bit_count = bit_count;

    uint32_t real_size = 0;
    real_size |= ptr[37];
    real_size <<= 8;
    real_size |= ptr[36];
    real_size <<= 8;
    real_size |= ptr[35];
    real_size <<= 8;
    real_size |= ptr[34];
    vital->real_size = real_size;

    vital->real_width = COUNT_REAL_WIDTH(width*bit_count);
    
    return STAT_OK;
}

void bmp_free(_bmp_pt bmp)
{
    if(bmp)
    {
        matrix_free(bmp->head_info);
        if(bmp->vital_info->reserved4)
            free(bmp->vital_info->reserved4);
        free(bmp->vital_info);
        matrix_free(bmp->data_info);
        free(bmp);
    }
}

void bmp_viatl_info_disp(_bmp_pt bmp)
{
    DISP("width=%d \n", bmp->vital_info->width);
    DISP("height=%d \n", bmp->vital_info->height);
    DISP("bit_count=%d\n", bmp->vital_info->bit_count);
    DISP("real_size=%d \n", bmp->vital_info->real_size);
    DISP("real_width=%d \n", bmp->vital_info->real_width);
}

_G_STATUS bmp_data_parse(_bmp_pt bmp, uint8_t *ptr)
{
#ifdef __DEBUG
    if(!ptr)
    {
        DISP_ERR("invalid ptr");
        return STAT_ERR;
    }
    
    if(!bmp || !bmp->data_info || !bmp->vital_info)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }

    if(!bmp->vital_info->height || !bmp->vital_info->real_width)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    bmp->data_info->pMat = ptr;
    bmp->data_info->row = bmp->vital_info->height;
    bmp->data_info->col = bmp->vital_info->real_width;
    
    return STAT_OK;
}

_bmp_pt bmp_convert_gray(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)  
    {
        DISP_ERR(ERR_BMP);
        return NULL;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return NULL;
    }
#endif //__DEBUG

    if(bmp->vital_info->bit_count != 24)
    {
        DISP_ERR("only support 24 bits depth bmp image");
        return NULL;
    }
    
    _bmp_pt bmp_ret = bmp_create();
    if(!bmp_ret)
    {
        DISP_ERR("error in bmp_create");
        return NULL;
    }

    uint8_t *head_info_ret = (uint8_t *)malloc(HEAD_INFO_SIZE);
    if(!head_info_ret)
    {
        DISP_ERR(ERR_MALLOC);
        bmp_free(bmp_ret);
        bmp_ret = NULL;
        return NULL;
    }
    bmp_ret->head_info->pMat = head_info_ret;
    bmp_ret->head_info->row = 1;
    bmp_ret->head_info->col = HEAD_INFO_SIZE;

    _bmp_vital_pt vital = bmp->vital_info;
    uint32_t width = vital->width;
    uint32_t height = vital->height;
    //only save the data of B, data of G and R are the same with B 
    uint8_t *data_info = (uint8_t *)malloc(width*height);
    if(!data_info)
    {
        DISP_ERR(ERR_MALLOC);
        bmp_free(bmp_ret);
        bmp_ret = NULL;
        return NULL;
    }
    bmp_ret->data_info->pMat = data_info;
    bmp_ret->data_info->row = height;
    bmp_ret->data_info->col = width;

    //copy head info to new bmp image
    uint32_t i = 0;
    uint8_t *tmp_ptr = bmp->head_info->pMat;
    for(i = 0; i < HEAD_INFO_SIZE; i++)
    {
        *head_info_ret++ = *tmp_ptr++;
    }

    //copy vital info to new bmp image
    _bmp_vital_pt vital_ret = bmp_ret->vital_info;
    uint32_t real_width = vital->real_width;
    vital_ret->width = width;
    vital_ret->height = height;
    vital_ret->bit_count = vital->bit_count;
    vital_ret->real_size = vital->real_size;
    vital_ret->real_width = real_width;

    //convert to gray image, algorithm: Gray = (R*38 + G*75 + B*15) >> 7
    uint32_t j = 0;
    uint32_t fill_pixel_num = real_width - ((width * vital->bit_count) >> 3);
    tmp_ptr = bmp->data_info->pMat;

    if(0 == fill_pixel_num) 
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data save by B,G,R format
                *data_info++  = 
                    (tmp_ptr[2]*38 + tmp_ptr[1]*75 + tmp_ptr[0]*15) >> 7;
                tmp_ptr += 3;
            }
        }
    }
    else
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data save by B,G,R format
                *data_info++  = 
                    (tmp_ptr[2]*38 + tmp_ptr[1]*75 + tmp_ptr[0]*15) >> 7;
                tmp_ptr += 3;
            }
            
            //fill pixel due to align of 4 byte
            for(j = 0; j < fill_pixel_num; j++)
            {
                tmp_ptr ++;
            }
        }
    }
    
    return bmp_ret;
}

//this algorithm costs much time in loop
_G_STATUS bmp_median_filter_slow(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)  
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint8_t model[FILTER_MODEL_SIZE*FILTER_MODEL_SIZE];
    
    uint32_t width = bmp->vital_info->width;
    uint32_t height = bmp->vital_info->height;
    
    _MAT_ROW row, row_end = height - FILTER_OFFSET;
    _MAT_COL col, col_end = width - FILTER_OFFSET;
    _MAT_ROW i = 0, i_end = 0 ;
    _MAT_COL j = 0, j_end = 0;
    
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    uint8_t *model_ptr = NULL;
    uint8_t tmp = 0, flag = 0, bubble_times = 0;
    uint32_t total_times = 0;

    for(row = FILTER_OFFSET; row < row_end; row++)
    {
        for(col = FILTER_OFFSET; col < col_end; col++)
        {
            i_end = row + FILTER_OFFSET;
            j_end = col + FILTER_OFFSET;
            model_ptr = model;
            for(i = row - FILTER_OFFSET; i <= i_end; i++)
            {
                for(j = col - FILTER_OFFSET; j <= j_end; j++)
                {
                    *model_ptr++ = data_ptr[width*i+j];
                    total_times++;
                }
            }

            bubble_times = FILTER_MODEL_SIZE*FILTER_MODEL_SIZE - 1;
            do
            {
                flag = 0;
                for(j = 0; j < bubble_times; j++)
                {
                    if(model[j] < model[j+1])
                    {  
                        tmp = model[j];
                        model[j] = model[j+1];
                        model[j+1] = tmp;
                        flag = 1;
                    }
                    total_times++;
                }
                bubble_times--;
            }while(flag);

            data_ptr[width*row + col] = model[FILTER_OFFSET_PLUS];
        }
    }
    
    DISP("%d \n", total_times);
    return STAT_OK;
}

_G_STATUS bmp_median_filter(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)  
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint8_t model[FILTER_MODEL_SIZE];
    uint8_t buf[FILTER_MODEL_SIZE];
    
    uint32_t width = bmp->vital_info->width;
    uint32_t height = bmp->vital_info->height;
    
    _MAT_ROW row, row_end = height - FILTER_OFFSET;
    _MAT_COL col, col_end = width - FILTER_OFFSET;
    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    _MAT_TYPE *tmp_ptr = NULL;
    uint8_t *model_ptr = NULL;
    uint8_t tmp = 0, flag = 0, bubble_times = 0;
    uint32_t total_times = 0;

    for(row = FILTER_OFFSET; row < row_end; row++)
    {
        for(col = FILTER_OFFSET; col < col_end; col++)
        {
            tmp_ptr = data_ptr + width*(row - FILTER_OFFSET) + col - FILTER_OFFSET;

            for(i = 0; i < FILTER_MODEL_SIZE; i++)
            {
                model_ptr = model;
                //set a row data to model
                for(j = 0; j < FILTER_MODEL_SIZE; j++)
                {
                    *model_ptr++ = tmp_ptr[j];
                    total_times++;
                }
                
                //bubble sort
                bubble_times = FILTER_MODEL_SIZE - 1; 
                do
                {
                    flag = 0;
                    for(j = 0; j < bubble_times; j++)
                    {
                        if(model[j] < model[j+1])
                        {  
                            tmp = model[j];
                            model[j] = model[j+1];
                            model[j+1] = tmp;
                            flag = 1;
                        }
                        total_times++;
                    }
                    bubble_times--;
                }while(flag);

                buf[i] = model[FILTER_OFFSET];
                
                tmp_ptr += width;
            }

            bubble_times = FILTER_MODEL_SIZE - 1; 
            do
            {
                flag = 0;
                for(i = 0; i < bubble_times; i++)
                {
                    if(buf[i] < buf[i+1])
                    {  
                        tmp = buf[i];
                        buf[i] = buf[i+1];
                        buf[i+1] = tmp;
                        flag = 1;
                    }
                    total_times++;
                }
                bubble_times--;
            }while(flag);

            data_ptr[width*row + col] = buf[FILTER_OFFSET];
        }
    }
    DISP("%s cycle index is: %d \n", __func__, total_times);
    return STAT_OK;
}
//#endif

//efficient algorithm when model size is 3
_G_STATUS bmp_median_filter_fast(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)  
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint8_t model[3];
    
    uint32_t width = bmp->vital_info->width;
    uint32_t height = bmp->vital_info->height;
    
    _MAT_ROW row, row_end = height - 1;
    _MAT_COL col, col_end = width - 1;
    uint8_t i = 0;
    
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    _MAT_TYPE *tmp_ptr = NULL;
    uint32_t total_times = 0;

#ifdef __DEBUG
    uint8_t num1 = 0, num2 = 0, num3 =0;
#endif //__DEBUG

    for(row = 1; row < row_end; row++)
    {
        for(col = 1; col < col_end; col++)
        {
            tmp_ptr = data_ptr + width*(row - 1) + col -1;
            for(i = 0; i < 3; i++)
            {
#ifdef __DEBUG
                num1 = tmp_ptr[0];
                num2 = tmp_ptr[1];
                num3 = tmp_ptr[2];
                model[i] = MIDDLE(num1, num2, num3);
#else
                model[i] = MIDDLE(tmp_ptr[0], tmp_ptr[1], tmp_ptr[2]);
#endif //__DEBUG
                tmp_ptr += width;
                total_times++;
            }

#ifdef __DEBUG
            num1 = model[0];
            num2 = model[1];
            num3 = model[2];
            data_ptr[width*row + col] = MIDDLE(num1, num2, num3);
#else
            data_ptr[width*row + col] = MIDDLE(model[0], model[1], model[2]);
#endif //__DEBUG
            
        }
    }
    
    //DISP("%d \n", total_times);
    return STAT_OK;
}

//formula: var=(uk - average*wk)^2 / (wk*(1-wk))
_G_STATUS bmp_get_threshold(_bmp_pt bmp, uint8_t *threshold)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)  
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint32_t size = bmp->vital_info->height * bmp->vital_info->width;
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    float histogram[256] = {0};
    uint32_t i = 0;
    
    for(i = 0; i < size; i++)
    {
        histogram[data_ptr[i]]++;
    }

    float average = 0;
    for(i = 0; i < 256; i++)  
    {  
        histogram[i] = histogram[i] / size;
        average  += i * histogram[i];
    }

    float var = 0;
    float var_max = 0;
    float wk = 0, uk = 0, ut = 0; 
    
    for(i = 0; i < 256; i++)
    {
        wk += histogram[i];  
        uk += i * histogram[i];       
  
        ut = average *wk - uk;
        var = ut*ut / (wk * (1 - wk));    
          
        if(var > var_max)  
        {  
            var_max = var;  
            *threshold = i;  
        }         
    }
    
    return STAT_OK;
}

_G_STATUS bmp_convert_binary(_bmp_pt bmp, uint8_t threshold)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint32_t size = bmp->vital_info->width * bmp->vital_info->height;
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    uint32_t i = 0;

    for(i = 0; i < size; i++)
    {
        //*data_ptr = ((*data_ptr < threshold) ? 0x0 : 0xFF);
        *data_ptr = ((*data_ptr < threshold) ? 0 : 1); //0 is background, 1 is foreground
        data_ptr++;
    }

    return STAT_OK;
}

_G_STATUS bmp_dilate(_bmp_pt bmp, uint8_t struct_width)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)  
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint32_t width = bmp->vital_info->width;
    uint32_t height = bmp->vital_info->height;

    uint8_t remain = struct_width - 1;
    _MAT_ROW row, row_start = height - 1;
    _MAT_COL col, col_end = width - remain;
    uint8_t i = 0, j = 0;
    
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    _MAT_TYPE *tmp_ptr = NULL;
    uint8_t flag = 0;
    
/*
    tmp_ptr = data_ptr;
    for(row = 0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {
            DISP("%d ", tmp_ptr[col]);
        }
        tmp_ptr += width;
        DISP("\n");
    }
    DISP("\n");
*/

    //dilation direction: from bottom to top, left to right
    for(row = row_start; row >= remain; row--)
    {
        for(col = 0; col < col_end; col++)
        {
            flag = 0;
            tmp_ptr = data_ptr + width*row + col;
            if(*tmp_ptr)
                continue;
            
            for(i = 0; i < struct_width; i++)
            {
                for(j = 0; j < struct_width; j++)
                {
                    flag |= tmp_ptr[j];
                }
                tmp_ptr -= width;
            }            
            data_ptr[width*row + col] = flag;
        }
    }

/*
    //dilate the rest pixels, the rest width is remain
    for(row = 0; row < remain; row++)
    {
        for(col = 0; col < col_end; col++)
        {
            flag = 0;
            tmp_ptr = data_ptr + width*row + col;
            if(*tmp_ptr)
                continue;
            
            for(i = 0; i < struct_width; i++)
            {
                for(j = 0; j < struct_width; j++)
                {
                    //DISP("%d ", tmp_ptr[j]);
                    flag |= tmp_ptr[j];
                }
                tmp_ptr += width;
            }            
            data_ptr[width*row + col] = flag;
        }
        DISP("\n");
    }


    tmp_ptr = data_ptr;
    for(row = 0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {
            DISP("%d ", tmp_ptr[col]);
        }
        tmp_ptr += width;
        DISP("\n");
    }
*/
    
    return STAT_OK;
}

_G_STATUS bmp_digit_row_locate(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint32_t col = bmp->vital_info->width;
    uint32_t row = bmp->vital_info->height;

    uint8_t *row_sum_array = (uint8_t *)malloc(row);
    if(!row_sum_array)
    {
        DISP_ERR(ERR_MALLOC);
        return STAT_ERR;
    }

    uint32_t *row_start_array = (uint32_t *)malloc(row*sizeof(uint32_t));
    if(!row_start_array)
    {
        DISP_ERR(ERR_MALLOC);
        free(row_sum_array);
        return STAT_ERR;
    }

    uint32_t *row_end_array = (uint32_t *)malloc(row*sizeof(uint32_t));
    if(!row_end_array)
    {
        DISP_ERR(ERR_MALLOC);
        free(row_sum_array);
        free(row_start_array);
        return STAT_ERR;
    }

    uint32_t *tmp_array = (uint32_t *)malloc(row*sizeof(uint32_t));
    if(!tmp_array)
    {
        DISP_ERR(ERR_MALLOC);
        free(row_sum_array);
        free(row_start_array);
        free(row_end_array);
        return STAT_ERR;
    }

    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    for(i = 0; i < row; i++)
    {
        row_sum_array[i] = 0;
        for(j = 0; j < col; j++)
        {
            row_sum_array[i] |= data_ptr[j];
        }
        data_ptr += col;
    }

    uint32_t tmp = row - 1;
    uint32_t row_start_array_pos = 0, row_end_array_pos = 0, tmp_array_pos = 0;
    for(i = 0; i < row; i++) //start from the second row
    {
        row_start_array[i] = 0;
        row_end_array[i] = 0;
        
        if( 0 != i)
        {
            if(i != tmp)
            {
                if( (1 == row_sum_array[i]) && (0 == row_sum_array[i-1]) )
                {
                    row_start_array[row_start_array_pos++] = i;
                }
                if( (0 == row_sum_array[i]) && ( 1 == row_sum_array[i-1]) )
                {
                    row_end_array[row_end_array_pos++] = i;
                    
                    tmp_array[tmp_array_pos++] = row_start_array[--row_start_array_pos];
                    tmp_array[tmp_array_pos++] = i;
                    tmp_array[tmp_array_pos++] = \
                        i - row_start_array[row_start_array_pos++];
                }
            }
            else
            {
                if( row_sum_array[tmp] && row_sum_array[tmp-1] )
                {
                    row_end_array[row_end_array_pos++] = tmp;
                    tmp_array[tmp_array_pos++] = row_start_array[--row_start_array_pos];
                    tmp_array[tmp_array_pos++] = i;
                    tmp_array[tmp_array_pos++] = \
                        i - row_start_array[row_start_array_pos++];
                }
            } 
        }
        else
        {
            if( (1 == row_sum_array[0]) && (1 == row_sum_array[1]) )
            {
                row_start_array[row_start_array_pos++] = 0;
            }
            else if( (1 == row_sum_array[0]) && (0 == row_sum_array[1]) )
            {
                i++;
            }
        }
    }

    if(row_start_array_pos != row_end_array_pos)
    {
        DISP_ERR(ERR_FATAL);
        free(row_sum_array);
        free(row_start_array);
        free(row_end_array);
        free(tmp_array);
        return STAT_ERR;
    }

    //if all the pixels is 0 - a whole black image
    if(0 == row_start_array_pos && 0 == row_end_array_pos)
    {
        DISP_ERR(ERR_FATAL);
        free(row_sum_array);
        free(row_start_array);
        free(row_end_array);
        free(tmp_array);
        return STAT_ERR;
    }

    uint32_t coordinate;
    uint32_t max = tmp_array[--tmp_array_pos];
    coordinate = tmp_array_pos;
    tmp_array_pos -= 2;
    
    while(tmp_array_pos)
    {
        tmp_array_pos--;
        if(max < tmp_array[tmp_array_pos])
        {
            max = tmp_array[tmp_array_pos];
            coordinate = tmp_array_pos;
        }
        tmp_array_pos -= 2;
    }

    bmp->vital_info->reserved2= tmp_array[coordinate-2];
    bmp->vital_info->reserved3 = tmp_array[coordinate-1];

    free(row_sum_array);
    free(row_start_array);
    free(row_end_array);
    free(tmp_array);
    return STAT_OK;
}

_G_STATUS bmp_digit_col_locate(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint32_t row_start = bmp->vital_info->reserved2;
    uint32_t row_diff = bmp->vital_info->reserved3 - row_start;
    uint32_t col = bmp->vital_info->width;
    uint32_t *col_sum_array = (uint32_t *)malloc(col*sizeof(uint32_t));
    if(!col_sum_array)
    {
        DISP_ERR(ERR_MALLOC);
        return STAT_ERR;
    }

    uint32_t *col_array = (uint32_t *)malloc(col*sizeof(uint32_t));
    if(!col_array)
    {
        DISP_ERR(ERR_MALLOC);
        free(col_sum_array);
        return STAT_ERR;
    }
    bmp->vital_info->reserved4 = col_array;

    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    _MAT_TYPE *tmp_data_ptr = data_ptr;
    uint32_t *tmp_ptr = NULL;
    _MAT_ROW i = 0;
    _MAT_COL j = 0;
    for(j = 0; j < col; j++)
    {
        tmp_data_ptr = data_ptr + col*row_start + j;
        tmp_ptr = col_sum_array + j;
        *tmp_ptr = 0;
        for(i = 0; i < row_diff; i++)
        {
            *tmp_ptr |= *tmp_data_ptr;
            tmp_data_ptr += col;
        }
    }

    uint32_t tmp = col - 1;
    uint32_t col_array_pos = 0;
    for(j = 0; j < col; j++) //start from the second column
    {
        if( 0 != j)
        {
            if(j != tmp)
            {
                //column start coordinate
                if( (1 == col_sum_array[j]) && (0 == col_sum_array[j-1]) )
                {
                    col_array[col_array_pos++] = j;
                }
                //column end coordinate
                if( (0 == col_sum_array[j]) && ( 1 == col_sum_array[j-1]) )
                {
                    col_array[col_array_pos++] = j;
                    col_array[col_array_pos] = COL_ARRAY_END; //the end of col_array
                }
            }
            else
            {
                if( col_sum_array[tmp] && col_sum_array[tmp-1] )
                {
                    col_array[col_array_pos++] = tmp;
                    col_array[col_array_pos] = COL_ARRAY_END; //the end of col_array
                }
            }
        }
        else
        {
            if(col_sum_array[0] && col_sum_array[1])
            {
                col_array[col_array_pos++] = 0;
            }
            else if( (1 == col_sum_array[0]) && (0 == col_sum_array[1]) )
            {
                col_array[col_array_pos++] = 0;
            }
        }
    }

    free(col_sum_array);
    //return STAT_ERR;
    return STAT_OK;
}

_G_STATUS bmp_digit_recognize(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    _bmp_vital_pt vital = bmp->vital_info;
    uint32_t *col_array = vital->reserved4;
    uint32_t *col_array_remain = col_array;
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    
    //the remaining area of col_array is used to storage the result of recognization
    while(COL_ARRAY_END != *col_array_remain++); //point to the remaining area
    
    uint32_t width = 0, width_start = 0, width_end = 0, width_last = 0;
    uint32_t height = 0, height_start = 0, height_end = 0;
    uint32_t flag = 0, tmp = 0;
    uint32_t height_tp = 0;
    uint8_t situation = 0;
    uint32_t offset = 0;
    _MAT_TYPE *template_ptr = NULL, *tmp_template_ptr = NULL;
    _MAT_TYPE *digit_ptr = NULL, *tmp_digit_ptr = NULL;
    //uint32_t real_width = vital->real_width;
    uint32_t real_width = vital->width;
    uint32_t j = 0, k = 0;
    uint32_t factor[10], min = 0;
    uint8_t i = 0, min_index = 0;

    while(COL_ARRAY_END != *col_array)
    {
        height_start = vital->reserved2;
        height_end = vital->reserved3;
        height = height_end - height_start;
        width_start = col_array[0];
        width_end = col_array[1];
        width = width_end - width_start;
        if((height/width) >= DIGIT_ONE_RATIO)
        {
            *col_array_remain++ = 1; //the digit is "1"
            *col_array_remain = COL_ARRAY_REAL_END;
            col_array += 2;
            continue;
        }

        //it's unnecessary to create template if width_last is equal with width
        if(width_last != width)
        {
            if(flag) //do not free template in the first time
                template_free();
            template_create(width);
            flag = 1;
        }

        //make sure that the heigth of template is the same as digit's
        height_tp = width * 2 ;        
        if(height_tp > height)
        {
            situation = 1;
        }
        else if(height_tp < height)
        {
            situation = 2;
        }
        else
        {
            situation = 3;
        }

        /*
        digit_ptr = data_ptr + (height_end-1)*real_width + width_start;
        for(j = height_start; j < height_end; j++)
        {
            for(k = 0; k < width; k++)
            {
                DISP("%d ", digit_ptr[k]);
            }
            digit_ptr -= real_width;
            DISP("\n");
        }
        DISP("\n");
        */

        switch(situation)
        {
            case 1 :
                for(i = 1; i < 10; i++)
                {
                    template_ptr = g_number_mat[i]->pMat;
                    digit_ptr = data_ptr + (height_end-1)*real_width + width_start;
                    tmp = 0;
                    for(j = 0; j < height; j++)
                    {
                        offset = (uint32_t)(((float)j*height_tp)/height + 0.5f);
                        tmp_template_ptr = template_ptr + offset*width;
                        for(k = 0; k < width; k++)
                        {
                            //DISP("%d ", tmp_template_ptr[k]);
                            tmp += (digit_ptr[k] ^ tmp_template_ptr[k]);
                        }
                        digit_ptr -= real_width;
                        //DISP("\n");
                    }
                    //DISP("\n");
                    factor[i] = tmp;
                }
                break;
            case 2 :
                for(i = 1; i < 10; i++)
                {
                    digit_ptr = data_ptr + (height_end-1)*real_width + width_start;
                    template_ptr = g_number_mat[i]->pMat;
                    tmp = 0;
                    for(j = 0; j < height_tp; j++)
                    {
                        offset = (uint32_t)(((float)j*height)/height_tp + 0.5f);
                        tmp_digit_ptr = digit_ptr - offset*real_width;
                        for(k = 0; k < width; k++)
                        {
                            //DISP("%d ", template_ptr[k]);
                            tmp += (tmp_digit_ptr[k] ^ template_ptr[k]);
                        }
                        template_ptr += width;
                        //DISP("\n");
                    }
                    //DISP("\n");
                    factor[i] = tmp;
                }
                break;
            case 3 :
                for(i = 1; i < 10; i++)
                {
                    digit_ptr = data_ptr + (height_end-1)*real_width + width_start;
                    template_ptr = g_number_mat[i]->pMat;
                    tmp = 0;
                    for(j = height_start; j < height_end; j++)
                    {
                        for(k = 0; k < width; k++)
                        {
                            //DISP("%d ", template_ptr[k]);
                            tmp += (digit_ptr[k] ^ template_ptr[k]);
                        }
                        digit_ptr -= real_width;
                        template_ptr += width;
                        //DISP("\n");
                    }
                    //DISP("\n");
                    factor[i] = tmp;
                }
                break;
            default :
                break;
        }

        min = factor[1];
        min_index = 1;
        for(i = 1; i < 10; i++)
        {
            DISP("%d ", factor[i]);
            if(min > factor[i])
            {
                min = factor[i];
                min_index = i;
            }
        }
        DISP("\n");

        if(1 != min_index)
        {
            *col_array_remain++ = min_index; //the digit is 2-9
            *col_array_remain = COL_ARRAY_REAL_END;
        }
        else
        {
            *col_array_remain++ = 0; //the digit is 0
            *col_array_remain = COL_ARRAY_REAL_END;
        }
        
        width_last = width;
        col_array += 2;
    }

    template_free();
    return STAT_OK;
}

#if 0
_G_STATUS bmp_digit_recognize(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    _bmp_vital_pt vital = bmp->vital_info;
    uint32_t *col_array = vital->reserved4;
    uint32_t *col_array_remain = col_array;
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    
    //the remaining area of col_array is used to storage the result of recognization
    while(COL_ARRAY_END != *col_array_remain++); //point to the remaining area
    
    uint32_t width = 0, width_start = 0, width_end = 0, width_last = 0;
    uint32_t height = 0, height_start = 0, height_end = 0;
    uint32_t flag = 0, tmp = 0;
    int32_t height_diff = 0;
    uint32_t height_start_tp = 0;
    _MAT_TYPE *template_ptr = NULL;
    _MAT_TYPE *digit_ptr = NULL;
    //uint32_t real_width = vital->real_width;
    uint32_t real_width = vital->width;
    uint32_t j = 0, k = 0;
    uint32_t factor[10], min = 0;
    uint8_t i = 0, min_index = 0;

    while(COL_ARRAY_END != *col_array)
    {
        height_start = vital->reserved2;
        height_end = vital->reserved3;
        height = height_end - height_start;
        width_start = col_array[0];
        width_end = col_array[1];
        width = width_end - width_start;
        if((height/width) >= DIGIT_ONE_RATIO)
        {
            *col_array_remain++ = 1; //the digit is "1"
            *col_array_remain = COL_ARRAY_REAL_END;
            col_array += 2;
            continue;
        }

        //it's unnecessary to create template if width_last is equal with width
        if(width_last != width)
        {
            if(flag) //do not free template in the first time
                template_free();
            template_create(width);
            flag = 1;
        }

        //make sure that the heigth of template is the same as digit's
        height_diff = width*2 - height;
        if(height_diff < 0)
        {
            height_diff *= -1;
            tmp = (uint32_t)height_diff / 2;
            height_start += (uint32_t)height_diff - tmp;
            height_end -= tmp;
        }
        else if(height_diff > 0)
        {
            tmp = (uint32_t)height_diff / 2;
            height_start_tp = tmp;
        }
        else
        {
            height_start_tp = 0;
        }

        digit_ptr = data_ptr + (height_end-1)*real_width + width_start;
        for(j = height_start; j < height_end; j++)
        {
            for(k = 0; k < width; k++)
            {
                DISP("%d ", digit_ptr[k]);
            }
            digit_ptr -= real_width;
            DISP("\n");
        }
        DISP("\n");
        
        for(i = 1; i < 10; i++)
        {
            digit_ptr = data_ptr + (height_end-1)*real_width + width_start;
            template_ptr = g_number_mat[i]->pMat + height_start_tp*width;
            tmp = 0;
            for(j = height_start; j < height_end; j++)
            {
                for(k = 0; k < width; k++)
                {
                    DISP("%d ", template_ptr[k]);
                    tmp += (digit_ptr[k] ^ template_ptr[k]);
                }
                digit_ptr -= real_width;
                template_ptr += width;
                DISP("\n");
            }
            DISP("\n");
            factor[i] = tmp;
        }

        min = factor[1];
        min_index = 1;
        for(i = 1; i < 10; i++)
        {
            DISP("%d ", factor[i]);
            if(min > factor[i])
            {
                min = factor[i];
                min_index = i;
            }
        }
        DISP("\n");

        if(1 != min_index)
        {
            *col_array_remain++ = min_index; //the digit is 2-9
            *col_array_remain = COL_ARRAY_REAL_END;
        }
        else
        {
            *col_array_remain++ = 0; //the digit is 0
            *col_array_remain = COL_ARRAY_REAL_END;
        }
        
        width_last = width;
        col_array += 2;
    }

    template_free();
    return STAT_OK;
}
#endif

#if 0
_G_STATUS bmp_digit_recognize(_bmp_pt bmp)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
    
    if(!bmp->vital_info || !bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_BMP);
        return STAT_ERR;
    }
#endif //__DEBUG

    uint8_t *new_digit = (uint8_t *)malloc(32*16);
    if(!new_digit)
    {
        DISP_ERR(ERR_MALLOC);
        return STAT_ERR;
    }
    if(template_create(16))
    {
        free(new_digit);
        return STAT_ERR;
    }
    
    _bmp_vital_pt vital = bmp->vital_info;
    uint32_t *col_array = vital->reserved4;
    uint32_t *col_array_remain = col_array;
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    
    //the remaining area of col_array is used to storage the result of recognization
    while(COL_ARRAY_END != *col_array_remain++); //point to the remaining area
    
    uint32_t width = 0, width_start = 0, width_end = 0;
    uint32_t height = 0, height_start = 0, height_end = 0;
    uint32_t i = 0, j = 0, row_off = 0, col_off = 0;
    uint8_t *new_digit_ptr = NULL, *tmp_digit_ptr = NULL;
    _MAT_TYPE *tmp_data_ptr = NULL, *template_ptr = NULL;
    uint32_t real_width = vital->width;
    _VOL int32_t factor[10];
    int32_t min = 0, tmp = 0;
    uint8_t min_index = 0;

    while(COL_ARRAY_END != *col_array)
    {
        height_start = vital->reserved2;
        height_end = vital->reserved3;
        height = height_end - height_start;
        width_start = col_array[0];
        width_end = col_array[1];
        width = width_end - width_start;
        if((height/width) >= DIGIT_ONE_RATIO)
        {
            *col_array_remain++ = 1; //the digit is "1"
            *col_array_remain = COL_ARRAY_REAL_END;
            col_array += 2;
            continue;
        }

        new_digit_ptr = new_digit;
        tmp_data_ptr = data_ptr + (height_end-1)*real_width + width_start;
        for(i = 0; i < 32; i++)
        {
            row_off = (uint32_t)(((float)i*height)/32 + 0.5);
            tmp_digit_ptr = tmp_data_ptr - row_off * real_width;
            for(j = 0; j < 16; j++)
            {
                col_off = (uint32_t)(((float)j*width)/16 + 0.5);
                //*new_digit_ptr++ = (tmp_digit_ptr[col_off] ? 0 : 1);
                *new_digit_ptr++ = tmp_digit_ptr[col_off];
            }
        }

        for(i = 1; i < 10; i++)
        {
            new_digit_ptr = new_digit;
            template_ptr = g_number_mat[i]->pMat;
            //factor[i] = 0;
            tmp = 0;
            for(j = 0; j < 32*16; j++)
            {
                tmp += (new_digit_ptr[j] ^ template_ptr[j]);
            }
            factor[i] = tmp;
            
        }

        min = factor[1];
        min_index = 1;
        for(i = 1; i < 10; i++)
        {
            if(min > factor[i])
            {
                min = factor[i];
                min_index = i;
            }
        }

        if(1 != min_index)
        {
            *col_array_remain++ = min_index; //the digit is 2-9
            *col_array_remain = COL_ARRAY_REAL_END;
        }
        else
        {
            *col_array_remain++ = 0; //the digit is 0
            *col_array_remain = COL_ARRAY_REAL_END;
        }
        
        col_array += 2;
    }
    
    free(new_digit);
    template_free();
    
    return STAT_OK;
}
#endif

/*************************************************************************
                          extended functions
 ************************************************************************/
 
uint8_t *bmp_rgb565_to_rgb888(uint32_t img_width, uint32_t img_height, 
    uint16_t *data_addr)
{
    uint32_t real_width = COUNT_REAL_WIDTH(img_width*24);
    uint32_t real_size = real_width * img_height;
    uint8_t *new_data_ptr = (uint8_t *)malloc(real_size + HEAD_INFO_SIZE);
    if(!new_data_ptr)
    {
        DISP_ERR(ERR_MALLOC);
        return NULL;
    }

    //create head info for rgb888
    uint32_t size = real_size + HEAD_INFO_SIZE;
    uint32_t tmp = 0;
    new_data_ptr[0] = 'B';
    new_data_ptr[1] = 'M';
    
    tmp = size;
    new_data_ptr[2] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[3] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[4] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[5] = tmp & 0xFF;

    new_data_ptr[6] = 0;
    new_data_ptr[7] = 0;
    new_data_ptr[8] = 0;
    new_data_ptr[9] = 0;

    new_data_ptr[10] = HEAD_INFO_SIZE;
    new_data_ptr[11] = 0;
    new_data_ptr[12] = 0;
    new_data_ptr[13] = 0;

    new_data_ptr[14] = 40;
    new_data_ptr[15] = 0;
    new_data_ptr[16] = 0;
    new_data_ptr[17] = 0;
    
    tmp = img_width;
    new_data_ptr[18] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[19] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[20] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[21] = tmp & 0xFF;

    tmp = img_height;
    new_data_ptr[22] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[23] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[24] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[25] = tmp & 0xFF;

    new_data_ptr[26] = 1;
    new_data_ptr[27] = 0;
    
    new_data_ptr[28] = 24;
    new_data_ptr[29] = 0;

    new_data_ptr[30] = 0;
    new_data_ptr[31] = 0;
    new_data_ptr[32] = 0;
    new_data_ptr[33] = 0;

    tmp = real_size;
    new_data_ptr[34] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[35] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[36] = tmp & 0xFF;
    tmp >>= 8;
    new_data_ptr[37] = tmp & 0xFF;

    new_data_ptr[38] = 0xC4;
    new_data_ptr[39] = 0x0E;
    new_data_ptr[40] = 0;
    new_data_ptr[41] = 0;

    new_data_ptr[42] = 0xC4;
    new_data_ptr[43] = 0x0E;
    new_data_ptr[44] = 0;
    new_data_ptr[45] = 0;

    new_data_ptr[46] = 0;
    new_data_ptr[47] = 0;
    new_data_ptr[48] = 0;
    new_data_ptr[49] = 0;

    new_data_ptr[50] = 0;
    new_data_ptr[51] = 0;
    new_data_ptr[52] = 0;
    new_data_ptr[53] = 0;

    //convert rgb565 to rgb888
    uint8_t *tmp_ptr = NULL;
    uint32_t fill_pixel_num = real_width - ((img_width * 24) >> 3);
    uint16_t i = 0, j = 0;
    uint16_t *data_ptr = NULL;

    tmp_ptr = new_data_ptr + HEAD_INFO_SIZE;
    if(0 == fill_pixel_num)
    {
        for(i = 0; i < img_height; i++)
        {
            data_ptr = data_addr + i*img_width;
            for(j = 0; j < img_width; j++)
            {
                *tmp_ptr++ = (data_ptr[j] & 0x001F) << 3;
                *tmp_ptr++ = ((data_ptr[j] & 0x07E0) >> 5) << 2;
                *tmp_ptr++ = ((data_ptr[j] & 0xF800) >> 11) << 3;
            }
        }
    }
    else
    {
        for(i = 0; i < img_height; i++)
        {
            data_ptr = data_addr + i*img_width;
            for(j = 0; j < img_width; j++)
            {
                *tmp_ptr++ = (data_ptr[j] & 0x001F) << 3;
                *tmp_ptr++ = ((data_ptr[j] & 0x07E0) >> 5) << 2;
                *tmp_ptr++ = ((data_ptr[j] & 0xF800) >> 11) << 3;
            }

            for(j = 0; j < fill_pixel_num; j++)
            {
                *tmp_ptr++ = 0;
            }
        }
    }

    return new_data_ptr;
}

