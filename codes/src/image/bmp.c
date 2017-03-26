/*************************************************************************
	> File Name: bmp.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 19时14分11秒
 ************************************************************************/

#include "bmp.h"
#include <stdlib.h>

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
    _bmp_vital_pt nptr = bmp->vital_info;
    
    uint32_t width = 0;
    width |= ptr[21];
    width <<= 8;
    width |= ptr[20];
    width <<= 8;
    width |= ptr[19];
    width <<= 8;
    width |= ptr[18];
    nptr->width = width;

    uint32_t height = 0;
    height |= ptr[25];
    height <<= 8;
    height |= ptr[24];
    height <<= 8;
    height |= ptr[23];
    height <<= 8;
    height |= ptr[22];
    nptr->height = height;
    
    uint16_t bit_count = 0;
    bit_count <<= 8;
    bit_count |= ptr[29];
    bit_count <<= 8;
    bit_count |= ptr[28];
    nptr->bit_count = bit_count;

    uint32_t real_size = 0;
    real_size |= ptr[37];
    real_size <<= 8;
    real_size |= ptr[36];
    real_size <<= 8;
    real_size |= ptr[35];
    real_size <<= 8;
    real_size |= ptr[34];
    nptr->real_size = real_size;

    nptr->real_width = COUNT_REAL_WIDTH(width*bit_count);
    
    return STAT_OK;
}

void bmp_free(_bmp_pt bmp)
{
    if(bmp)
    {
        matrix_free(bmp->head_info);
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
#if 0
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
#endif

#if 0
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
    DISP("%d \n", total_times);
    return STAT_OK;
}
#endif

//efficient algorithm when model size is 3
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
    
    DISP("%d \n", total_times);
    return STAT_OK;
}

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

    float avgValue = 0;
    for(i = 0; i < 256; i++)  
    {  
        histogram[i] = histogram[i] / size;
        avgValue += i*histogram[i];
    }

    float var = 0;
    float var_max = 0;
    float wk = 0, uk = 0, ut = 0; 
    
    for(i = 0; i < 256; i++)
    {
        wk += histogram[i];  
        uk += i * histogram[i];       
  
        ut = avgValue*wk - uk;  
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
        *data_ptr = ((*data_ptr < threshold) ? 0x0 : 0xFF);
        data_ptr++;
    }
    //matrix_disp(bmp->data_info);

    return STAT_OK;
}

