// file name : tool.c
// 工具（如时间）
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <string.h>  
#include <time.h>  

void Timeshow(time_t T, char* current) {
    struct tm t = { 0 };
    if ((localtime_s(&t, &T)) == 0) {
        strftime(current, 20, "%Y-%m-%d %H:%M", &t);
    }
    else {
        printf("时间转换错误！");
    }
}
//2026-06-10 20:39
time_t cintime(char* current) {
    struct tm t = { 0 };
    sscanf_s(current, "%d-%d-%d %d:%d",
        &t.tm_year, &t.tm_mon, &t.tm_mday,
        &t.tm_hour, &t.tm_min);
    t.tm_year -= 1900;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    return mktime(&t);
}

void timetostr(time_t a, char* s)   //将时间转化为年月日
{
    struct tm* timeinfo;
    timeinfo = localtime(&a);
    strftime(s, 20, "%Y-%m-%d", timeinfo);
}