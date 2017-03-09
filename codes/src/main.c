/*************************************************************************
	> File Name: main.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年02月27日 星期一 16时10分06秒
 ************************************************************************/

#include <stdio.h>
#include "matrix.h"
#include "matlab.h"
#include "template.h"

#define     REAL_WID                        32

int main(int argc, char **argv)
{
    template_create(REAL_WID);
    template_disp();
    template_free();
    return 0;
}

