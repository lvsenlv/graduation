/*************************************************************************
	> File Name: common.c
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月09日 星期四 14时47分06秒
 ************************************************************************/

#include "common.h"

#ifdef __OS
    #ifdef __REDIRECTION
        #pragma message("Activate __REDIRECTION")
        FILE *g_disp_file = NULL;
    #else //__REDIRECTION
        #warning "Unactivated __REDIRECTION"
    #endif //__REDIRECTION

    #ifdef __LINUX
        #pragma message("Activate __LINUX")
        struct timeval g_start_time, g_stop_time;
    #else //__LINUX
        #warning "Unactivate __LINUX"
    #endif //__LINUX
#endif //__OS

void __attribute__((constructor)) before_main(void)
{
#ifdef __OS
    #ifdef __LINUX
        START_COUNT;
    #endif //__LINUX

    #ifdef __REDIRECTION
        g_disp_file = fopen("./log.txt", "w+");
        if(g_disp_file)
            fclose(g_disp_file);

        g_disp_file = fopen("./log.txt", "a+");
        if(!g_disp_file)
            g_disp_file = stderr;
    #endif //__REDIRECTION
#endif //__OS
}

void __attribute__((destructor)) after_main(void)
{
#ifdef __OS
    #ifdef __REDIRECTION
        if(g_disp_file)
            fclose(g_disp_file);
    #endif //__REDIRECTION

    #ifdef __LINUX
        STOP_COUNT;
        START_STOP;
    #endif //__LINUX
#endif //__OS
}

