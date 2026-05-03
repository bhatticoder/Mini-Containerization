#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Scheduler {
    void (*schedule)(struct Scheduler* self, Process* processes, int count);
    void (*destroy)(struct Scheduler* self);
} Scheduler;

void Scheduler_CalculateMetrics(Process* processes, int count);

#endif
