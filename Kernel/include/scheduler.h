#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <process.h>
#include <utils.h>

#define QUANTUM 3 // 20ms
#define RR_INDEX(i) ((i + 1) % (MAX_PROCESS * PRIORITY4))

SchedulerInfo create_scheduler();
int8_t set_state(uint8_t new_state);
SchedulerInfo get_scheduler();
int8_t get_state();
uint16_t create_process(char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char *argv[], int argc, main_function rip);
unsigned int get_pid();
uint64_t scheduler(void *stack_pointer);
PCBT *update_quantum(void *stack_pointer);

#endif
