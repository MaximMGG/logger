#ifndef _LOG_I_
#define _LOG_I_
#include <stdbool.h>

typedef enum log_level_e {
    TRACE, 
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
}log_level;

typedef enum log_output_e {
    CONSOLE = 0x0001,
    FILE = 0x0002,
    HTTP = 0x0004
} log_output;

typedef struct logger_s {
    char prioritet_log_level;
    int output; 
    int max_file_length;
    char file_name[128];
    bool time_on;
    char path[128];
     
    
    char *http_response; //do not functional
} logger;


#endif //_LOG_I_
