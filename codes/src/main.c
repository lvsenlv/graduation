/*************************************************************************
	> File Name: main.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时10分06秒
 ************************************************************************/

#include "image.h"
#include "template.h"

int main(int argc, char **argv)
{
    _bmp_pt bmp = NULL;
    bmp = image_read("/root/pictures/9.bmp");
    if(!bmp)
    {
        DISP_ERR("error in image_read");
        return -1;
    }

    _bmp_pt bmp_gray_ret = NULL;
    bmp_gray_ret = bmp_convert_gray(bmp);
    if(!bmp_gray_ret)
    {
        DISP_ERR("error in bmp_convert_gray");
        bmp_free(bmp);
        return -1;
    }

    image_write(bmp_gray_ret, "./object/gray.bmp");
    bmp_free(bmp);
    bmp_free(bmp_gray_ret); 

    return 0;
}

