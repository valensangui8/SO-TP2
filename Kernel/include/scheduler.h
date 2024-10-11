#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <process.h>
#include <stdint.h>
#include <stdlib.h>
#include <utils.h>

#define MAX_PROCESS 20
#define QUANTUM 20 // 20ms
#define RR_INDEX(i) ((i+1)%(MAX_PROCESS * PRIORITY4))

typedef struct Scheduler {
	PCBT processes[MAX_PROCESS];
	PCBT *round_robin[MAX_PROCESS * PRIORITY4];
	uint16_t current_pid;
	uint16_t index_p; // va a estar en un lugar libre, si esta lleno valdra -1
	uint16_t index_rr;
	int8_t quantum_remaining;
} Scheduler;

typedef struct Scheduler *SchedulerInfo;

SchedulerInfo create_scheduler();
int8_t set_state(uint8_t new_state);
SchedulerInfo get_scheduler();
int8_t get_state();
void create_process(char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, void *stack_pointer, char foreground, char *argv[], int argc);
unsigned int get_pid();
void scheduler();
void update_quantum();


#endif
