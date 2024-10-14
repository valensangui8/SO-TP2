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

#define MAX_PROCESS 20
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

typedef int (*main_function)(char **argv, uint64_t argc);

typedef struct stack_registers {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;
} Stack_registers;

typedef struct stack {
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	Stack_registers my_registers;
} stack;

typedef stack *Process_stack;

typedef struct PCB {
	char is_active;
	char name[32];
	uint16_t pid;
	uint16_t ppid;
	Priority priority;
	PCBState state;
	Process_stack stack_process;
	char foreground;
	char **argv;
	int argc;
} PCBT;

typedef struct Scheduler {
	PCBT processes[MAX_PROCESS];
	PCBT *round_robin[MAX_PROCESS * PRIORITY4];
	uint16_t current_pid;
	uint16_t index_p; // va a estar en un lugar libre, si esta lleno valdra -1
	uint16_t index_rr;
	int8_t quantum_remaining;
	uint32_t amount_processes;
} Scheduler;

typedef struct Scheduler *SchedulerInfo;


#endif
