#include "../headers/logger.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
    memset(LOG.current_file, 0, 128);
};

void configure();

char *cvtlogtochar(log_level l) {
    char *buf = malloc(sizeof(char) * 32);

    switch(l) {
        case TRACE:
            strcpy(buf, "TRACE");
            break;
        case DEBUG: 
            strcpy(buf, "DEBUG");
            break;
        case INFO:
            strcpy(buf, "INFO");
            break;
        case WARN:
            strcpy(buf, "WARN");
            break;
        case ERROR:
            strcpy(buf, "ERROR");
            break;
        case FATAL:
            strcpy(buf, "FATAL");
            break;
    }

    return buf;
}

int checkfile() {

    return 0;
}

void log(log_level l, char *msg) {
    if (l >= LOG.prioritet_log_level) {
        checkfile();
        char tmp[] = "%s: %s %s";
        char buf[128];
        memset(buf, 0, 128);
    }
}
