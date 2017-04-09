/*************************************************************************
	> File Name: image.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月15日 星期三 12时25分43秒
 ************************************************************************/

#include "image.h"
#include <stdlib.h>

#ifdef __LINUX
#include <unistd.h>
#endif

#ifdef __OS
FILE *g_image_fp = NULL;

_bmp_pt image_read(int8_t *img_file)
{
#ifdef __LINUX
    if(access(img_file, F_OK))
    {
        DISP_ERR_PLUS("[%s][%d]: %s does not exist \n", __func__, __LINE__, img_file);
        return NULL;
    }
#endif //__LINUX

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

    uint32_t ret = fread(head_info, sizeof(uint8_t), HEAD_INFO_SIZE, g_image_fp);
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fread");
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(head_info);
        head_info = NULL;
        return NULL;
    }

    _bmp_pt bmp = bmp_create();
    if(!bmp)
    {
        DISP_ERR("error in bmp_create");
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(head_info);
        head_info = NULL;
        return NULL;
    }

    if(bmp_head_parse(bmp, head_info))
    {
        fclose(g_image_fp);
        g_image_fp = NULL;
        free(head_info);
        head_info = NULL; //this case, bmp->head_info and bmp->data_info != NULL
        bmp_free(bmp);
        bmp = NULL;
    }

    uint32_t real_size = bmp->vital_info->real_size;
    uint8_t *data_info = (uint8_t *)malloc(real_size);
    if(!data_info)
    {
        DISP_ERR(ERR_MALLOC);
        fclose(g_image_fp);
        g_image_fp = NULL;
        bmp_free(bmp);
        bmp = NULL;
        return NULL;
    }

    ret = fread(data_info, sizeof(uint8_t), real_size, g_image_fp);
    if(real_size != ret)
    {
        DISP_ERR("error in fread");
        fclose(g_image_fp);
        g_image_fp = NULL;
        bmp_free(bmp);
        bmp = NULL;
        return NULL;
    }

    if(bmp_data_parse(bmp, data_info))
    {
        DISP_ERR("error in bmp_data_parse");
        fclose(g_image_fp);
        g_image_fp = NULL;
        bmp_free(bmp);
        bmp = NULL;
        return NULL;
    }

    fclose(g_image_fp);
    g_image_fp = NULL;
    
    return bmp;
}

#else //__OS
#ifdef __FATFS

FIL g_fp;
FIL *g_image_fp = NULL;
#endif
#endif //__OS

