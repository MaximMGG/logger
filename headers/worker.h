#ifndef _WORKER_H_
#define _WORKER_H_
#include "logger.h"

void worker_init(logger *l);
void worker_put(struct log_msg *msg);

#endif // _WORKER_H_
