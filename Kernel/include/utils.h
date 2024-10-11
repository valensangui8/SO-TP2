#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include "interrupts.h"

#define MM_ADDRESS 0x50000
#define SCHEDULER_ADDRESS 0x60000
#define FREE_ 0x9FFFF
#define heap_size 4092
#define MEM_ADDRESS_MAX 0x9FFFF
#define FOREGROUND 1
#define BACKGROUND !FOREGROUND

typedef enum { BLOCKED = 0,
			   READY,
			   RUNNING,
			   ZOMBIE,
			   DEAD
} PCBState;

typedef enum { PRIORITY0 = 0,
			   PRIORITY1,
			   PRIORITY2,
			   PRIORITY3,
			   PRIORITY4
} Priority;

typedef struct PCB {
	char is_active;
	char name[32];
	uint16_t pid;
	uint16_t ppid;
	Priority priority;
	PCBState state;
	void *stack_pointer;
	char foreground;
	char **argv;
	int argc;
} PCBT;





#endif