_G_STATUS image_write(_bmp_pt bmp, int8_t *img_file)
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

    #ifdef __FATFS
    g_image_fp = fopen_rewrite(img_file, "w+");
    #else
    g_image_fp = fopen(img_file, "w+");
    #endif //__FATFS
    
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return STAT_ERR;
    }

    uint32_t ret = 0;
    #ifdef __FATFS
    ret = fwrite_rewrite(bmp->head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    #else
    ret = fwrite(bmp->head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    #endif //__FATFS
    
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fwrite");
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    #ifdef __FATFS
    ret = fwrite_rewrite(bmp->data_info->pMat, sizeof(uint8_t), 
                bmp->vital_info->real_size, g_image_fp);
    #else
    ret = fwrite(bmp->data_info->pMat, sizeof(uint8_t), 
                bmp->vital_info->real_size, g_image_fp);
    #endif //__FATFS
    
    if(bmp->vital_info->real_size != ret)
    {
        DISP_ERR("error in fwrite");
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }
    
    #ifdef __FATFS
    fclose_rewrite(g_image_fp);
    #else
    fclose(g_image_fp);
    #endif //__FATFS
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

_G_STATUS image_write_gray(_bmp_pt bmp, int8_t *img_file)
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

    #ifdef __FATFS
    g_image_fp = fopen_rewrite(img_file, "w+");
    #else
    g_image_fp = fopen(img_file, "w+");
    #endif //__FATFS
    
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return STAT_ERR;
    }

    uint32_t ret = 0;
    #ifdef __FATFS
    ret = fwrite_rewrite(bmp->head_info->pMat, sizeof(uint8_t), 
        HEAD_INFO_SIZE, g_image_fp);
    #else
    ret = fwrite(bmp->head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    #endif //__FATFS
    
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fwrite");
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    _bmp_vital_pt vital = bmp->vital_info; 
    uint32_t real_size = vital->real_size;
    uint8_t *buf = (uint8_t *)malloc(real_size);
    if(!buf)
    {
        DISP_ERR(ERR_MALLOC);
        return STAT_ERR;
    }
    
    uint32_t width = vital->width;
    uint32_t height = vital->height;
    uint32_t fill_pixel_num = 
        vital->real_width - ((width * vital->bit_count) >> 3);
    uint32_t i = 0, j = 0;
    uint8_t *data_info = bmp->data_info->pMat;
    uint8_t *tmp_ptr = buf;

    if(0 == fill_pixel_num) 
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data of G and R are the same with B
                *tmp_ptr++  = *data_info;
                *tmp_ptr++  = *data_info;
                *tmp_ptr++  = *data_info++;
            }    
        }
        
    }
    else
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data of G and R are the same with B
                *tmp_ptr++  = *data_info;
                *tmp_ptr++  = *data_info;
                *tmp_ptr++  = *data_info++;
            } 
            
            //fill pixel due to align of 4 byte
            for(j = 0; j < fill_pixel_num; j++)
            {
                *tmp_ptr++ = 0;
            }
        }
    }

    #ifdef __FATFS
    ret = fwrite_rewrite(buf, sizeof(uint8_t), real_size, g_image_fp);
    #else
    ret = fwrite(buf, sizeof(uint8_t), real_size, g_image_fp);
    #endif //__FATFS
    
    if(real_size != ret)
    {
        DISP_ERR("error in fwrite");
        free(buf);
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    free(buf);
    #ifdef __FATFS
    fclose_rewrite(g_image_fp);
    #else
    fclose(g_image_fp);
    #endif //__FATFS
    g_image_fp = NULL;

    return STAT_OK;
}

_G_STATUS image_write_binary(_bmp_pt bmp, int8_t *img_file)
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

    #ifdef __FATFS
    g_image_fp = fopen_rewrite(img_file, "w+");
    #else
    g_image_fp = fopen(img_file, "w+");
    #endif //__FATFS
    
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return STAT_ERR;
    }

    uint32_t ret = 0;
    #ifdef __FATFS
    ret = fwrite_rewrite(bmp->head_info->pMat, sizeof(uint8_t), 
        HEAD_INFO_SIZE, g_image_fp);
    #else
    ret = fwrite(bmp->head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    #endif //__FATFS
    
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fwrite");
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    _bmp_vital_pt vital = bmp->vital_info; 
    uint32_t real_size = vital->real_size;
    uint8_t *buf = (uint8_t *)malloc(real_size);
    if(!buf)
    {
        DISP_ERR(ERR_MALLOC);
        return STAT_ERR;
    }
    
    uint32_t width = vital->width;
    uint32_t height = vital->height;
    uint32_t fill_pixel_num = 
        vital->real_width - ((width * vital->bit_count) >> 3);
    uint32_t i = 0, j = 0;
    uint8_t *data_info = bmp->data_info->pMat;
    uint8_t *tmp_ptr = buf, tmp = 0;

    if(0 == fill_pixel_num) 
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data of G and R are the same with B
                //convert the color data "1" to "0xFF"
                tmp = (*data_info++ == 1) ? 0xFF : 0;
                *tmp_ptr++  = tmp;
                *tmp_ptr++  = tmp;
                *tmp_ptr++  = tmp;
            }    
        }
    }
    else
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data of G and R are the same with B
                //convert the color data "1" to "0xFF"
                tmp = (*data_info++ == 1) ? 0xFF : 0;
                *tmp_ptr++  = tmp;
                *tmp_ptr++  = tmp;
                *tmp_ptr++  = tmp;
            } 
            
            //fill pixel due to align of 4 byte
            for(j = 0; j < fill_pixel_num; j++)
            {
                *tmp_ptr++ = 0;
            }
        }
    }

    #ifdef __FATFS
    ret = fwrite_rewrite(buf, sizeof(uint8_t), real_size, g_image_fp);
    #else
    ret = fwrite(buf, sizeof(uint8_t), real_size, g_image_fp);
    #endif //__FATFS
    
    if(real_size != ret)
    {
        DISP_ERR("error in fwrite");
        free(buf);
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    free(buf);
    #ifdef __FATFS
    fclose_rewrite(g_image_fp);
    #else
    fclose(g_image_fp);
    #endif //__FATFS
    g_image_fp = NULL;

    return STAT_OK;
}

