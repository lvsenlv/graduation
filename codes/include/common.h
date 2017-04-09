/*************************************************************************
	> File Name: common.h
	> Author: lvsenlv
	> Mail: lvsen46000@163.com
	> Created Time: 2017年03月06日 星期一 13时42分52秒
 ************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#ifdef __OS

#ifdef __LINUX
typedef     char                            int8_t;
typedef     short                           int16_t;
typedef     int                             int32_t;
typedef     long long                       int64_t;
typedef     unsigned char                   uint8_t;
typedef     unsigned short                  uint16_t;
typedef     unsigned int                    uint32_t;
typedef     unsigned long long              uint64_t;
#else //__LINUX
#include <stdint.h>
#endif //__LINUX

#else //__OS
#include <stdint.h>
#endif //__OS

//#undef      NULL
//#define     NULL                            ((void *)0)

#define     _VOL                            volatile
#define     _CON                            const
#define     _STA                            static

#define     ERR_MALLOC                      "error in malloc"
#define     ERR_MATRIX                      "error in matrix"
#define     ERR_PMAT                        "error in pMat"
#define     ERR_CREATE                      "error in create"
#define     ERR_FATAL                       "fatal error"
#define     ERR_BMP                         "invalid bmp image"

typedef enum {
    STAT_OK = 0,
    STAT_ERR,
}_G_STATUS;

#ifdef __OS

#define     DISP_ERR(str) \
            fprintf(stderr, "[%s][%d]: %s \n", __func__, __LINE__, str)
#define     DISP_ERR_PLUS(format, args...) \
            fprintf(stderr, format, ##args)

#ifdef __LINUX
#include <sys/time.h>
extern struct timeval g_start_time, g_stop_time;
#define     START_COUNT                     gettimeofday(&g_start_time, NULL)
#define     STOP_COUNT                      gettimeofday(&g_stop_time, NULL)
#define     START_STOP \
            fprintf(stdout, "projec cost about %ldus in total\n", \
            (g_stop_time.tv_sec - g_start_time.tv_sec) * 1000000 + \
             g_stop_time.tv_usec - g_start_time.tv_usec)
#endif //__LINUX

#ifdef __REDIRECTION
extern FILE *g_disp_file;
#define     DISP(format, args...) \
            fprintf(g_disp_file, format, ##args)
#else //REDIRECTION
#define     DISP(format, args...) \
            fprintf(stdout, format, ##args)
#endif //__REDIRECTION

#else //__OS
#define     DISP(format, args...)           ((void)0)
#define     DISP_ERR(str)                   ((void)0)
#define     DISP_ERR_PLUS(format, args...)  ((void)0)

#endif //__OS

#endif
