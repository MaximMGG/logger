#include "../headers/logger.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <threads.h>

static logger LOG;
static mtx_t *mutex;
static thrd_t worker;
// static thrd_t *thread;

char *getcurrenttime() {
    time_t t;
    t = time(&t);
    struct tm *tm = localtime(&t);
    char *tmp = malloc(128);
    strftime(tmp, 128, "%G:%m:%d-%H:%M:%S", tm);

    return tmp;
}



void createfile() {
    char buf[128];
    char fmt[] = "%s->%s.log";
    char *current_time = getcurrenttime();
    snprintf(buf, 128, fmt, LOG.file_name, current_time);

    
    DIR *dir = opendir(LOG.path);
    if (dir == NULL) {
        mkdir(LOG.path, ACCESSPERMS);
    }
    closedir(dir);

    char full_path[256];
    memset(full_path, 0, 256);

    strcat(full_path, LOG.path);
    strcat(full_path, buf);

    strcpy(LOG.file_name, full_path);
    FILE *f = fopen(full_path, "w");

    if (f == NULL) {
        fprintf(stderr, "Cannot create new log file");
    }
    
    free(current_time);
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
    createfile();
};

char *cvtlogtochar(log_level l) {
    char *buf = malloc(sizeof(char) * 10);

    switch(l) {
        case L_TRACE:
            strcpy(buf, "TRACE");
            break;
        case L_DEBUG: 
            strcpy(buf, "DEBUG");
            break;
        case L_INFO:
            strcpy(buf, "INFO");
            break;
        case L_WARN:
            strcpy(buf, "WARN");
            break;
        case L_ERROR:
            strcpy(buf, "ERROR");
            break;
        case L_FATAL:
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

log_level log_level_l;
char *msg_l;

int do_log_thrd(void *ptr) {
    if (log_level_l >= LOG.prioritet_log_level) {
        checkfile();
        char *level = cvtlogtochar(log_level_l);
        char *time = getcurrenttime();
        char tmp[] = "%s: %s %s\n";
        char buf[128];
        memset(buf, 0, 128);
        snprintf(buf, 128, tmp, level, msg_l, time);

        FILE *f = fopen(LOG.file_name, "a");
        if (f == NULL) {
            fprintf(stderr, "Can't open file %s\n", LOG.file_name);
        }
        fputs(buf, f);
        fclose(f);
    }
    thrd_exit(0);
    return 0;
}

void do_log(log_level l, char *msg) {
    log_level_l = l;
    msg_l = msg;
    thrd_t thread;
    thrd_create(&thread, &do_log_thrd, NULL);
}