_G_STATUS image_write_plus_row(_bmp_pt bmp, int8_t *img_file)
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

    #ifdef __FATFS
    g_image_fp = fopen_rewrite(img_file, "w+");
    #else
    g_image_fp = fopen(img_file, "w+");
    #endif //__FATFS
    
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return STAT_ERR;
    }

    _bmp_vital_pt vital = bmp->vital_info;
    uint32_t width = vital->width;
    uint32_t start_height = vital->reserved2;
    uint32_t end_height = vital->reserved3;
    uint32_t height = end_height - start_height;
    uint32_t real_size = vital->real_width * height;

    //rewrite head_data
    _matrix_pt head_info = bmp->head_info;
    _MAT_TYPE *head_ptr = head_info->pMat;
    uint32_t tmp = height;
    head_ptr[22] = tmp & 0xFF;
    tmp >>= 8;
    head_ptr[23] = tmp & 0xFF;
    tmp >>= 8;
    head_ptr[24] = tmp & 0xFF;
    tmp >>= 8;
    head_ptr[25] = tmp & 0xFF;

    tmp = real_size;
    head_ptr[34] = tmp & 0xFF;
    tmp >>= 8;
    head_ptr[35] = tmp & 0xFF;
    tmp >>= 8;
    head_ptr[36] = tmp & 0xFF;
    tmp >>= 8;
    head_ptr[37] = tmp & 0xFF;
    
    uint32_t ret = 0;
    #ifdef __FATFS
    ret = fwrite_rewrite(head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    #else
    ret = fwrite(head_info->pMat, sizeof(uint8_t), HEAD_INFO_SIZE, 
                g_image_fp);
    #endif //__FATFS
    
    if(HEAD_INFO_SIZE != ret)
    {
        DISP_ERR("error in fwrite");
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    uint8_t *buf = (uint8_t *)malloc(real_size);
    if(!buf)
    {
        DISP_ERR(ERR_MALLOC);
        return STAT_ERR;
    }
    
    uint32_t fill_pixel_num = 
        vital->real_width - ((width * vital->bit_count) >> 3);
    uint32_t i = 0, j = 0;
    uint8_t *data_info = bmp->data_info->pMat + start_height*width;
    uint8_t *tmp_ptr = buf, tmp_data;

    if(0 == fill_pixel_num) 
    {
        for(i = start_height; i < end_height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data of G and R are the same with B
                //convert the color data "1" to "0xFF"
                tmp_data = (*data_info++ == 1) ? 0xFF : 0;
                *tmp_ptr++  = tmp_data;
                *tmp_ptr++  = tmp_data;
                *tmp_ptr++  = tmp_data;
            }
        }
    }
    else
    {
        for(i = start_height; i < end_height; i++)
        {
            for(j = 0; j < width; j++)
            {
                //data of G and R are the same with B
                //convert the color data "1" to "0xFF"
                tmp_data = (*data_info++ == 1) ? 0xFF : 0;
                *tmp_ptr++  = tmp_data;
                *tmp_ptr++  = tmp_data;
                *tmp_ptr++  = tmp_data;
            } 
            
            //fill pixel due to align of 4 byte
            for(j = 0; j < fill_pixel_num; j++)
            {
                *tmp_ptr++ = 0;
            }
        }
    }

    #ifdef __FATFS
    ret = fwrite_rewrite(buf, sizeof(uint8_t), real_size, g_image_fp);
    #else
    ret = fwrite(buf, sizeof(uint8_t), real_size, g_image_fp);
    #endif //__FATFS
    
    if(real_size != ret)
    {
        DISP_ERR("error in fwrite");
        free(buf);
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }
    
    #ifdef __FATFS
    fclose_rewrite(g_image_fp);
    #else
    fclose(g_image_fp);
    #endif //__FATFS
    g_image_fp = NULL;
    free(buf);

    return STAT_OK;
}

