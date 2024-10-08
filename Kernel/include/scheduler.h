#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <process.h>
#include <stdint.h>
#include <stdlib.h>
#include <utils.h>

#define MAX_PROCESS 20
#define QUANTUM 20 // 20ms

typedef struct Scheduler {
	PCBT *processes[MAX_PROCESS];
	PCBT *round_robin[MAX_PROCESS * PRIORITY4];
	uint16_t current_pid;
	uint16_t index_p;
	uint16_t index_rr;
	int8_t quantum_remaining;
} Scheduler;

typedef struct Scheduler *SchedulerInfo;

SchedulerInfo create_scheduler();
int8_t set_state(uint8_t new_state);
SchedulerInfo get_scheduler();
int8_t get_status();

#endif
