/*************************************************************************
	> File Name: bmp.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 19时14分11秒
 ************************************************************************/

#include "bmp.h"
#include <stdlib.h>

//_bmp_t g_bmp;

_bmp_pt bmp_create(void)
{
    _bmp_pt bmp = (_bmp_pt)malloc(sizeof(_bmp_t));
    if(!bmp)
    {
        DISP_ERR(ERR_MALLOC);
        return NULL;
    }
    
    bmp->data_info = NULL;
    bmp->head_info = (_matrix_pt)malloc(sizeof(_matrix_t));
    if(!bmp->head_info)
    {
        DISP_ERR(ERR_MALLOC);
        free(bmp);
        bmp = NULL;
        return NULL;
    }
    bmp->head_info->pMat = NULL;
    
    bmp->data_info = (_matrix_pt)malloc(sizeof(_matrix_t));
    if(!bmp->data_info)
    {
        DISP_ERR(ERR_MALLOC);
        free(bmp->head_info);
        bmp->head_info = NULL;
        free(bmp);
        bmp = NULL;
        return NULL;
    }
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
    
    if(!bmp || !bmp->head_info || !bmp->data_info)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR("use bmp_create firstly");
        return STAT_ERR;
    }
#endif //__DEBUG

    if( ('B' != ptr[0]) || ('M' != ptr[1]) )
    {
        
        DISP_ERR("not a bmp image");
        return STAT_ERR;
    }
    
    bmp->head_info->pMat = ptr;
    bmp->head_info->row = 1;
    bmp->head_info->col = HEAD_INFO_SIZE;
    return STAT_OK;
    
#if 0
    ptr += 2; //jump to the 3rd byte
    uint32_t tmp_32;
    uint16_t tmp_16;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.main_info.size = tmp_32;
    ptr += 4;

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_bmp.main_info.reserved1 = tmp_16;
    ptr += 2;
    
    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_bmp.main_info.reserved2 = tmp_16;
    ptr += 2;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.main_info.off_bits = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.struct_size = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.width = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.height = tmp_32;
    ptr += 4;

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_bmp.plus_info.planes = tmp_16;
    ptr += 2;    

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_bmp.plus_info.bit_count = tmp_16;
    ptr += 2;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.comperssion = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.real_size = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.x_pixel= tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.y_pixel= tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.clr_used= tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.clr_import= tmp_32; 
#endif
}

#if 0
void bmp_info_disp(void)
{
    DISP("size=%d \n", g_bmp.main_info.size);
    DISP("off_bits=%d \n", g_bmp.main_info.off_bits);
    DISP("struct_size=%d\n", g_bmp.plus_info.struct_size);
    DISP("width=%d \n", g_bmp.plus_info.width);
    DISP("height=%d \n", g_bmp.plus_info.height);
    DISP("planes=%d\n", g_bmp.plus_info.planes);
    DISP("bit_count=%d\n", g_bmp.plus_info.bit_count);
    DISP("comperssion=%d\n", g_bmp.plus_info.comperssion);
    DISP("real_size=%d \n", g_bmp.plus_info.real_size);
    DISP("x_pixel=%d \n", g_bmp.plus_info.x_pixel);
    DISP("y_pixel=%d \n", g_bmp.plus_info.y_pixel);
    DISP("clr_used=%d \n", g_bmp.plus_info.clr_used);
    DISP("clr_import=%d \n", g_bmp.plus_info.clr_import);
}
#endif

void bmp_free(_bmp_pt bmp)
{
    if(bmp)
    {
        matrix_free(bmp->head_info);
        matrix_free(bmp->data_info);
        free(bmp);
    }
}

_G_STATUS bmp_get_vital_info(_bmp_pt bmp, _bmp_vital_pt vital)
{
#ifdef __DEBUG
    if(!bmp || !bmp->head_info || !bmp->head_info->pMat)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR("invalid bmp image");
        return STAT_ERR;
    }
#endif //__DEBUG
    uint32_t width = 0;
    width |= bmp->head_info->pMat[21];
    width <<= 8;
    width |= bmp->head_info->pMat[20];
    width <<= 8;
    width |= bmp->head_info->pMat[19];
    width <<= 8;
    width |= bmp->head_info->pMat[18];
    vital->width = width;

    uint32_t height = 0;
    height |= bmp->head_info->pMat[25];
    height <<= 8;
    height |= bmp->head_info->pMat[24];
    height <<= 8;
    height |= bmp->head_info->pMat[23];
    height <<= 8;
    height |= bmp->head_info->pMat[22];
    vital->height = height;
    
    uint16_t bit_count = 0;
    bit_count <<= 8;
    bit_count |= bmp->head_info->pMat[29];
    bit_count <<= 8;
    bit_count |= bmp->head_info->pMat[28];
    vital->bit_count = bit_count;

    uint32_t real_size = 0;
    real_size |= bmp->head_info->pMat[37];
    real_size <<= 8;
    real_size |= bmp->head_info->pMat[36];
    real_size <<= 8;
    real_size |= bmp->head_info->pMat[35];
    real_size <<= 8;
    real_size |= bmp->head_info->pMat[34];
    vital->real_size = real_size;

    vital->real_width = COUNT_REAL_WIDTH(width*bit_count);

    return STAT_OK;
}

