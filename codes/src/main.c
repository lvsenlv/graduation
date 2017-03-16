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
    bmp = image_read("/root/test.bmp");
    if(!bmp)
    {
        DISP_ERR("error in image_read");
        return -1;
    }

    image_write(bmp, "./object/tmp.bmp");
    bmp_free(bmp);

    template_create(16);
    //template_disp();
    template_free();
    
    return 0;
}

