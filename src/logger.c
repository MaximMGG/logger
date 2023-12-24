#include "../headers/logger.h"
#include <string.h>

static logger LOG;


void logger_init(char prioritet_log_level, 
                int output, 
                int max_file_length,
                char *file_name,
                char *path,
                bool time_on) 
{
    LOG.prioritet_log_level = prioritet_log_level;
    LOG.output = output;
    if (output & FILE) {
        LOG.max_file_length = max_file_length;
        strncpy(LOG.file_name, file_name, 127);
    }
    LOG.time_on = time_on;
    if (path != NULL) {
        strcpy(LOG.path, path);
    }
};

void configure();

void log(log_level l, char *msg);
