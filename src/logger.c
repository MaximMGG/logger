#include "../headers/logger.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

static logger LOG;

char *getcurrenttime() {
    time_t t;
    t = time(&t);
    struct tm *tm = localtime(&t);

    return asctime(tm);
}

void createfile() {
    char buf[128];
    char fmt[] = "%s %s.log";
    snprintf(buf, 128L, fmt, LOG.file_name, getcurrenttime());
    strcpy(LOG.file_name, buf);

    FILE *f = fopen(buf, "w");

    if (f == NULL) {
        fprintf(stderr, "Cannot create new log file");
    }
    
    fclose(f);
}

void logger_init(char prioritet_log_level, 
                int output, 
                int max_file_length,
                char *file_name,
                char *path,
                bool time_on) 
{
    LOG.prioritet_log_level = prioritet_log_level;
    LOG.output = output;
    if (output & LOG_FILE) {
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
    char *buf = malloc(sizeof(char) * 10);

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
    struct stat st;
    stat(LOG.file_name, &st);
    size_t file_size = st.st_size;

    if ((file_size / 1024) > LOG.max_file_length) {
        createfile();
        return 1;
    }
    return 0;
}

void log(log_level l, char *msg) {
    if (l >= LOG.prioritet_log_level) {
        checkfile();
        char *level = cvtlogtochar(l);
        char *time = getcurrenttime();
        char tmp[] = "%s: %s %s";
        char buf[128];
        memset(buf, 0, 128);
        snprintf(buf, 128, tmp, level, msg, time);

        FILE *f = fopen(LOG.file_name, "w");
        if (f == NULL) {
            fprintf(stderr, "Can't open file %s\n", LOG.file_name);
        }
        fputs(buf, f);
        fclose(f);
    }
}
