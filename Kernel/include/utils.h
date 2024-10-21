#ifndef _UTILS_H
#define _UTILS_H

#include <interrupts.h>
#include <lib.h>
#include <memoryManagerADT.h>
#include <naiveConsole.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 50
#define MM_ADDRESS 0x50000
#define SCHEDULER_ADDRESS 0x60000

#define FOREGROUND 1
#define BACKGROUND !FOREGROUND


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
	Priority priority;
	int times_to_run; // contador de veces que se ejecuto
	PCBState state;
	void * stack_pointer;
	void * stack_base;
	char foreground;
	char **argv;
	int argc;
} PCBT;

typedef struct Scheduler {
	PCBT processes[MAX_PROCESS];
	uint16_t current_pid;
	uint16_t index_p; // va a estar en un lugar libre, si esta lleno valdra -1
	uint16_t index_rr;
	int8_t quantum_remaining;
	uint32_t amount_processes;
	uint64_t next_pid;
} Scheduler;

typedef struct Scheduler *SchedulerInfo;


#endif