void bmp_viatl_info_disp(_bmp_vital_pt vital)
{
    DISP("width=%d \n", vital->width);
    DISP("height=%d \n", vital->height);
    DISP("bit_count=%d\n", vital->bit_count);
    DISP("real_size=%d \n", vital->real_size);
    DISP("real_width=%d \n", vital->real_width);
}

_G_STATUS bmp_data_parse(_bmp_pt bmp, _bmp_vital_pt vital, 
            uint8_t *ptr)
{
#ifdef __DEBUG
    if(!ptr)
    {
        DISP_ERR("invalid ptr");
        return STAT_ERR;
    }
    
    if(!bmp || !bmp->data_info)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR("invalid bmp image");
        return STAT_ERR;
    }

    if(!vital->height || !vital->real_width)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR("use bmp_get_vital_info firstly");
        return STAT_ERR;
    }
#endif //__DEBUG

    bmp->data_info->pMat = ptr;
    bmp->data_info->row = vital->height;
    bmp->data_info->col = vital->real_width;
    
    return STAT_OK;
}

_G_STATUS bmp_check(_bmp_pt bmp)
{
    if(!bmp || !bmp->head_info || !bmp->data_info)
    {
        DISP_ERR(ERR_MATRIX);
        return STAT_ERR;
    }

    if(!bmp->head_info->pMat)
    {
        DISP_ERR(ERR_PMAT);
        return STAT_ERR;
    }

    if(!bmp->data_info->pMat)
    {
        DISP_ERR(ERR_PMAT);
        return STAT_ERR;
    }

    return STAT_OK;
}

_bmp_pt bmp_convert_gray(_bmp_pt bmp)
{
    _bmp_vital_t vital;
#ifdef __DEBUG
    memset(&vital_info, 0, sizeof(_bmp_vital_t));
#endif //__DEBUG
    if(bmp_get_vital_info(bmp, &vital))
        return NULL;

    if(vital.bit_count != 24)
    {
        DISP_ERR("only support 24 bits depth bmp image");
        return NULL;
    }

#ifdef __DEBUG
    if(!bmp->data_info || !bmp->data_info->pMat)
    {
        DISP_ERR(ERR_FATAL);
        DISP_ERR("invalid bmp image");
        return NULL;
    }
#endif //__DEBUG

    _bmp_pt bmp_ret = bmp_create();
    if(!bmp_ret)
    {
        DISP_ERR("error in bmp_create");
        return NULL;
    }

    uint8_t *ret_head_info = (uint8_t *)malloc(HEAD_INFO_SIZE);
    if(!ret_head_info)
    {
        DISP_ERR(ERR_MALLOC);
        bmp_free(bmp_ret);
        bmp_ret = NULL;
        return NULL;
    }
    bmp_ret->head_info->pMat = ret_head_info;
    bmp_ret->head_info->row = 1;
    bmp_ret->head_info->col = HEAD_INFO_SIZE;
    
    uint8_t *info = (uint8_t *)malloc(vital.real_size);
    if(!info)
    {
        DISP_ERR(ERR_MALLOC);
        bmp_free(bmp_ret);
        bmp_ret = NULL;
        return NULL;
    }
    bmp_ret->data_info->pMat = info;
    bmp_ret->data_info->row = vital.height;
    bmp_ret->data_info->col = vital.real_width;


    //copy head info to new bmp image
    uint32_t i = 0;
    uint8_t *tmp_ptr = bmp->head_info->pMat;
    for(i = 0; i < HEAD_INFO_SIZE; i++)
    {
        *ret_head_info++ = *tmp_ptr++;
    }

    //convert to gray image, algorithm: Gray = (R*38 + G*75 + B*15) >> 7
    tmp_ptr = bmp->data_info->pMat;
    uint32_t j = 0;
    uint32_t fill_pixel_num = 
        vital.real_width - ((vital.width * vital.bit_count) >> 3);
    uint8_t gray = 0;

    //if fill_pixel_num != 0 , must fill pixels for 4 byte alignment
    if(0 == fill_pixel_num) 
    {
        for(i = 0; i < vital.height; i++)
        {
            for(j = 0; j < vital.width; j++)
            {
                //data save by B,G,R format
                gray = (tmp_ptr[2]*38 + tmp_ptr[1]*75 + tmp_ptr[0]*15) >> 7;
                *info++ = gray;
                *info++ = gray;
                *info++ = gray;
                tmp_ptr += 3;
            }
        }
    }
    else
    {
        for(i = 0; i < vital.height; i++)
        {
            for(j = 0; j < vital.width; j++)
            {
                //data save by B,G,R format
                gray = (tmp_ptr[2]*38 + tmp_ptr[1]*75 + tmp_ptr[0]*15) >> 7;
                *info++ = gray;
                *info++ = gray;
                *info++ = gray;
                tmp_ptr += 3;
            }

            //fill pixel due to align of 4 byte
            for(j = 0; j < fill_pixel_num; j++)
            {
                *info++ = 0;
                tmp_ptr ++;
            }
        }
    }
    
    return bmp_ret;
}