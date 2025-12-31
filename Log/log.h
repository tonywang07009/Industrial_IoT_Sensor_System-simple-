#ifndef _LOG_
#define _LOG_


#include<stdio.h>
#include<stdarg.h> // RD
#include<time.h> // RD
#include<stdarg.h>

typedef enum Log_event_type
{
    LOG_EVENT_SERVER_START,
    LOG_EVENT_CLIENT_RUN,
    LOG_EVENT_STAT_RESULT,
    LOG_EVENT_ERROR

}LogEventType;

/* Function*/

static void make_time_string(char* buffer , size_t buffer_size);
void syslog_info(LogEventType type,int port,int is_bad,const char *format, ...);  // ... varable parameter

#endif