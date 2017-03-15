/*************************************************************************
	> File Name: bmp.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 19时14分11秒
 ************************************************************************/

#include "bmp.h"

_bmp_t g_bmp;
extern _mat_char *g_mat_str[];


_G_STATUS bmp_head_parse(_CON uint8_t *ptr)
{
    if( ('B' != ptr[0]) || ('M' != ptr[1]) )
    {
        DISP_ERR("not a bmp image");
        return STAT_ERR;
    }

    ptr += 2; //jump to the 3rd byte
    uint32_t tmp_32;
    uint16_t tmp_16;
    
#ifdef __DEBUG
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
    
#else //__DEBUG
    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.main_info.size = tmp_32;
    ptr += 8;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.main_info.off_bits = tmp_32;
    ptr += 8;

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
    ptr += 6;

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_bmp.plus_info.bit_count = tmp_16;
    ptr += 6;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_bmp.plus_info.real_size = tmp_32;
    
#endif //__DEBUG
    return STAT_OK;
}

void bmp_info_disp(void)
{
#ifdef __DEBUG
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
#else //__DEBUG
    DISP("size=%d \n", g_bmp.main_info.size);
    DISP("width=%d \n", g_bmp.plus_info.width);
    DISP("height=%d \n", g_bmp.plus_info.height);
    DISP("bit_count=%d\n", g_bmp.plus_info.bit_count);
    DISP("real_size=%d \n", g_bmp.plus_info.real_size);
#endif //__DEBUG

}

_matrix_pt bmp_data_parse(_CON uint8_t *ptr)
{
    _matrix_pt bmp_data_mat = NULL;
   bmp_data_mat = matrix_create(g_bmp.plus_info.height, g_bmp.plus_info.width);
    if(!bmp_data_mat)
    {
        DISP_ERR(g_mat_str[ERR_MALLOC]);
        return NULL;
    }

    //the image was saved by aligning by way of 4 byte
    uint32_t real_width = \
        COUNT_REAL_WIDTH(g_bmp.plus_info.width*g_bmp.plus_info.bit_count);

    
    
    return bmp_data_mat;
}

