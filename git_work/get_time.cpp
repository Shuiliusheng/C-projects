#include "stdafx.h"
void get_now_time(char str_time[])//获取系统时间，以字符串表示
{
    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    sprintf(str_time,"%d/%d/%d-", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
	char temp[20];
    sprintf(temp,"%s-%d:%d:%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
	strcat(str_time,temp);
}