#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <utils.h>

#define QUANTUM 1
#define RR_INDEX(i) ((i + 1) % (MAX_PROCESS * PRIORITY4))
#define IDLE_PID 0

#define PID_WIDTH 6
#define STAT_WIDTH 8
#define RSP_WIDTH 10
#define RBP_WIDTH 10
#define COMMAND_WIDTH 20

SchedulerInfo create_scheduler();
int8_t set_state(uint8_t new_state);
SchedulerInfo get_scheduler();
int8_t get_state();
void* scheduler(void *stack_pointer);
PCBT *update_quantum(void *stack_pointer);

uint64_t create_process(char *name, Priority priority, char foreground, char *argv[], int argc, main_function rip);
void list_processes_state();
uint64_t kill_process(unsigned int pid);
uint16_t unblock_process(unsigned int pid);
uint16_t block_process(unsigned int pid);
void update_priority(unsigned int pid, Priority new_priority);

#endif
