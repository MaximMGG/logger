#ifndef _LOG_I_
#define _LOG_I_
#include <stdbool.h>

typedef struct logger_s {
    char prioritet;
    int output; 
    int max_file_length;
    char file_name[128];
    bool time;
     
    
    char *http_response;
} logger;

#endif //_LOG_I_
