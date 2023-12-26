#include <threads.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <util/util.h>
#include "../headers/logger.h"

thrd_t worker;
Queue *q;
logger *w_log;
boolean job = true;
mtx_t mt;

char *cvt_level_to_char(log_level l) {
    char *level = malloc(sizeof(char) * 10);
    switch(l) {
        case L_TRACE:
            strcpy(level, "TRACE");
            break;
        case L_INFO:
            strcpy(level, "INFO");
            break;
        case L_WARN:
            strcpy(level, "WARN");
            break;
        case L_DEBUG:
            strcpy(level, "DEBUG");
            break;
        case L_ERROR:
            strcpy(level, "ERROR");
            break;
        case L_FATAL:
            strcpy(level, "FATAL");
            break;
    }
    return level;
}

char *get_current_time() {
    time_t t;
    t = time(&t);
    struct tm *tm;
    tm = localtime(&t);

    char *localtime = malloc(64);

    strftime(localtime, 64, "%G:%m:%d-%H:%M:%S", tm);

    return localtime;
}

void worker_create_file() {
    puts("worker_create_file worker");
    char full_path[128];
    memset(full_path, 0, 128);
    strcat(full_path, w_log->path);
    strcat(full_path, w_log->file_name);
    char *time = get_current_time();
    strcat(full_path, "->");
    strcat(full_path, time);
    strcat(full_path, ".log");

    strcpy(w_log->current_file, full_path);

    DIR *dir = opendir(w_log->path);
    if (dir == NULL) {
        mkdir(w_log->path, ACCESSPERMS);
    }
    closedir(dir);

    FILE *f = fopen(w_log->current_file, "w");
    tryp(f);
    fclose(f);
    free(time);
}

void check_file() {
    struct stat st;
    stat(w_log->current_file, &st);
    long f_size = st.st_size;

    if ((f_size / 1024) > w_log->max_file_length) {
        worker_create_file();
    }
}

void worker_log_file(char *msg) {

    FILE *f = fopen(w_log->current_file, "a");
    tryp(f);

    fputs(msg, f);
    fputs("\n", f);

    fclose(f);
    check_file();
}

void worker_log_console(char *msg) {
    printf("%s\n", msg);
}

char *concat_msg(char *time, char *level, char *msg) {
    char *log_msg = malloc(sizeof(char) * 256);
    char fmt[] = "%s: %s->%s";
    snprintf(log_msg, 256, fmt, level, msg, time);

    return log_msg;
}

int logging(void *ptr) {

    while(job) {
        while(get_size(q) > 0) {
            struct log_msg *lm = (struct log_msg *) queue_get(q);
            char *current_time = get_current_time();
            char *level = cvt_level_to_char(lm->l);
            char *final_msg = concat_msg(current_time, level, lm->msg);

            if (w_log->output & LOG_FILE) {
                worker_log_file(final_msg);
            }
            if (w_log->output & LOG_CONSOLE) {
                worker_log_console(final_msg);
            }
            free(current_time);
            free(level);
            free(final_msg);
            free(lm);
        }
    }
    thrd_exit(0);
    return 0;
}

void worker_init(logger *l) {
    w_log = l;
    q = queue_create();
    worker_create_file();
}

void worker_put(void *m) {
    queue_add(q, m);
    thrd_create(&worker, &logging, NULL);
}


