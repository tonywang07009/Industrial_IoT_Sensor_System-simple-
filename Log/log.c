#include "log.h"

static void make_time_string(char* buffer , size_t buffer_size)
{
    time_t now = time(NULL); //Create the time box
    struct tm* t = localtime(&now); //bind the local time
    strftime(buffer,buffer_size, "%Y-%m-%d %H:%M:%S", t); // Relize 
}
static const char *log_event_to_string(LogEventType type) 
// The static need Specify the datatype in behind.
{
    switch (type)  // This is enums
    {
        case LOG_EVENT_SERVER_START:
        {
            return "SERVER_START";
        }
        case LOG_EVENT_CLIENT_RUN:
        {   
            return "CLIENT_RUN";
        }
        case LOG_EVENT_STAT_RESULT:
        {  
            return "STAT_RESULT";
        }
        case LOG_EVENT_ERROR:
        {        
            return "ERROR";
        }
        default:
        {                     
            return "UNKNOWN";
        }
    }
}

/* The code research*/
void syslog_info(LogEventType type,int port,int is_bad,const char *format,...) // ... varable parameter
                                                                               // ... -> can free add parameter.
{
    /*create the time and even string*/
    char time_buffer [32];
    make_time_string(time_buffer,sizeof(time_buffer));
    // The sizeof is pair the size_t
    const char* event_string = log_event_to_string(type);

    FILE* fp = fopen("system.log","a");
    if (fp == NULL)
    {
        printf("Open the system log is fail \n");
        return;
    }
    /*The failue print*/
    fprintf(fp, "%s [%s] port=%d is_bad=%d ",time_buffer, event_string, port, is_bad);

    /*need research data type*/
    va_list args ;
    va_start(args,format);
    vfprintf(fp,format,args);
    va_end(args);

    fprintf(fp,"\n");
    fclose(fp);
    

}

