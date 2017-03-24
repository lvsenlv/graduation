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

    #else //__REDIRECTION
        #pragma message("Unactivated __REDIRECTION")
    #endif //__REDIRECTION

    void __attribute__((constructor)) before_main(void)
    {
        START_COUNT;
    #ifdef __REDIRECTION
        g_disp_file = fopen("./log.txt", "w+");
        if(g_disp_file)
            fclose(g_disp_file);
    
        g_disp_file = fopen("./log.txt", "a+");
        if(!g_disp_file)
            g_disp_file = stderr;
    #endif //__REDIRECTION
    }

    void __attribute__((destructor)) after_main(void)
    {
    #ifdef __REDIRECTION
        if(g_disp_file)
            fclose(g_disp_file);
    #endif //__REDIRECTION
        STOP_COUNT;
        START_STOP;
    }


#else //__LINUX
    #pragma message("Unactivated __LINUX")
    #pragma message("Unactivated __REDIRECTION")
#endif //__LINUX

