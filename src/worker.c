#include <threads.h>
#include <stdio.h>
#include <util/util.h>
#include "../headers/logger.h"

thrd_t worker;
Queue *q;
logger *w_log;
boolean job = true;

char *cvt_level_to_char(log_level l) {

    return NULL;
}

char *get_current_time() {

    return NULL;
}

void worker_create_file() {
    puts("worker_create_file worker");

}

void worker_log_file(char *msg) {

    puts("worker_log_file worker");
}

void worker_log_console(char *msg) {
    puts("worker_log_console worker");
}

char *concat_msg(char *time, char *level, char *msg) {
    char *log_msg = malloc(sizeof(char) * 256);
    char fmt[] = "%s: %s->%s";
    snprintf(log_msg, 256, fmt, level, msg, time);

    return log_msg;
}

int logging(void *ptr) {
    printf("Start logging worker");

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


    return 0;
}

void worker_init(logger *l) {
    puts("Worker init worker");
    q = queue_create();
    worker_create_file();





    thrd_create(&worker, &logging, NULL);
    w_log = l;
}

void worker_put(void *m) {
    puts("worker_put");
    queue_add(q, m);
}