_G_STATUS image_write_plus_col(_bmp_pt bmp, int8_t *img_path)
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

    g_image_fp = NULL;
    _MAT_TYPE *head_ptr = bmp->head_info->pMat;
    _bmp_vital_pt vital = bmp->vital_info;
    uint32_t *col_array = vital->reserved4;
    _MAT_TYPE *data_ptr = bmp->data_info->pMat;
    
    uint32_t height_start = vital->reserved2, height_end = vital->reserved3;
    uint32_t height = height_end - height_start;
    uint32_t width = 0, width_start = 0, width_end = 0;
    uint32_t real_width = 0, real_size = 0, initial_width = vital->width;
    uint32_t bit_count = vital->bit_count, fill_pixel_num = 0;
    uint32_t tmp = 0 ,ret = 0;
    int8_t file_name[IMAGE_FILE_NAME_LEN] = {0};
    uint8_t files = 1;          //determine the image name
    uint8_t *tmp_data_ptr = NULL, *buf = NULL, *tmp_ptr = NULL;
    uint32_t i = 0, j = 0;
    uint8_t tmp_data = 0;
    
    while(COL_ARRAY_END != *col_array)
    {
        //create current image name
        snprintf(file_name, IMAGE_FILE_NAME_LEN, "%s/num_%d_th.bmp", img_path, files++);
        #ifdef __FATFS
        g_image_fp = fopen_rewrite(file_name, "w+");
        #else
        g_image_fp = fopen(file_name, "w+");
        #endif //__FATFS
        
        if(!g_image_fp)
        {
            DISP_ERR_PLUS("error in fopen: %s\n", file_name);
            return STAT_ERR;
        }
        
        width_start = col_array[0];
        width_end = col_array[1];
        width = width_end - width_start;
        
        //rewrite head_data
        tmp = width;
        head_ptr[18] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[19] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[20] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[21] = tmp & 0xFF;

        tmp = height;
        head_ptr[22] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[23] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[24] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[25] = tmp & 0xFF;

        tmp = real_size;
        head_ptr[34] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[35] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[36] = tmp & 0xFF;
        tmp >>= 8;
        head_ptr[37] = tmp & 0xFF;

        #ifdef __FATFS
        ret = fwrite_rewrite(head_ptr, sizeof(uint8_t), HEAD_INFO_SIZE, g_image_fp);
        #else
        ret = fwrite(head_ptr, sizeof(uint8_t), HEAD_INFO_SIZE, g_image_fp);
        #endif //__FATFS
        
        if(HEAD_INFO_SIZE != ret)
        {
            DISP_ERR_PLUS("error in fwrite: %s\n", file_name);
            #ifdef __FATFS
            fclose_rewrite(g_image_fp);
            #else
            fclose(g_image_fp);
            #endif //__FATFS
            g_image_fp = NULL;
            return STAT_ERR;
        }

        real_width = COUNT_REAL_WIDTH(width*bit_count);
        real_size = real_width * height;
        buf = (uint8_t *)malloc(real_size);
        if(!buf)
        {
            DISP_ERR(ERR_MALLOC);
            return STAT_ERR;
        }

        fill_pixel_num = real_width - ((width*bit_count) >> 3);
        tmp_ptr = buf;
        if(0 == fill_pixel_num)
        {
            for(i = height_start; i < height_end; i++)
            {
                tmp_data_ptr = data_ptr + i*initial_width + width_start;
                for(j = width_start; j < width_end; j++)
                {
                    //data of G and R are the same with B
                    //convert the color data "1" to "0xFF"
                    tmp_data = (*tmp_data_ptr++ == 1) ? 0xFF : 0;
                    *tmp_ptr++  = tmp_data;
                    *tmp_ptr++  = tmp_data;
                    *tmp_ptr++  = tmp_data;
                }
            }
        }
        else
        {
            for(i = height_start; i < height_end; i++)
            {
                tmp_data_ptr = data_ptr + i*initial_width + width_start;
                for(j = width_start; j < width_end; j++)
                {
                    //data of G and R are the same with B
                    //convert the color data "1" to "0xFF"
                    tmp_data = (*tmp_data_ptr++ == 1) ? 0xFF : 0;
                    *tmp_ptr++  = tmp_data;
                    *tmp_ptr++  = tmp_data;
                    *tmp_ptr++  = tmp_data;
                }
            
                //fill pixel due to align of 4 byte
                for(j = 0; j < fill_pixel_num; j++)
                {
                    *tmp_ptr++ = 0;
                }
            }
        }

        #ifdef __FATFS
        ret = fwrite_rewrite(buf, sizeof(uint8_t), real_size, g_image_fp);
        #else
        ret = fwrite(buf, sizeof(uint8_t), real_size, g_image_fp);
        #endif //__FATFS
        
        if(real_size != ret)
        {
            DISP_ERR_PLUS("error in fwrite: %s\n", file_name);
            free(buf);
            #ifdef __FATFS
            fclose_rewrite(g_image_fp);
            #else
            fclose(g_image_fp);
            #endif //__FATFS
            g_image_fp = NULL;
            return STAT_ERR;
        }

        free(buf);
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        
        col_array += 2;
    }

    return STAT_OK;
}

