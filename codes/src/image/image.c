/*************************************************************************
	> File Name: image.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分43秒
 ************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include "image.h"
#include "bmp.h"

FILE *g_image_fp = NULL;
extern _bmp_t g_bmp;
extern _mat_char *g_mat_str[];

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

    //read bmp image
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

    _G_STATUS stat;
    stat = bmp_head_parse(head_info);
    if(STAT_ERR == stat)
        return IMG_ERR;
    
    bmp_info_disp();

    uint8_t *data_info = NULL;
    data_info = (uint8_t *)malloc(g_bmp.plus_info.real_size);
    if(!data_info)
    {
        DISP_ERR(g_mat_str[ERR_MALLOC]);
        return IMG_ERR;
    }

    ret = fread(data_info, sizeof(uint8_t), g_bmp.plus_info.real_size, g_image_fp);
    if(g_bmp.plus_info.real_size != ret)
    {
        DISP_ERR("error in fread");
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(data_info);
        data_info = NULL;
        return IMG_ERR;
    }

    

    fclose(g_image_fp);
    g_image_fp = NULL;
    free(data_info);
    data_info = NULL;
    return IMG_OK;
}

