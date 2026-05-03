#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"

typedef struct {
    Scheduler base;
    int timeQuantum;
} RoundRobinScheduler;

RoundRobinScheduler* RoundRobinScheduler_Create(int quantum);
void RoundRobinScheduler_Destroy(Scheduler* self);

#endif
