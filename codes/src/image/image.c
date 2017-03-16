/*************************************************************************
	> File Name: image.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分43秒
 ************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include "image.h"
#include <string.h>

FILE *g_image_fp = NULL;

_bmp_pt image_read(int8_t *img_file)
{
    if(access(img_file, F_OK))
    {
        DISP_ERR_PLUS("[%s][%d]: %s does not exist \n", __func__, __LINE__, img_file);
        return NULL;
    }

    g_image_fp = fopen(img_file, "r");
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return NULL;
    }
    if(feof(g_image_fp) || ferror(g_image_fp))
    {
        DISP_ERR("error in feof or ferror");
        fclose(g_image_fp);
        g_image_fp = NULL;
        return NULL;
    }

    //parse bmp format
    
    uint8_t *head_info = (uint8_t *)malloc(HEAD_INFO_SIZE);
    if(!head_info)
    {
        DISP_ERR(ERR_MALLOC);
        fclose(g_image_fp);
        g_image_fp = NULL;
        return NULL;
    }

    uint32_t ret = 0;
    ret = fread(head_info, sizeof(uint8_t), HEAD_INFO_SIZE, g_image_fp);
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fread");
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(head_info);
        head_info = NULL;
        return NULL;
    }

    _bmp_pt bmp = NULL;
    bmp = bmp_create();
    if(!bmp)
    {
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(head_info);
        head_info = NULL;
        return NULL;
    }

    _G_STATUS stat;
    stat = bmp_head_parse(bmp, head_info);
    if(STAT_ERR == stat)
    {
        DISP_ERR("error in bmp_head_parse");
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(head_info);
        head_info = NULL; //under this case, 
        bmp_free(bmp);    //bmp->head_info and bmp->data_info != NULL
    }

    _bmp_vital_t vital_info;
#ifdef __DEBUG
    memset(&vital_info, 0, sizeof(_bmp_vital_t));
#endif //__DEBUG
    
    stat = bmp_get_vital_info(bmp, &vital_info);
    if(STAT_ERR == stat)
    {
        DISP_ERR("error in bmp_get_vital_info");
        fclose(g_image_fp);
        g_image_fp = NULL; //under this case, 
        bmp_free(bmp);     //bmp->head_info->pMat = head_info,
                           //matrix_free would free it.
    }

    uint8_t *info = (uint8_t *)malloc(vital_info.real_size);
    if(!info)
    {
        DISP_ERR(ERR_MALLOC);
        fclose(g_image_fp);
        g_image_fp = NULL;
        bmp_free(bmp);
        return NULL;
    }

    ret = fread(info, sizeof(uint8_t), vital_info.real_size, g_image_fp);
    if(vital_info.real_size != ret)
    {
        DISP_ERR("error in fread");
        fclose(g_image_fp);
        g_image_fp = NULL;
        bmp_free(bmp);
        return NULL;
    }

    stat = bmp_data_parse(bmp, &vital_info, info);
    if(STAT_ERR == stat)
    {
        DISP_ERR("error in bmp_data_parse");
        fclose(g_image_fp);
        g_image_fp = NULL;
        bmp_free(bmp);
        return NULL;
    }

    fclose(g_image_fp);
    g_image_fp = NULL;
    
    return bmp;
}

_G_STATUS image_write(_bmp_pt bmp, int8_t *img_file)
{
    if(STAT_ERR == bmp_check(bmp))
        return STAT_ERR;

    _bmp_vital_t vital_info;
#ifdef __DEBUG
    memset(&vital_info, 0, sizeof(_bmp_vital_t));
#endif //__DEBUG
    if(STAT_ERR == bmp_get_vital_info(bmp, &vital_info))
        return STAT_ERR;

    g_image_fp = fopen(img_file, "w+");
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return STAT_ERR;
    }

    uint32_t ret = 0;
    ret = fwrite(bmp->head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fwrite");
        fclose(g_image_fp);
        g_image_fp = NULL;
        return STAT_ERR;
    }

    ret = fwrite(bmp->data_info->pMat, sizeof(uint8_t), vital_info.real_size, 
                g_image_fp);
    if(vital_info.real_size != ret)
    {
        DISP_ERR("error in fwrite");
        fclose(g_image_fp);
        g_image_fp = NULL;
        return STAT_ERR;
    }
    
    fclose(g_image_fp);
    g_image_fp = NULL;

#if 0
#ifdef __LINUX
    int8_t buf[64] = {0};
    snprintf(buf, sizeof(buf), "gthumb %s", img_file);
    system(buf);
#endif //__LINUX
#endif

    return STAT_OK;
}