/*************************************************************************
                          extended functions
 ************************************************************************/

#ifdef __OS
#define HEAD_INFO_SIZE_16BIT        (HEAD_INFO_SIZE+12)
uint16_t *image_read_16bit_bmp(uint32_t img_width, uint32_t img_height, 
    int8_t *img_file)
{
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

    uint32_t real_width = COUNT_REAL_WIDTH(img_width*16);
    uint32_t real_size = real_width*img_height;
    uint8_t *data_ptr = (uint8_t *)malloc(real_size+HEAD_INFO_SIZE_16BIT);
    if(!data_ptr)
    {
        DISP_ERR(ERR_MALLOC);
        fclose(g_image_fp);
        g_image_fp = NULL;
        return NULL;
    }

    uint32_t ret = 0;
    ret = fread(data_ptr, sizeof(uint8_t), real_size+HEAD_INFO_SIZE_16BIT, g_image_fp);
    if(ret != real_size+HEAD_INFO_SIZE_16BIT)
    {
        DISP_ERR("error in fread");
        free(data_ptr);
        fclose(g_image_fp);
        g_image_fp = NULL;
        return NULL;
    }

    uint16_t *new_data_ptr = (uint16_t *)malloc(img_width*img_height*sizeof(uint16_t));
    if(!new_data_ptr)
    {
        DISP_ERR(ERR_MALLOC);
        free(data_ptr);
        fclose(g_image_fp);
        g_image_fp = NULL;
        return NULL;
    }

    uint32_t i = 0, j = 0;
    uint8_t *tmp_data_ptr = data_ptr+HEAD_INFO_SIZE_16BIT;
    uint16_t *tmp_new_data_ptr = new_data_ptr;
    uint16_t ntmp = 0, tmp = 0;
    for(i = 0; i < img_height; i++)
    {
        for(j = 0; j < img_width; j+=2)
        {
            tmp = 0;
            tmp |= tmp_data_ptr[j];
            tmp <<= 8;
            tmp |= tmp_data_ptr[j+1];

            ntmp = 0;
            ntmp |= ((tmp & 0xF800) >> 11);
            ntmp |= (tmp & 0x7E0);
            ntmp |= ((tmp & 0x1F) << 11);
            
            *tmp_new_data_ptr++ = ntmp;
        }
        tmp_data_ptr += real_width;
    }

    free(data_ptr);
    fclose(g_image_fp);
    g_image_fp = NULL;

    return new_data_ptr;
}
#endif //__OS

