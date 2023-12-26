#ifndef _LOGGER_
#define _LOGGER_
#include "loginstrument.h"
#include <util/util.h>

//if configure not NULL, check file witch name log.conf, output can by 0
void logger_init(char prioritet_log_level, 
                int output, 
                int max_file_length,
                char *file_name,
                char *path,
                bool time);

void do_log(log_level l, char *msg);


void logger_close();




#endif //_LOGGER_
