#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "Scheduler.h"
#include <string.h>

typedef struct {
    Scheduler base;
} FCFSScheduler;

FCFSScheduler* FCFSScheduler_Create(void);
void FCFSScheduler_Destroy(Scheduler* self);

#endif
