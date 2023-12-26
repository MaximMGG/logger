#include "../headers/logger.h"
#include "worker.c"
#include <string.h>


logger LOG;



void logger_init(char prioritet_log_level, 
                int output, 
                int max_file_length,
                char *file_name,
                char *path,
                bool time_on) {
    LOG.prioritet_log_level = prioritet_log_level;
    LOG.output = output;
    LOG.max_file_length = max_file_length;
    strcpy(LOG.path, path);
    strcpy(LOG.file_name, file_name);
    LOG.time_on = time_on;
    worker_init(&LOG);
}

void do_log(log_level l, char *msg);


void logger_close();
