/*************************************************************************
	> File Name: main.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时10分06秒
 ************************************************************************/

#include "image.h"
#include "template.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    _bmp_pt bmp = NULL;
    bmp = image_read(argv[1]);
    if(!bmp)
    {
        DISP_ERR("error in image_read");
        return -1;
    }

    _bmp_pt bmp_ret = NULL;
    bmp_ret = bmp_convert_gray(bmp);
    if(!bmp_ret)
    {
        DISP_ERR("error in bmp_convert_gray");
        bmp_free(bmp);
        return -1;
    }
    if(image_write_gray(bmp_ret, "./object/gray.bmp"))
    {
        DISP_ERR("error in image_write_gray");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    
    if(bmp_median_filter_fast(bmp_ret))
    {
        DISP_ERR("error in bmp_median_filter");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    if(image_write_gray(bmp_ret, "./object/filter.bmp"))
    {
        DISP_ERR("error in image_write_gray");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }

    uint8_t threshold = 0;
    if(bmp_get_threshold(bmp_ret, &threshold))
    {
        DISP_ERR("error in bmp_get_threshold");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    if(bmp_convert_binary(bmp_ret, threshold))
    {
        DISP_ERR("error in bmp_convert_binary");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    if(image_write_binary(bmp_ret, "./object/binary.bmp"))
    {
        DISP_ERR("error in image_write_binary");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }

    if(bmp_dilate(bmp_ret, 3))
    {
        DISP_ERR("error in bmp_dilate");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    if(image_write_binary(bmp_ret, "./object/dilate.bmp"))
    {
        DISP_ERR("error in image_write_binary");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }

    if(bmp_digit_row_locate(bmp_ret))
    {
        DISP_ERR("error in bmp_digit_row_locate");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    if(image_write_plus_row(bmp_ret, "./object/row.bmp"))
    {
        DISP_ERR("error in image_write_plus_row");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    
    if(bmp_digit_col_locate(bmp_ret))
    {
        DISP_ERR("error in bmp_digit_col_locate");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }
    if(image_write_plus_col(bmp_ret, "./object"))
    {
        DISP_ERR("error in image_write_plus_col");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }

    if(bmp_digit_recognize(bmp_ret))
    {
        DISP_ERR("error in bmp_digit_recognize");
        bmp_free(bmp);
        bmp_free(bmp_ret);
        return -1;
    }    

    uint32_t *col_array_remain = bmp_ret->vital_info->reserved4;
    while(COL_ARRAY_END != *col_array_remain++);
    while(COL_ARRAY_REAL_END != *col_array_remain)
    {
        DISP("%d \n", *col_array_remain++);
    }
  
    bmp_free(bmp);
    bmp_free(bmp_ret);

    return 0;
}