_G_STATUS image_write_rbg888(uint32_t img_width, uint32_t img_height, 
    uint8_t *data_addr, int8_t *img_file)
{
    #ifdef __FATFS
    g_image_fp = fopen_rewrite(img_file, "w+");
    #else
    g_image_fp = fopen(img_file, "w+");
    #endif //__FATFS
    
    if(!g_image_fp)
    {
        DISP_ERR("error in fopen");
        return STAT_ERR;
    }
#ifdef __OS
    if(feof(g_image_fp) || ferror(g_image_fp))
    {
        DISP_ERR("error in feof or ferror");
        fclose(g_image_fp);
        g_image_fp = NULL;
        return STAT_ERR;
    }
#endif //__OS

    uint32_t real_width = COUNT_REAL_WIDTH(img_width*24);
    uint32_t total_size = real_width*img_height + HEAD_INFO_SIZE;
    uint32_t ret = 0;

    #ifdef __FATFS
    ret = fwrite_rewrite(data_addr, sizeof(uint8_t), total_size, g_image_fp);
    #else
    ret = fwrite(data_addr, sizeof(uint8_t), total_size, g_image_fp);
    #endif //__FATFS

    if(ret != total_size)
    {
        DISP_ERR("error in fwrite");
        #ifdef __FATFS
        fclose_rewrite(g_image_fp);
        #else
        fclose(g_image_fp);
        #endif //__FATFS
        g_image_fp = NULL;
        return STAT_ERR;
    }

    #ifdef __FATFS
    fclose_rewrite(g_image_fp);
    #else
    fclose(g_image_fp);
    #endif //__FATFS
    g_image_fp = NULL;

    return STAT_OK;
}



/*************************************************************************
                          extended functions
 ************************************************************************/

#ifndef __OS

#ifdef __FATFS

#include "bsp_sdio_sd.h"

FATFS g_fs;

FIL *fopen_rewrite(const int8_t *path, int8_t *mode)
{
    if(mode[0] == 'w' && mode[1] == '+')
    {
        FRESULT res_sd;
        res_sd = f_open(&g_fp, path, FA_CREATE_ALWAYS | FA_WRITE );
        if(res_sd != FR_OK)
            return NULL;
    }
    else
        return NULL;

    return ((FIL *)&g_fp);
}

uint32_t fwrite_rewrite(const void* buf, uint32_t unused, 
    uint32_t write_size, FIL *fp)
{
    uint32_t count_write = 0;
    FRESULT res_sd;
    res_sd = f_write(fp, buf, write_size, &count_write); 
    if((res_sd != FR_OK))
        return 0;

    return count_write;
}

_G_STATUS sd_mount(void)
{
    FRESULT sd_ret;
    sd_ret = f_mount(&g_fs, "0:", 1);
    if(sd_ret == FR_NO_FILESYSTEM)
    {
        DISP("no file system in card, ready to format \n");
        sd_ret = f_mkfs("0:", 0, 0);
        if(sd_ret == FR_OK)
        {
            DISP("finish formatting \n");
            sd_ret = f_mount(NULL, "0:", 1); //remount after format
            sd_ret = f_mount(&g_fs, "0:", 1);
        }
        else
        {
            DISP_ERR("failed to format");
            f_mount(NULL, "0:", 1);
            return STAT_ERR;
        }
    }
    else if(sd_ret != FR_OK)
    {
        DISP_ERR_PLUS("failed to mount, error code: %d \n", sd_ret);
        DISP_ERR("ensure card inserted to the board");
        DISP_ERR("ensure 3V3_WIFI connetted to 3V3 instead of GND");
        f_mount(NULL, "0:", 1);
        return STAT_ERR;
    }

    return STAT_OK;
}

#endif //__FATFS

#endif //#ifndef __OS

