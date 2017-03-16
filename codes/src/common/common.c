/*************************************************************************
	> File Name: common.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月09日 星期四 14时47分06秒
 ************************************************************************/

#include "common.h"

#ifdef __LINUX
#pragma message("Activate __LINUX")
struct timeval g_start_time, g_stop_time;

#ifdef __REDIRECTION
#pragma message("Activate __REDIRECTION")
FILE *g_disp_file = NULL;

void __attribute__((constructor)) before_main(void)
{
    START_COUNT;
    g_disp_file = fopen("./log.txt", "w+");
    if(g_disp_file)
        fclose(g_disp_file);
    
    g_disp_file = fopen("./log.txt", "a+");
    if(!g_disp_file)
        g_disp_file = stderr;
}

void __attribute__((destructor)) after_main(void)
{
    if(g_disp_file)
        fclose(g_disp_file);
    STOP_COUNT;
    START_STOP;
}
#else //__REDIRECTION
#pragma message("Unactivated __REDIRECTION")
#endif //__REDIRECTION

#else //__LINUX
#pragma message("Unactivated __LINUX")
#pragma message("Unactivated __REDIRECTION")
#endif //__LINUX

