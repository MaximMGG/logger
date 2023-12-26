#ifndef _LOG_I_
#define _LOG_I_
#include <stdbool.h>

typedef enum log_level_e {
    L_TRACE, 
    L_DEBUG,
    L_INFO,
    L_WARN,
    L_ERROR,
    L_FATAL
}log_level;

typedef enum log_output_e {
    LOG_CONSOLE = 0x0001,
    LOG_FILE = 0x0002,
    LOG_HTTP = 0x0004
} log_output;

//struct for threds queue
struct log_msg {
    char *msg;
    log_level l;
};

typedef struct logger_s {
    char prioritet_log_level;
    int output; 
    int max_file_length;
    char file_name[128];
    bool time_on;
    char path[128];
     
    
    char *http_response; //do not functional
    char current_file[128];
} logger;


#endif //_LOG_I_
