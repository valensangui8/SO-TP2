#ifndef _PROCESS_H
#define _PROCESS_H

#include <scheduler.h>
#include <stdint.h>
#include <utils.h>

#define DESCRIPTORS 3
#define IDLE_PID 0

typedef struct PCB {
	char name[32];
	uint16_t pid;
	uint16_t ppid;
	Priority priority;
	PCBState state;
	int16_t file_descriptors[DESCRIPTORS];
	void *stack_base;
	void *stack_pointer;
} PCBT;

void init_procces(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, int16_t file_descriptors[], void *stack_base, void *stack_pointer);
uint8_t has_children(unsigned int pid);
void wait_children(unsigned int ppid);
uint8_t kill_process(unsigned int pid);

#endif
