/*************************************************************************
	> File Name: common.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月06日 星期一 13时42分52秒
 ************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>

#define     _VOL                            volatile
#define     _CON                            const
#define     _STA                            static

#define     DISP_ERR(str) \
            fprintf(stderr, "[%s][%d]: %s \n", __func__, __LINE__, str)
#define     DISP_ERR_PLUS(format, args...) \
            fprintf(stderr, format, ##args)

typedef enum {
    STAT_OK = 0,
    STAT_ERR,
}_G_STATUS;

#ifdef __LINUX

#include <sys/time.h>
extern struct timeval g_start_time, g_stop_time;
#define     START_COUNT                     gettimeofday(&g_start_time, NULL)
#define     STOP_COUNT                      gettimeofday(&g_stop_time, NULL)
#define     START_STOP \
            fprintf(stdout, "projec cost about %ldus in total\n", \
            (g_stop_time.tv_sec - g_start_time.tv_sec) * 1000000 + \
             g_stop_time.tv_usec - g_start_time.tv_usec)

#ifdef __REDIRECTION
extern FILE *g_disp_file;
#define     DISP(format, args...) \
            fprintf(g_disp_file, format, ##args)
#else //REDIRECTION
#define     DISP(format, args...) \
            fprintf(stdout, format, ##args)
#endif //__REDIRECTION

#else //__LINUX
#define     DISP(format, args...) \
            fprintf(stdout, format, ##args)

#endif //__LINUX

#endif
