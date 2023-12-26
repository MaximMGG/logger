#include "../headers/logger.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <threads.h>
#include <util/util.h>

#define try(a) if ((a) == NULL || (a) < 0) fprintf(stderr, "%s error\n", # a)

static logger LOG;
static mtx_t *mutex;
static thrd_t worker;

boolean work = false;

static Queue *q;

void create_worker();

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
    char *current_time = getcurrenttime();
    char fmt[] = "%s->%s.log";

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
    q = queue_create();
    create_worker();
    work = true;
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

int do_log_thrd(void *ptr) {

    while (work) {
        while(get_size(q) > 0) {
            struct log_msg *msg = (struct log_msg *)queue_get(q);
            if (msg->l >= LOG.prioritet_log_level) {
                checkfile();
                char *level = cvtlogtochar(msg->l);
                char *time = getcurrenttime();
                char tmp[] = "%s: %s %s\n";
                char buf[128];
                memset(buf, 0, 128);
                snprintf(buf, 128, tmp, level, msg->msg, time);

                FILE *f = fopen(LOG.file_name, "a");
                if (f == NULL) {
                    fprintf(stderr, "Can't open file %s\n", LOG.file_name);
                }
                fputs(buf, f);
                fclose(f);

                free(level);
                free(time);
                free(msg);
            }
        }
    }

    return 0;
}


void create_worker() {
    thrd_create(&worker, &do_log_thrd, NULL);
}


void do_log(log_level l, char *msg) {
    struct log_msg *log_msg = malloc(sizeof(*msg));
    log_msg->msg = msg;
    log_msg->l = l;
    queue_add(q, (void *) log_msg);
}



void logger_close() {
    mtx_destroy(mutex);
    work = false;
    tryi(thrd_detach(worker));
}
