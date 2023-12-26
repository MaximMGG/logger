#include <threads.h>
#include <util/util.h>
#include "../headers/logger.h"

thrd_t worker;
Queue *q;
logger *w_log;


int logging(void *ptr) {

    printf("Start logging");

    return 0;
}

char *cvt_level_to_char(log_level l) {

    return NULL;
}

char *get_current_time() {

    return NULL;
}

void worker_create_file() {

}

void worker_init(logger *l) {
    q = queue_create();




    thrd_create(&worker, &logging, NULL);
    w_log = l;
}

void worker_put(void *m) {
    queue_add(q, m);
}


