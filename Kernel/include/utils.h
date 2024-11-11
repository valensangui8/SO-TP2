#ifndef _UTILS_H
#define _UTILS_H

#include "memory.h"
#include <interrupts.h>
#include <lib.h>
#include <memoryManagerADT.h>
#include <naiveConsole.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 20
#define MM_ADDRESS 0x50000
#define SCHEDULER_ADDRESS 0x60000
#define SEMAPHORE_MANAGER_ADDRESS 0x70000
#define PIPE_MANAGER_ADDRESS 0x80000

#define BUILT_IN_FD 3
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define DEV_NULL -1

#define NO_CHILDREN 0

typedef enum { BLOCKED = 0,
			   READY,
			   RUNNING,
			   ZOMBIE,
			   DEAD
} PCBState;

typedef enum { PRIORITY1 = 1,
			   PRIORITY2,
			   PRIORITY3,
			   PRIORITY4
} Priority;

typedef int (*main_function)(uint64_t argc, char **argv);

typedef struct PCB {
	char name[32];
	uint16_t pid;
	uint16_t ppid;
	uint16_t waiting_pid;
	Priority priority;
	int times_to_run; // contador de veces que se ejecuto
	PCBState state;
	void *stack_pointer;
	void *stack_base;
	int16_t fds[BUILT_IN_FD];
	char **argv;
	int argc;
	int ret;
} PCBT;

typedef struct Scheduler {
	PCBT processes[MAX_PROCESS];
	uint16_t current_pid;
	uint16_t index_p; // va a estar en un lugar libre, si esta lleno valdra -1
	uint16_t index_rr;
	int8_t quantum_remaining;
	uint32_t amount_processes;
	uint64_t next_pid;
	uint8_t kill_foreground;
	uint64_t foreground_pid;
} Scheduler;

typedef struct Scheduler *SchedulerInfo;

#endif
