/*************************************************************************
	> File Name: image.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分43秒
 ************************************************************************/

#include "image.h"
#include <unistd.h>

_image_t g_image;
FILE *g_image_fp = NULL;

_STA _IMG_STATUS image_bmp_parse(uint8_t *ptr);

_IMG_STATUS image_read(char *img_file)
{
    if(access(img_file, F_OK))
    {
        DISP_ERR_PLUS("[%s][%d]: %s does not exist \n", __func__, __LINE__, img_file);
        return IMG_ERR;
    }

    g_image_fp = fopen(img_file, "r");
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return IMG_ERR;
    }
    if(feof(g_image_fp) || ferror(g_image_fp))
    {
        DISP_ERR("error in feof or ferror");
        fclose(g_image_fp);
        g_image_fp = NULL;
        return IMG_ERR;
    }

    uint32_t ret = 0;
    uint8_t head_info[HEAD_INFO_SIZE] = {0};
    ret = fread(head_info, sizeof(uint8_t), HEAD_INFO_SIZE, g_image_fp);
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fread");
        fclose(g_image_fp);
        g_image_fp = NULL;
        return IMG_ERR;
    }

    _IMG_STATUS stat;
    stat = image_bmp_parse(head_info);
    if(IMG_ERR == stat)
        return IMG_ERR;

    DISP("%d \n", g_image.main_info.size);
    DISP("%d \n", g_image.main_info.off_bits);
    DISP("%d \n", g_image.plus_info.width);
    DISP("%d \n", g_image.plus_info.height);
    DISP("%d \n", g_image.plus_info.size_real);
    DISP("%d \n", g_image.plus_info.x_pixel);
    DISP("%d \n", g_image.plus_info.y_pixel);

    fclose(g_image_fp);
    return IMG_OK;
}

_STA _IMG_STATUS image_bmp_parse(uint8_t *ptr)
{
    if( ('B' != ptr[0]) || ('M' != ptr[1]) )
    {
        DISP_ERR("not a bmp image");
        return IMG_ERR;
    }

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
    g_image.main_info.size = tmp_32;
    ptr += 4;

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_image.main_info.reserved1 = tmp_16;
    ptr += 2;
    
    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_image.main_info.reserved2 = tmp_16;
    ptr += 2;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.main_info.off_bits = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.size_struct = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.width = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.height = tmp_32;
    ptr += 4;

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_image.plus_info.planes = tmp_16;
    ptr += 2;    

    tmp_16 = 0;
    tmp_16 |= ptr[1];
    tmp_16 <<= 8;
    tmp_16 |= ptr[0];
    g_image.plus_info.bit_count = tmp_16;
    ptr += 2;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.comperssion = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.size_real = tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.x_pixel= tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.y_pixel= tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.clr_used= tmp_32;
    ptr += 4;

    tmp_32 = 0;
    tmp_32 |= ptr[3];
    tmp_32 <<= 8;
    tmp_32 |= ptr[2];
    tmp_32 <<= 8;
    tmp_32 |= ptr[1];
    tmp_32 <<= 8;
    tmp_32 |= ptr[0];
    g_image.plus_info.clr_import= tmp_32;

    return IMG_OK;
}
