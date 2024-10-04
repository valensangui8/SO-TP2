#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdint.h>
#include <process.h>
#include <stdlib.h>
#include "utils.h"

#define MAX_PROCESS 20
#define QUANTUM 20 //20ms

typedef struct SchedulerCDT * SchedulerADT;
SchedulerADT create_scheduler();

#endif
