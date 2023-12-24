#ifndef _LOGGER_
#define _LOGGER_
#include "loginstrument.h"

enum log_level {
    TRACE, 
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

//if configure not NULL, check file witch name log.conf, output can by 0
void logger_init(char *configure, int output);

//TODO here will be more interesting!
void configure();

void log(log_level l, char *msg);





#endif //_LOGGER_
